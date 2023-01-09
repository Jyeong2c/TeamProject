#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QFile>
#include <iostream>
#include <QScrollArea>

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class IdNameBody;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

    void connectWidget();

    /*파일 수신 시작 및 상태 알림 슬롯*/
    void start();
    void acceptConnection();
    void upDateServerProagress();

private slots:
    void on_SelectSend_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;

    std::unique_ptr<QFile> file;

    bool fd_flag = false;
    bool send_flag = false;
    const int PORT_NUMBER = 1024;
    int cnt = 0;

    /*파일 수신 변수 설정*/
    QTcpServer fileReceiver;
    QTcpSocket *tcpServerConnection;

    /*프로그래스 상태 변수*/
    qint64 totalBytes;
    qint64 byteReceived;
    qint64 fileNameSize;

    QString fileName;       //  파일 이름 변수
    QFile *localFile;       //  지정된 파일 변수
    QByteArray inBlock;     //  데이터 입력 저장 버퍼
};
#endif // WIDGET_H
