var express = require('express');
var router = express.Router();

var fs = require('fs');
var path = require('path');
var mime = require('mime');
const { Router } = require('express');

router.get('D:/GitDesktop/TeamProject/FollowNodeJs/Routing/Images/google.png', function(req, res, next){
    var upload_folder = 'D:/GitDesktop/TeamProject/FollowNodeJs/DownloadTest/router';
    var file = upload_folder + req.params.file_name;

    try{
        if(fs.existsSync(file)){
            var filename = path.basename(file);
            var mimetype = mime.getType(file);

            res.setHeader('Content-disposition', 'attachment: filename=' + filename);
            res.setHeader('Content-type', mimetype);

            var filestream = fs.createReadStream(file);
            filestream.pipe(res);
        }else{
            res.send('해당 파일이 없습니다.');
            return;
        }
    }
    catch(e){
        console.log(e);
        res.send('파일을 다운로드하는 중에 에러가 발생했습니다.');
        return;
    }
});

module.exports = router;