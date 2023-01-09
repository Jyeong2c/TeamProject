#ifndef FILESENDERDLG_H
#define FILESENDERDLG_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>

namespace Ui {
class FileSenderDlg;
}

class FileSenderDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSenderDlg(QWidget *parent = nullptr);
    ~FileSenderDlg();

public slots:
#if 1
    void connectDlg();                     // connect() 모음
#endif
    void start();                       // 파일 보내기 시작 슬롯 함수
    void startTransfer();               // 파일 문자 변환 시작 함수
    void updateClientProgress(qint64);  // 파일 보내는 프로그래스 과정 업데이트 슬롯 함수
    void openFile();                    // 파일 열기 슬롯 함수

private:
    Ui::FileSenderDlg *ui;
#if 1
    QTcpSocket tcpClient;               // 파일 소켓 클라이언트

    qint64 totalBytes;                  // 레코드 데이터의 총 길이
    qint64 bytesWritten;                // 데이터의 길이를 기록합니다.
    qint64 bytesToWrite;                // 데이터의 길이를 기록합니다. 아직 쓰지 않았습니다.
    qint64 loadSize;                    // 각 데이터의 길이를 기록합니다.

    QString IP;                         // 파일을 전송할 Server IP 주소
    QString fileName;                   // 파일의 이름 변수
    QFile * localFile;                  // 지정된 파일 변수
    QByteArray outBlock;                // 데이터 출력 저장 버퍼
#else

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
#endif

};

#endif // FILESENDERDLG_H
