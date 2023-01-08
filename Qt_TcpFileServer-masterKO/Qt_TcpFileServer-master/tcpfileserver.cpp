#include "tcpfileserver.h"

TCPFileserver::TCPFileserver(QWidget *parent)
    : QDialog(parent)
{
    totalBytes=0;
    byteReceived=0;
    fileNameSize=0;
    serverProgressBar=new QProgressBar;
    serverStatusLabel=new QLabel(QStringLiteral("서버 준비 완료"));
    startButton=new QPushButton(QStringLiteral("수신 대기 시작"));
    quitButton=new QPushButton(QStringLiteral("나가기"));
    buttonBox =new QDialogButtonBox;
    buttonBox->addButton(startButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("수신 파일"));

    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(&tcpserver,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

TCPFileserver::~TCPFileserver(){}
void TCPFileserver::start()
{
    startButton->setEnabled(false);
    byteReceived=0;
    fileNameSize=0;
    tcpserver.listen(QHostAddress::AnyIPv4,16998);
    serverStatusLabel->setText(QStringLiteral("수신대기중..."));
}
void TCPFileserver::acceptConnection()
{
    tcpServerConnection = tcpserver.nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this,SLOT(upDateServerProgress()));
    serverStatusLabel->setText(QStringLiteral("수신 연결"));
    tcpserver.close();
}
void TCPFileserver::upDateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);
    if(byteReceived<=sizeof(qint64)*2){
        if(fileNameSize == 0 && tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2){
            in>>totalBytes>>fileNameSize;
            byteReceived += sizeof(qint64)*2;
        }
        if(fileNameSize != 0 && tcpServerConnection->bytesAvailable() >= fileNameSize){
            in>>fileName;
            byteReceived += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this,QStringLiteral("서버"),QStringLiteral("파일을 열 수 없습니다. %1:\n%2").arg(fileName).arg(localFile->errorString()));
                return;
            }
        }
        else return;
    }
    if(byteReceived < totalBytes){
        byteReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    serverProgressBar->setMaximum(totalBytes);
    serverProgressBar->setValue(byteReceived);
    serverStatusLabel->setText(QStringLiteral("받은 %1 bytes").arg(byteReceived));
    if(byteReceived == totalBytes){
        tcpServerConnection->close();
        startButton->setEnabled(true);
        localFile->close();
        start();
    }
}
