#include "tcpserverdlg.h"
#include "ui_tcpserverdlg.h"

/*문자열을 배열로 바꿔주는 인라인 함수*/
static inline qint32 ArrayToInt(QByteArray source);

TCPServerDlg::TCPServerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCPServerDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP Server Dialog");

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QString socket_data;
    socket_data = QString("Listening : %1\n").arg(server->listen(QHostAddress::Any, PORT_NUMBER) ? "true" : "false");
    ui->textBrowser->insertPlainText(socket_data);
}

TCPServerDlg::~TCPServerDlg()
{
    delete ui;

}

void TCPServerDlg::newConnection()
{
    while(server->hasPendingConnections())
    {
        /*Qt에서 쓰는 통신의 읽기와 끊김 상태 변화*/
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}
void TCPServerDlg::disconnected()
{
//    /*블록이 완전히 수신될 때까지 데이터를 저장할 버퍼가 필요합니다.*/
//    QHash<QTcpSocket*, QByteArray*> buffers;
//    /*블록이 완전히 수신되었는지 확인하기 위해 크기를 저장해야 합니다.*/
//    QHash<QTcpSocket*, qint32*> sizes;
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void TCPServerDlg::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);

    /*포인터 형 주소 변환*/
    qint32 *s = sizes.value(socket);        //int *pa = 5;
    qint32 size = *s;                       //int a = *pa;
    while(socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size))   //데이터를 처리할 수 있는동안 데이터를 처리함
        {
            if(size == 0 && buffer->size() >= 4) //데이터 크기가 완전히 수신되면 전역 변수에 저장합니다.
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }

            if(size > 0 && buffer->size() >= size)  //데이터가 완전히 수신되면 전역 변수에 저장합니다.
            {
                ui->textBrowser->insertPlainText(QString(buffer->data()));
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                emit dataReceived(data);
            }
        }
    }
}

/*배열을 정수로 변환하는 함수*/
qint32 ArrayToInt(QByteArray source)       //qint32(signed int)자료형인지를 확인하기 위해 숫자가 4바이트을 사용, 클라이언트와 반대 개념
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

