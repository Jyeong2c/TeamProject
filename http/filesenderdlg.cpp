#include "filesenderdlg.h"
#include "ui_filesenderdlg.h"

FileSenderDlg::FileSenderDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSenderDlg)
{
    ui->setupUi(this);
#if 1
    /*파일 전송 진행 변수 초기화*/
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;

    /*하나의 IP주소의 포트번호는 65535까지 설정*/
    ui->portSpinBox->setMaximum(65535);

    /*파일 선택이 안되어 있으므로 파일 전송버튼은 Enable(false) = disable 상태로 설정*/
    ui->fileTransferButton->setEnabled(false);

    /*프로그래스바 초기 상태를 0으로 지정*/
    ui->fileSenderProgress->setValue(0);

    /*슬롯 함수 호출*/
    connectDlg();
#else
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
#endif
}

FileSenderDlg::~FileSenderDlg()
{
    delete ui;
}

void FileSenderDlg::connectDlg(){                               //connect() 함수의 모음
    /*파일 전송 버튼 클릭 시 start() 슬롯과 커넥트*/
    connect(ui->fileTransferButton, SIGNAL(clicked()), this, SLOT(start()));

    /*파일 열기 버튼 클릭시 openFile() 슬롯과 커넥트*/
    connect(ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));

    /*TCP 소켓은 Qt의 startTransfer()를 커넥트(파일 옮기기 상태)*/
    connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));

    /*Tcp 소켓은 qint64형태 바이트로 전송상태를 커넥트*/
    connect(&tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
}

QString ip;

void FileSenderDlg::start(){                                    // 파일 보내기 시작 슬롯 함수
#if 1
    /*서버를 찾기 전 서버의 IP주소 찾기 과정*/
    IP = ui->ipLineEdit->text();
    ui->fileTransferButton->setEnabled(false);
    bytesWritten = 0;

    /*QStringLiteral을 사용하면 런타임이 아닌, 컴파일 타임에 QString을 생성한다*/
    ui->clientStatusLable->setText(QStringLiteral("Finding Server....."));

    /*서버의 IP와 포트번호를 찾으면 연결*/
    tcpClient.connectToHost(QHostAddress(IP), ui->portSpinBox->value());
#else
    ip = lineEdit1->text();
    startButton->setEnabled(false);
    bytesWritten = 0;
    clientStatusLabel->setText(QStringLiteral("서버 찾는중......"));
    tcpClient.connectToHost(QHostAddress(ip),spinbox->value());
#endif
}

void FileSenderDlg::startTransfer(){                            // 파일 문자 변환 시작 함수
#if 1
    localFile = new QFile(fileName);

    /*파일을 열 수 없으면 경고 메시지를 부여*/
    if(!localFile->open(QFile::ReadOnly)){
        QMessageBox::warning(this, QStringLiteral("warning"),
                             QStringLiteral("Can't Read this file %1\n%2.")
                             .arg(fileName).arg(localFile->errorString()));
        return;
    }

    /*데이터 전체 크기는 지정된 파일의 사이즈 만큼 할당*/
    totalBytes = localFile->size();
    /*데이터 스트림은 쓰기로만 설정*/
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    /*데이터 형식 버전 번호 설정*/
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf("/")-1);
    sendOut << qint64(0) << qint64(0) << currentFile;
    totalBytes += outBlock.size();

    /*출력 표시기가 헤드 엔드로 이동*/
    sendOut.device()->seek(0);
    sendOut << totalBytes << qint64((outBlock.size()-sizeof(qint64)*2));
    bytesToWrite = totalBytes - tcpClient.write(outBlock);
    ui->clientStatusLable->setText(QStringLiteral("connect!!"));
    /*outBlock 0으로 초기화*/
    outBlock.resize(0);

#else
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
    sendOut.device()->seek(0);

    //출력 표시기가 헤드 엔드로 이동했습니다.
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    bytesToWrite = totalBytes - tcpClient.write(outBlock);
    clientStatusLabel->setText(QStringLiteral("연결됨"));
    outBlock.resize(0);//빈 outBlock
#endif
}

void FileSenderDlg::updateClientProgress(qint64 data){               // 파일 보내는 프로그래스 과정 업데이트 슬롯 함수
#if 1
    bytesWritten += (int)data;
    if(bytesToWrite > 0){
        outBlock = localFile->read(qMin(bytesToWrite, loadSize));
        bytesToWrite -= (int) tcpClient.write(outBlock);
        outBlock.resize(0);
    }
    else{
        localFile->close();
    }

    /*파일 전송 과정을 업데이트하고 결과를 알려줌*/
    ui->fileSenderProgress->setMaximum(totalBytes);
    ui->fileSenderProgress->setValue(bytesWritten);
    ui->clientStatusLable->setText(QStringLiteral("sended file size : %1 bytes").arg(bytesWritten));

#else
    bytesWritten += (int) data;
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
#endif
}

void FileSenderDlg::openFile(){                                 // 파일 열기 슬롯 함수
#if 1
    /*파일 다이얼로그를 불러서 보내는 파일을 찾고 전송 준비*/
    fileName = QFileDialog::getOpenFileName(this);

    /*파일을 준비하면 전송 버튼 활성화*/
    if(!fileName.isEmpty()) ui->fileTransferButton->setEnabled(true);
#else
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()) startButton->setEnabled(true);
#endif
}
