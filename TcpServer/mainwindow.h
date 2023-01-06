#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*데이터를 받는 시그널 함수*/
signals:
    void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    /*블록이 완전히 수신될 때까지 데이터를 저장할 버퍼가 필요합니다.*/
    QHash<QTcpSocket*, QByteArray*> buffers;
    /*블록이 완전히 수신되었는지 확인하기 위해 크기를 저장해야 합니다.*/
    QHash<QTcpSocket*, qint32*> sizes;
    /*클라이언트와 연동되는 포트 번호*/
    const int PORT_NUMBER = 1024;
};
#endif // MAINWINDOW_H
