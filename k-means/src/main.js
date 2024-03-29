import { readFile } from "fs";
import _ from 'lodash';
const { max, min, minBy, mean } = _;
import ChartJsImage from 'chartjs-to-image';
class Cluserizer {
    constructor() {
        this.NUMBER_OF_CLUSTERS = 8;
        this.delim = ' ';
        this.filename = 'unbalance.txt';
        this.restarts = 50;
        this.points = [];
        this.clusters = [];
        this.currentBest = null;
        this.bestClusters = [];
        this.readData().then(arr => {
            this.points = arr.map(([x, y], i) => ({ x: Number(x), y: Number(y), id: i }));
            this.initPoints();
            this.randomRestart();
        });
    }
    initPoints() {
        this.upper_x = max(this.points.map(p => p.x));
        this.upper_y = max(this.points.map(p => p.y));
        this.lower_x = min(this.points.map(p => p.x));
        this.lower_y = min(this.points.map(p => p.y));
    }
    initClusters() {
        this.clusters = [];
        for (let index = 0; index < this.NUMBER_OF_CLUSTERS; index++) {
            const x = Math.random() * (this.upper_x - this.lower_x) + this.lower_x;
            const y = Math.random() * (this.upper_y - this.lower_y) + this.lower_y;
            const center = { x, y };
            this.clusters.push({ center, id: index, points: new Map() });
        }
        this.calcLoop();
    }
    randomRestart() {
        for (let index = 0; index < this.restarts; index++) {
            this.initClusters();
            console.log(index);
        }
        this.drawPic();
    }
    pointsCalc() {
        this.points.forEach(p => {
            const closest = minBy(this.clusters.map(c => ({ dist: this.dist(p, c.center), cluster: c })), 'dist');
            p.dist = closest.dist;
            closest.cluster.points.set(p.id, p);
        });
    }
    clusterCalc() {
        this.clusters.forEach(c => {
            const newX = mean([...c.points.values()].map(p => p.x));
            const newY = mean([...c.points.values()].map(p => p.y));
            c.center.x = isNaN(newX) ? c.center.x : newX;
            c.center.y = isNaN(newY) ? c.center.y : newY;
        });
    }
    calcLoop() {
        let index = 1;
        let lastCenters = [];
        while (!this.sameCenters(lastCenters, this.clusters.map(c => c.center))) {
            lastCenters = [];
            this.clusters.forEach(c => {
                lastCenters.push(Object.assign({}, c.center));
            });
            this.clusters.forEach(c => c.points = new Map());
            this.pointsCalc();
            this.clusterCalc();
            if (lastCenters.some(p => isNaN(p.x))) {
                return;
            }
        }
        const scatter = this.withinPointScatter();
        if (this.currentBest === null || scatter < this.currentBest) {
            this.currentBest = scatter;
            this.bestClusters = this.clusters.map(c => ({ ...c }));
        }
    }
    withinPointScatter() {
        return this.clusters.map(({ points, center }) => {
            const pointss = [...points.values()];
            if (pointss.length === 0)
                return 0;
            return pointss.map(p => {
                const dist = this.dist(p, center);
                return dist * dist;
            }).reduce((p, c) => c + p);
        }).reduce((p, c) => c + p);
    }
    sameCenters(oldCenters, newCenters) {
        if (oldCenters.length !== newCenters.length) {
            return false;
        }
        for (let index = 0; index < oldCenters.length; index++) {
            if (!this.eqPoints(oldCenters[index], newCenters[index])) {
                return false;
            }
        }
        return true;
    }
    dist(a, b) {
        const diffx = a.x - b.x;
        const diffy = a.y - b.y;
        return Math.sqrt(diffx * diffx + diffy * diffy);
    }
    eqPoints(p1, p2) {
        const tolerance = 0.0001;
        return Math.abs(p1.x - p2.x) < tolerance && Math.abs(p1.y - p2.y) < tolerance;
    }
    async readData() {
        return new Promise((resolve) => {
            readFile(this.filename, (_, data) => {
                const result = data.toString().split('\n').map(l => l.split(this.delim));
                resolve(result);
            });
        });
    }
    drawPic() {
        const datasets = this.bestClusters.map(c => ({ data: [...c.points.values()].map(({ x, y }) => ({ x, y, r: 3 })) }));
        const myChart = new ChartJsImage();
        const chart = myChart.setConfig({
            type: 'bubble',
            data: {
                datasets
            }
        });
        myChart.toFile('result.png');
    }
}
const a = new Cluserizer();
//# sourceMappingURL=main.js.map