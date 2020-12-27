import { readFile } from "fs";
import _ from "lodash";
const { maxBy, shuffle, groupBy, values } = _;

class Classifier {
  // possitiveClass = 'yes';
  // negativeClass = 'no';
  // numberOfAttributes = 4;
  // fileName = '../test_data.txt';
  // minK = 1;
  // folds = 3;

  possitiveClass = 'recurrence-events';
  negativeClass = 'no-recurrence-events';
  numberOfAttributes = 9;
  fileName = '../breast-cancer.data';
  minK = 5;
  folds = 5;

  dataset;
  root = {};
  attributeValues;
  databackup;

  constructor() { }

  async readData() {
    return new Promise((resolve) => {
      readFile(this.fileName, (_, data) => {
        resolve(data.toString().split('\n'))
      })
    });
  }

  async init() {
    const fileLines = (await this.readData()).map(l => l.split(','))
    this.dataset = fileLines.map(([className, ...attributes]) => {
      return { className, attributes };
    });

    this.fillMissing();
    this.attributeValues = Array.from(Array(this.numberOfAttributes).keys())
      .map(i => new Set(this.dataset.map(inst => inst.attributes[i])))
      .map(set => Array.from(set.keys()))

    // console.log(this.attributeValues);
    // this.buildTree(this.root, this.dataset, []);
    // console.log(this.root);
    // console.log(this.root.nodes.get('sunny'));
    // console.log(this.root.nodes.get('rainy'));
  }

  fillMissing() {
    const indexes = Array.from(Array(this.numberOfAttributes).keys());
    indexes.forEach(i => {
      const group = groupBy(this.dataset, inst => inst.attributes[i]);
      const mostCommon = maxBy(values(group), g => g.length)[0].attributes[i];
      this.dataset = this.dataset.map(inst => {
        if (inst.attributes[i] === '?') {
          inst.attributes[i] = mostCommon;
        }
        return inst;
      });
    });
  }

  entropy(instances) {
    if (instances.length === 0) return 0;
    const total = instances.length;
    const positive = instances.filter(i => i.className === this.possitiveClass).length;
    const p = positive / total; const q = (total - positive) / total;
    if (p === 0) return - q * Math.log2(q);
    if (q === 0) return - p * Math.log2(p);
    return -p * Math.log2(p) - q * Math.log2(q);
  }

  getMajorityClass(instances) {
    const positive = instances.filter(i => i.className === this.possitiveClass).length;
    const negative = instances.filter(i => i.className === this.negativeClass).length;
    return positive > negative ? this.possitiveClass : this.negativeClass;
  }

  getRandomClass() {
    return shuffle([this.possitiveClass, this.negativeClass])[0];
  }

  buildTree(root, instances, used) {
    const entropy = this.entropy(instances)
    if (entropy === 0 || instances.length <= this.minK) {
      root.attribute = -1;
      root.className = instances.length ? this.getMajorityClass(instances) : this.getRandomClass();
      return;
    }
    const bestIndex = this.getBestAttribute(instances, used);
    root.nodes = new Map();
    root.attribute = bestIndex;
    this.attributeValues[bestIndex].forEach(v => {
      const subset = instances.filter(i => i.attributes[bestIndex] === v)
      const node = {};
      this.buildTree(node, subset, [...used, bestIndex]);
      root.nodes.set(v, node);
    });
  }

  getBestAttribute(instances, used) {
    const setEntropy = this.entropy(instances);
    const attributes = Array.from(Array(this.numberOfAttributes).keys())
      .filter(a => !used.includes(a))
      .map(index => {
        let currentGain = setEntropy;
        this.attributeValues[index].forEach(v => {
          const filtered = instances.filter(ins => ins.attributes[index] === v);
          const entropy = this.entropy(filtered);
          currentGain -= filtered.length / instances.length * entropy;
        })
        return { gain: currentGain, index };
      })
    return maxBy(attributes, a => a.gain).index;
  }

  getSetIndexes() {
    const setIndexes = [];
    let setSize = Math.floor(this.dataset.length / this.folds);
    let reminder = this.dataset.length % this.folds;
    let increment = 0;
    for (let i = 0; i < this.folds; i++) {
      let val = i * setSize + increment;
      if (i < reminder) {
        increment++;
      }
      setIndexes.push(val);
    }
    setIndexes.push(this.dataset.length);
    return setIndexes;
  }

  crossValidate() {
    this.databackup = shuffle(this.dataset);
    const setIndexes = this.getSetIndexes();
    let total = 0;
    for (let i = 0; i < this.folds; i++) {
      const testData = this.databackup.filter((_, index) => setIndexes[i] <= index && index < setIndexes[i + 1]);
      const buildData = this.databackup.filter((_, index) => !(setIndexes[i] <= index && index < setIndexes[i + 1]));
      
      const model = {};
      this.buildTree(model, buildData, []);

      const accuracy = this.testModel(model, testData);
      total += accuracy;
      console.log(i, accuracy);
    }

    return total / this.folds;
  }

  testModel(model, testData) {
    const matches = testData.map(inst => {
      const result = this.testInstance(model, inst.attributes);
      return result === inst.className;
    }).filter(m => m);
    return matches.length / testData.length;
  }

  testInstance(root, attributes) {
    if (root.attribute === -1) {
      return root.className;
    }
    return this.testInstance(root.nodes.get(attributes[root.attribute]), attributes);
  }
}

const a = new Classifier();
await a.init();
console.log(a.crossValidate());