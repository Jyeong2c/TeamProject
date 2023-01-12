var fs = require('fs');             //파일 로드 사용
var express = require('express');   //웹 서버 사용
var app = express();
var http = require('http');

//웹 서버 생성
var server = http.createServer(app).listen(3303, function(){
    console.log('Server Running on 3303 port.');
});

//index 페이지 라우팅
app.get('/', function(req, res){
    fs.readFile('index.html', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    });
});

//File 다운로드
app.get('/download/:id', function(req, res){
    var filename = req.params.id;

    filepath = __dirname + "/files/" + filename;
    res.download(filepath);
});



