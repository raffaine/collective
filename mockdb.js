
var mockdb = {
users: [{username:'john', 
              name:'John', 
              wallet_status:30.0,
              work_dao:[
                {id:'1',
                 power:5000,
                 tasks:[{id:'1'}]}],
              consume_dao:[],
              skills:[
                {name:'farming', level:3}]
              }],
daos: [{
  id:'1',
  name:'farmers',
  contracts:[{id:'1'}],
  total_power:100000
}],

tasks: [
  {
    id:'1',
    name:'Gardening Tomato Field',
    skills_required:[],
    resources_required:[]
  },
  {
    id:'2',
    name:'Getting Garden Resources',
    skills_required:[],
    resources_required:[]
  }
],

contracts: [{
  id:'1',
  name:'1 Pound of Tomatoes',
  tasks:[{id:'1', index:0.1}, {id:'2', index:0.01}]
}]
}

function func() { return mockdb;}


module.exports = func;