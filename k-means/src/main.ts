import { readFile } from "fs";
import _ from 'lodash';
const { max, min, minBy, mean } = _;
import ChartJsImage from 'chartjs-to-image';


interface Point {
  id: number;
  x: number;
  y: number;
  dist: number;
}

interface Cluster {
  id: number;
  points: Map<number, Point>;
  center: Point;
}

class Cluserizer {
  upper_x: number;
  upper_y: number;
  lower_x: number;
  lower_y: number;
  readonly NUMBER_OF_CLUSTERS = 8;
  delim = ' ';  // '\t' ot ' '
  filename = 'unbalance.txt';
  points: Point[] = [];
  clusters: Cluster[] = [];

  constructor() {
    this.readData().then(arr => {
      this.points = arr.map(([x, y], i) => ({ x: Number(x), y: Number(y), id: i }) as Point);
      this.initClusters();
    });
  }

  initClusters() {
    this.upper_x = max(this.points.map(p => p.x));
    this.upper_y = max(this.points.map(p => p.y));
    this.lower_x = min(this.points.map(p => p.x));
    this.lower_y = min(this.points.map(p => p.y));
    for (let index = 0; index < this.NUMBER_OF_CLUSTERS; index++) {
      const x = Math.random() * (this.upper_x - this.lower_x) + this.lower_x;
      const y = Math.random() * (this.upper_y - this.lower_y) + this.lower_y;
      const center = { x, y } as Point
      this.clusters.push({ center, id: index, points: new Map() });
    }
    this.calcLoop();
  }

  pointsCalc() {
    this.points.forEach(p => {
      const closest = minBy(this.clusters.map(c => ({ dist: this.dist(p, c.center), cluster: c })), 'dist');
      p.dist = closest.dist;
      closest.cluster.points.set(p.id, p);
    })
  }

  clusterCalc() {
    this.clusters.forEach(c => {
      // console.log('before',c.center);

      c.center.x = mean(Array.from(c.points.values()).map(p => p.x));
      c.center.y = mean(Array.from(c.points.values()).map(p => p.y));
      // console.log('after',c.center);
    });
  }

  calcLoop() {
    let index = 1;
    let lastCenters: Point[] = [];
    while (!this.sameCenters(lastCenters, this.clusters.map(c => c.center))) {
      console.log('iteration: ', index++);
      console.log(this.clusters);
      
      lastCenters = [];
      this.clusters.forEach(c => {
        lastCenters.push(Object.assign({}, c.center));
      })
      this.clusters.forEach(c => c.points = new Map());
      this.pointsCalc();
      this.clusterCalc();

      console.log(lastCenters);
      // console.log(this.clusters.map(c => c.center));
    }

    this.drawPic()
  }

  sameCenters(oldCenters: Point[], newCenters: Point[]) {
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

  dist(a: Point, b: Point) {
    const diffx = a.x - b.x;
    const diffy = a.y - b.y;
    return Math.sqrt(diffx * diffx + diffy * diffy)
  }

  eqPoints(p1: Point, p2: Point) {
    const tolerance = 0.0001;
    return Math.abs(p1.x - p2.x) < tolerance && Math.abs(p1.y - p2.y) < tolerance;
  }

  async readData() {
    return new Promise<string[][]>((resolve) => {
      readFile(this.filename, (_, data) => {
        const result = data.toString().split('\n').map(l => l.split(this.delim))
        resolve(result)
      })
    });
  }

  drawPic() {
    const datasets = this.clusters.map(c => ({ data: [...c.points.values()].map(({x,y}) => ({x, y, r: 5})) }));
    const myChart = new ChartJsImage();
    const chart = myChart.setConfig({
      type: 'bubble',
      data: {
        datasets
      }
    })
    myChart.toFile('result.png');
  }
}

const a = new Cluserizer();