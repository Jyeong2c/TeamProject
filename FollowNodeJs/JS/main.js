var http = require('http');
var fs = require('fs');

var app = http.createServer(function(request, response){
    console.log(request.url);

    if(request.url==='/'){              //타입을 맞춰주기 위한 === 대입 연산자
        response.writeHead(200);
        response.end(fs.readFileSync(__dirname + '\\index.html'));
    }
    if(request.url ==='/picture'){
        fs.readFile('./picture/osstem.png', function(err, data){
            console.log('picture loading...');
            response.writeHead(200);
            response.write(data);
            response.end();
        });
    }
});

app.listen(3000);