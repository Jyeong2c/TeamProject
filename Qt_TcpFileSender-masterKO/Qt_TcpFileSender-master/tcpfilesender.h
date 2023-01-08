#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include<QtNetwork>
#include<QtWidgets>

class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = 0);
    ~TcpFileSender();

public slots:
    void start();
    void startTransfer();
    void updateClientProgress(qint64);
    void openFile();

private:
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *startButton;
    QPushButton *openButton;
    QPushButton *quitButton;
    QTcpSocket tcpClient;
    QLineEdit *lineEdit1;
    QLabel *label1;
    QLabel *label2;
    QSpinBox *spinbox;

    qint64 totalBytes; // 레코드 데이터의 총 길이
    qint64 bytesWritten; // 데이터의 길이를 기록합니다.
    qint64 bytesToWrite; // 데이터의 길이를 기록합니다. 아직 쓰지 않았습니다.
    qint64 loadSize; // 각 데이터의 길이를 기록합니다.

    QString fileName;
    QFile * localFile;
    QByteArray outBlock; // 데이터 출력 저장 버퍼

};

#endif // TCPFILESENDER_H
