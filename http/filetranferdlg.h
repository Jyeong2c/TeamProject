#ifndef FILETRANFERDLG_H
#define FILETRANFERDLG_H

#include "qtcpserver.h"
#include <QDialog>
#include <QFile>
#include <QProgressDialog>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

namespace Ui {
class FileTranferdlg;
}

class FileTranferdlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileTranferdlg(QWidget *parent = nullptr);
    ~FileTranferdlg();

private slots:
    void acceptConnection();            /*파일 서버 연결 여부*/
    void readClient();

    /////////////////////////////////////////////////////////////////////////////////////

    /*파일및 프로토콜을 보내는 슬롯*/
    void sendFile();
    void goOnSend(qint64);

private:
    Ui::FileTranferdlg *ui;
    const int PORT_NUMBER = 8100;

    QTcpServer* fileServer;     /*파일만 받는 서버 추가*/

    QFile* file;                /*파일 생성 변수*/
    QProgressDialog* progressDialog;    /*프로그래스바 */
    qint64 totalSize;           /*데이터 토탈 사이즈 변수*/
    qint64 byteReceived;       /*데이터 받기 변수*/

    QByteArray inBlock;        //데이터 블록
    /////////////////////////////////////////////////////////////////////////////////////
    QTcpSocket* fileSocket;             /*파일 전용 소켓*/
    qint64 loadSize;
    qint64 byteToWrite;
    QByteArray outBlock;

    bool isSent;
};

#endif // FILETRANFERDLG_H
