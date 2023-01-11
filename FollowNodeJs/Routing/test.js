const express = require('express');             //express pacakage
const fs = require('fs');                       //fs pacakage
const app = express();

app.get('/', function(req, res){
    res.send('This is main');
    
});

app.get('/Paient', function(req, res){
    res.send('This is Paient Page.\n');
});

app.get('/Paient/1', function(req, res){
    res.json(
        {
            id : 1,
            name : "jaehyeon",
            age : 27,
            LocalImage : "D:/GitDesktop/TeamProject/FollowNodeJs/Images/google.png"    
        });
});

app.get('/Paient/1/Images', function(req, res){
    res.send('This is Paient 1 Images');
});

app.get('/Paient/1/Images/Panorama', function(req, res){
    res.send('This is Painent 1 Images Panorama');
    //panorama Image show
    fs.readFile('index.html', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    });
});

//이미지 페이지 라우팅
app.get('/Images', function(req, res){
    fs.readFile('./Images/google.png', function(error, data){
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    });
});

app.get('/Paient/2', function(req, res){
    res.json(
        {
            id : 2,
            name : "dahyeon",
            age : 26,
            LocalImage : "D:/GitDesktop/TeamProject/FollowNodeJs/Images/system.png"    
        });
});

app.get('/Paient/2/Images', function(req, res){
    res.send('This is Paient 2 Images');
});

app.get('/Paient/2/Images/Panorama', function(req, res){
    res.send('This is Painent 2 Images Panorama');
});

app.get('/Paient/3', function(req, res){
    res.json(
        {
            id : 3,
            name : "haeyoung",
            age : 52,
            LocalImage : "D:/GitDesktop/TeamProject/FollowNodeJs/Images/osstem.png"    
        });
});

app.get('/Paient/3/Images', function(req, res){
    res.send('This is Paient 3 Images');
});

app.get('/Paient/3/Images/Panorama', function(req, res){
    res.send('This is Painent 3 Images Panorama');
});

app.listen(3000, () => console.log('Server listen on port 3000...'));

