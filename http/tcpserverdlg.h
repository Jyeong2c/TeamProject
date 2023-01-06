#ifndef TCPSERVERDLG_H
#define TCPSERVERDLG_H

#include <QDialog>
#include <QtCore>
#include <QtNetwork>
#include <QString>

namespace Ui {
class TCPServerDlg;
}

class TCPServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TCPServerDlg(QWidget *parent = nullptr);
    ~TCPServerDlg();

    /*데이터를 받는 시그널 함수*/
signals:
    void dataReceived(QByteArray);

private slots:

    void newConnection();   //연결 상태 확인 함수
    void disconnected();    //비연결 상태 확인 함수
    void readyRead();       //읽기 준비 상태 함수

private:
    Ui::TCPServerDlg *ui;
    QTcpServer *server;
    /*블록이 완전히 수신될 때까지 데이터를 저장할 버퍼가 필요합니다.*/
    QHash<QTcpSocket*, QByteArray*> buffers;
    /*블록이 완전히 수신되었는지 확인하기 위해 크기를 저장해야 합니다.*/
    QHash<QTcpSocket*, qint32*> sizes;
    /*클라이언트와 연동되는 포트 번호*/
    const int PORT_NUMBER = 1024;
};

#endif // TCPSERVERDLG_H
