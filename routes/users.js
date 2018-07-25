var express = require('express');
var router = express.Router();

var mockdb = require('../mockdb')();




/* GET users listing. */
router.get('/', function(req, res, next) {
  console.log(mockdb);
  var in_user = req.query.user;
  var usr = mockdb.users.find( u => u.username == in_user);
  if(usr) {
    var workdaos = [];
    usr.work_dao.forEach(function(dao_cur) {
      var it = mockdb.daos.find(d => d.id == dao_cur.id);
      if (it) {
        var userdao = Object.assign({}, it);
        userdao.mypower = dao_cur.power;
        userdao.pct = (userdao.mypower / userdao.total_power) * 100;
        workdaos.push(userdao);
      }
    }, this);

    res.render('users', { title: 'Collective',
                          user: usr,
                          workdaos });
  } else {
    res.render('error', {});
  }
  
});

module.exports = router;
