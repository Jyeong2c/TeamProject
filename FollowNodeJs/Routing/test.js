const express = require('express');
const app = express();
var boardRouter = require('./router/board.js');

app.get('/', function(req, res){
    res.send('This is main');
});

app.use('/board', boardRouter);

app.listen(3000, () => console.log('Server listen on port 3000...'));

// app.get('/', function(req, res){
//     res.send('This is main');
// });

// app.get('/board', function(req, res){
//     res.send('This is board list');
// });

// app.get('/board/write', function(req, res){
//     res.send('This is board write');
// });

// app.get('/board/detail', function(req, res){
//     res.send('This is board detail');
// });

// app.get('/board/update', function(req, res){
//     res.send('This is board update');
// });

// app.get('/board/update/update_1', function(req, res){
//     res.send('This is board update/update_1');
// });

// app.listen(3000, () => console.log('Server listen on port 3000...'));

// 여기서 app은 express의 인스턴스
// app.get('/', function(req, res){
//     res.send('GET request to the root');
// });

// app.post('/', function(req, res){
//     res.send('POST request to the root');
// });

// app.get('/path', function(req, res){
//     console.log('GET request to the /path');
//     next();                      //둘 이상의 핸들러 함수를 사용하기 위함/
// }, function(req, res){
//     res.send('this is /path page!');
// });

