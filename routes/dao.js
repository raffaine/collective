var express = require('express');
var router = express.Router();

var mockdb = require('../mockdb')();

router.get('/dao/:daoId', function(req, res, next) {
  var in_dao = req.params.daoId;
  
  var c_dao = mockdb.daos.find(d => d.id == in_dao);
  if(c_dao) {
    var cts = [];
    c_dao.contracts.forEach(function(contract){
        var ctc = mockdb.contracts.find( c => c.id == contract.id);
        if(ctc) {
            ctc.tasks.forEach(function(c_tsk){
                t = mockdb.tasks.find( ts => ts.id == c_tsk.id );
                if (t) {
                    c_tsk.info = t;
                }
            });
            cts.push(ctc);
        }
    });

    res.render('dao', { title: 'Collective',
                        dao : c_dao,
                        contracts: cts
                      });      
  } else {
      res.render('error', {});
  }
  
});

module.exports = router;