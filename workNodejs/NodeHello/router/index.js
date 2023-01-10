var express = require('express');
var router = express.Router();
var path = require('path');
var HelloWorld = require('./sample/HelloWorld.js');

router.get('/', function(req, res){
    res.sendFile(path.join(__dirname, "../public/main.html"));
});

router.use('/helloworld', HelloWorld);

module.exports = router;

