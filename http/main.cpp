// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QDir>
#include <QScreen>
#include <QHttpServer>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>

/*Json 클래스 호출*/
#include "idnamebody.h"

#include "httpwindow.h"

using namespace std;

class IdNameBody;

int main(int argc, char *argv[])
{
    /* json/text/ini 파일 생성(현재는 json 형태의 파일을 생성)*/
    QString path = "D:/GitDesktop/TeamProject/JsonHttp/test.json";   //json파일 인코딩
    IdNameBody INB;
    INB.CreateJson(path);

    QApplication app(argc, argv);
    HttpWindow httpWin;
    const QRect availableSize = httpWin.screen()->availableGeometry();
    httpWin.resize(availableSize.width() / 5, availableSize.height() / 5);
    httpWin.move((availableSize.width() - httpWin.width()) / 2, (availableSize.height() - httpWin.height()) / 2);
#ifdef Q_OS_ANDROID         //안드로이드에서 정의되는 함수, 사용하지 않는 상태
    httpWin.showMaximized();
#else
    httpWin.show();         //현재 윈도우로 쓰는 상태를 표시
#endif

    QHttpServer httpServer; //http 서버 변수 선언

    /*람다 함수로 구현한 라우트 함수*/
    httpServer.route("/", [&path]() {
#if 0
        //return "Hello world";           //웹 페이지에 출력할 구문
        //JSON 시리얼라이징 하지 않은 날것의 텍스트문
        QString str =   "{\r\n"
                        "\t\"date\": \"12-31-2022\", \r\n"
                        "\t\"milliseconds_since_epoch\": \"1672277402417\", \r\n"
                        "\t\"time\": \"01:30:02 PM\" \r\n"
                        "}";
        return str;
#else
        /*어플리케이션 경로*/
        //QString ApplicationPath = QApplication::applicationDirPath();

        /*읽어드릴 파일명*/
        QFile file(path);

        if(!file.open(QFile::ReadOnly|QFile::Text)) // 읽기 전용, 텍스트로 파일 열기
        {
            if(!file.exists()) // 파일이 존재하지 않으면...
            {
                // 파일이 존재하지 않으면...여기에 동작 적용
                qDebug() << "No Such File..";
            }
            else
            {
                // 파일을 열수 없거나 파일 손상 등 여러 요인작용...
                qDebug() << "Somethig wrong file open..";
            }
        }


        QTextStream OpenFile(&file);
        QString str;
        while(!OpenFile.atEnd())            //파일을 끝까지 읽어서
        {
            str += OpenFile.readLine();      //한 라인씩 읽어서 변수에 적용
            str += "\r\n";
        }

        return str;  //여기서 리턴하면 마지막 한줄만 출력
        file.close();        //파일 닫기

#endif
    });

    httpServer.afterRequest([](QHttpServerResponse &&resp) {
        resp.setHeader("Server", "Super server!");
        return std::move(resp);
    });


    //생성하는 서버는 번호를 따로 만들어서 메인문에 생성
    const auto port = httpServer.listen(QHostAddress::Any, 8001);
    if (!port) {
        qDebug() << QCoreApplication::translate(
                        "QHttpServerExample", "Server failed to listen on a port.");
        return 0;
    }

    //emit SIGNAL(port);
    qDebug() << QCoreApplication::translate(
                    "QHttpServerExample", "(main)Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

//test

    return app.exec();
}



