#include "filetranferdlg.h"
#include "ui_filetranferdlg.h"

FileTranferdlg::FileTranferdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileTranferdlg)
{
    ui->setupUi(this);

    /*파일 선송을 위한 서버*/
    fileServer = new QTcpServer(this);
    connect(fileServer, SIGNAL(newConnection()), SLOT(acceptConnection()));
    /*accpetConnection 파일 전송을 위한 커넥트*/
    if (!fileServer->listen(QHostAddress::Any, PORT_NUMBER)) {      /*PORT NUMBER = 8100*/
        QMessageBox::critical(this, tr("Chatting Server"),          /*파싱 위젯 과 파일 전송의 포트가 같으면*/
                              tr("Unable to start the server: %1.") /*통신이 안될 수 있으므로 다른포트로 저장*/
                              .arg(fileServer->errorString( )));    /*문제가 있을 시 메시지 박스 출력*/
        close( );
        return;
    }

    /*파일 전송 버튼*/
    connect(ui->FileSendButton, SIGNAL(clicked()), SLOT(sendFile()));

    fileSocket = new QTcpSocket(this);
    connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));
}

FileTranferdlg::~FileTranferdlg()
{
    delete ui;
}

/* 파일 전송 */
void FileTranferdlg::acceptConnection()
{
    qDebug("Connected, preparing to receive files!");

    QTcpSocket* receivedSocket = fileServer->nextPendingConnection();
    connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}

/*클라이언트에서 데이터를 읽는 함수*/
void FileTranferdlg::readClient()
{
    qDebug("Receiving file ...");
    QTcpSocket* receivedSocket = dynamic_cast<QTcpSocket *>(sender( ));
    QString filename;
    QString name;

    if (byteReceived == 0) { // 방금 데이터 수신을 시작, 이 데이터는 파일 정보입니다.
        progressDialog->reset();    /*프로그래스 바 리셋*/
        progressDialog->show();     /*다이얼로그 출력*/

        QDataStream in(receivedSocket);         /*데이터의 흐름을 읽는 변수 선언*/
        in >> totalSize >> byteReceived >> filename >> name;    /*1.파일의 전체 사이즈, 2.읽어드리는 데이터, 3.파일경로, 4.파일이름*/
        progressDialog->setMaximum(totalSize);              /*프로그래스 바는 전체사이즈로 설정한 가변수의 길이 만큼 지정*/
    }

    /*파일 전송 완료시*/
    if (byteReceived == totalSize) {
        qDebug() << QString("%1 receive completed").arg(filename);

        inBlock.clear();
        byteReceived = 0;           /*받아진 데이터 표시 0*/
        totalSize = 0;              /*파일 전체 크기 0*/
        progressDialog->reset();    /*프로그래스 바 리셋*/
        progressDialog->hide();     /*다이얼로그는 숨겨진 상태*/
        file->close();              /*open되었던 파일은 다시 닫아주기*/
        delete file;
        /*보내는 데이터를 초기화*/
    }
}


////////////////////////////////////////////////////////////////////////////////////////////


/*파일 전송 슬롯 함수*/
void FileTranferdlg::sendFile( )
{
    //가변수 초기화
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString filename = QFileDialog::getOpenFileName(this);

    file = new QFile(filename);
    file->open(QFile::ReadOnly);

    qDebug() << QString("file %1 is opened").arg(filename);
    progressDialog->setValue(0);   //파일을 보내지 않으면 처음 상태는 0으로 조정

    if(!isSent)         /*처음 전송될 때만 연결이 신호 연결을 생성할 때 발생합니다.*/
    {
        fileSocket->connectToHost("192.168.",
                                  PORT_NUMBER);
        isSent = true;
    }

    //처음으로 보낼 때 connectToHost는 send를 호출하기 위해 연결 신호를 시작하고 두 번째 후에는 send를 호출해야 합니다.
    byteToWrite = totalSize = file->size(); /*남은 데이터 사이즈를 정의*/
    loadSize = 1024; // 매번 전송되는 데이터의 크기

    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out << qint64(0) << qint64(0) << filename;

    totalSize += outBlock.size();
    //전체 크기는 파일 크기에 파일 이름 및 기타 정보의 크기를 더한 것입니다.
    byteToWrite += outBlock.size();

    out.device()->seek(0);
    //바이트 스트림의 시작 부분으로 돌아가 전체 크기와 파일 이름 및 기타 정보 크기인 qint64를 앞에 씁니다.
    out << totalSize << qint64(outBlock.size());

    fileSocket->write(outBlock);/*읽은 파일을 소켓으로 보냅니다.*/

    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);
    progressDialog->show();

    qDebug() << QString("Sending file %1").arg(filename);
}

void FileTranferdlg::goOnSend(qint64 numBytes)
{
    byteToWrite -= numBytes; // 남은 데이터의 크기
    outBlock = file->read(qMin(byteToWrite, numBytes));
    fileSocket->write(outBlock);

    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);

    if (byteToWrite == 0) { // 데이터를 모두 보냈을 경우
        qDebug("File sending completed!");
        progressDialog->reset();
    }
}

