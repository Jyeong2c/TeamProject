var express = require('express')
var app = express()
var router = require('./router/index')

app.listen(3000, function(){
    console.log("start!! express server on port 3000")
});

app.use(express.static('public'))
app.set('view engine', 'ejs')
app.use(router)
