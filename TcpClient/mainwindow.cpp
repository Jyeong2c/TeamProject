#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <iostream>

//using namespace std;

static inline QByteArray IntToArray(qint32 source); //정수형을 문자열로 바꾸는 인라인 함수

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    fd_flag = connectToHost("127.0.0.1");       //localhost

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(fd_flag)
    {
//        QString status = QString("Processing file %1 of %2: %3")
//                        .arg(i).arg(total).arg(fileName);@
        QString textData, sendData;
        textData = QString("Button click : %1").arg(cnt);
        sendData = QString("Socket data : %1").arg(cnt);

        ui->textBrowser->insertPlainText(textData);
        send_flag = writeData(sendData.toStdString().c_str());

        if(!send_flag)
            ui->textBrowser->insertPlainText("Socket send fail\n");
        else
            cnt++;
    }
}

/*호스트를 커넥트하기 위한 함수*/
bool MainWindow::connectToHost(QString host)
{
    socket->connectToHost(host, PORT_NUMBER);       //id address, port
    return socket->waitForConnected();
}

/*Data 쓰기*/
bool MainWindow::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(IntToArray(data.size()));
        socket->write(data);
        return socket->waitForBytesWritten();
    }else{
        return false;
    }
}

/*정수를 배열로 바꾸는 함수 구현부*/
QByteArray IntToArray(qint32 source)    //숫자가 4바이트인지를 확인하기 위해 qint32(signed int)자료형을 사용
{
    /*캐스트 사용을 피하고, 객체를 직렬화 하는 Qt방식*/
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}


