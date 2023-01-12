var express = require('express');
var fs = require('fs');
var app = express();

app.get('/', function(req, res){
    res.send("this is main page");
});

app.get('/patient', function(req, res){
    res.send("this is patient page");
});

// 환자 정보 1
app.get('/patient/1', function(req, res){
    res.json(
        {
            id : 1,
            name : "jae hyeon",
            age : 27,
            localimage : "D:/GitDesktop/TeamProject/FollowNodeJs/Test/picture/google.png"
        });
});

app.get('/patient/1/picture', function(req, res){
    res.send("this is patient 1 picture page");
});


app.get('/patient/1/picture/panorama', function(req, res){
    fs.readFile('main.html', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    }); 
});

//환자 1의 이미지 페이지 라우팅
app.get('/picture', function(req, res){
    fs.readFile('./picture/google.png', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
        console.log(":paitent/1/panorama");
    });
});


// 환자 정보 2
app.get('/patient/2', function(req, res){
    res.json(
        {
            id : 2,
            name : "da hyeon",
            age : 26,
            localimage : "D:/GitDesktop/TeamProject/FollowNodeJs/Test/picture/osstem.png"
        });
});

app.get('/patient/2/picture', function(req, res){
    res.send("this is patient 2 picture page");
});


app.get('/patient/2/picture/panorama', function(req, res){
    fs.readFile('main1.html', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    });
});

//환자 2의 이미지 페이지 라우팅
app.get('/picture1', function(req, res){
    fs.readFile('./picture1/Naver.png', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
        console.log(":patient/2/panorama");
    });
});


app.listen(3000, function(){
    console.log('Server Start...');
})