#include "tcpfilesender.h"

TcpFileSender::TcpFileSender(QWidget *parent) : QDialog(parent)
{
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    clientProgressBar = new QProgressBar;
    clientStatusLabel = new QLabel(QStringLiteral("클라이언트 준비"));
    startButton = new QPushButton(QStringLiteral("전송 시작"));
    openButton = new QPushButton(QStringLiteral("파일 열기"));
    quitButton = new QPushButton(QStringLiteral("나가기"));
    lineEdit1 = new QLineEdit(this);
    label1 = new QLabel(this);
    label2 = new QLabel(this);
    spinbox = new QSpinBox(this);
    label1->setText("Server IP:");
    label2->setText("Server Port:");
    spinbox->setMaximum(65535);
    startButton->setEnabled(false);
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(openButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(clientProgressBar);
    mainLayout->addWidget(clientStatusLabel);
    mainLayout->addWidget(label1);
    mainLayout->addWidget(lineEdit1);
    mainLayout->addWidget(label2);
    mainLayout->addWidget(spinbox);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("파일 전송 클라이언트"));
    connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(openButton,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(&tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
}

TcpFileSender::~TcpFileSender(){}

QString ip;

void TcpFileSender::start(){
    ip = lineEdit1->text();
    startButton->setEnabled(false);
    bytesWritten = 0;
    clientStatusLabel->setText(QStringLiteral("서버 찾는중......"));
    tcpClient.connectToHost(QHostAddress(ip),spinbox->value());
}

void TcpFileSender::startTransfer(){
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly)){
        QMessageBox::warning(this,QStringLiteral("신청서"),QStringLiteral("읽을 수 없습니다. %1\n%2.").arg(fileName).arg(localFile->errorString()));
        return;
    }
    totalBytes = localFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6); //데이터 형식 버전 번호 설정
    QString currentFile = fileName.right(fileName.size()-fileName.lastIndexOf("/")-1);
    sendOut<<qint64(0)<<qint64(0)<<currentFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0); //출력 표시기가 헤드 엔드로 이동했습니다.
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    bytesToWrite = totalBytes - tcpClient.write(outBlock);
    clientStatusLabel->setText(QStringLiteral("연결됨"));
    outBlock.resize(0);//빈 outBlock

}

void TcpFileSender::updateClientProgress(qint64 a){
   bytesWritten += (int) a;
   if(bytesToWrite>0){
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));
       bytesToWrite -= (int) tcpClient.write(outBlock);
       outBlock.resize(0);
   }
   else{
       localFile->close();
   }
   clientProgressBar->setMaximum(totalBytes);
   clientProgressBar->setValue(bytesWritten);
   clientStatusLabel->setText(QStringLiteral("전송 된 크기 : %1 bytes").arg(bytesWritten));
}

void TcpFileSender::openFile(){
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()) startButton->setEnabled(true);
}
