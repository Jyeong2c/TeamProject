#include "widget.h"
#include "ui_widget.h"

#include <QDir>
#include <QFileInfo>

using namespace std;
/*내일은 지정된 서버에서 파일을 보내는 방법을 강구 해봐함*/
/*서버에서 JSON데이터를 사용하는데 ui로 데이터를 생성해보는 연구 필요*/

static inline QByteArray IntToArray(qint32 source); //정수형을 문자열로 바꾸는 인라인 함수

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->UrlLineEdit->text();

    /*소켓 사용 할당 변수*/
    socket = new QTcpSocket(this);
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

    /*파싱 데이터 전송을 위한 IP에 연결*/
    fd_flag = connectToHost("192.168.0.48");       //localhost

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");


    /*JsonParsingTable 초기화 이후 ID, Name, Body 헤더 표시*/
    ui->JsonPasingTable->clear();
    QTreeWidgetItem* header = new QTreeWidgetItem;

    /*ID, Name, Gender, Age, phoneNumber, Address, date*/
    header->setText(0, "ID");
    header->setText(1, "Name");
    header->setText(2, "Gender");
    header->setText(3, "Age");
    header->setText(4, "PhoneNumber");
    header->setText(5, "Address");
    header->setText(6, "Date");

    ui->JsonPasingTable->setHeaderItem(header);

    /*TreeView column 조정 중*/
    ui->JsonPasingTable->header()->setStretchLastSection(true);
    ui->JsonPasingTable->header()->setCascadingSectionResizes(true);

    /*프로그래스 상태 및 수신 데이터 준비 상태 초기화*/
    ui->receiverProgress->setValue(0);
    totalBytes = 0;
    byteReceived = 0;
    fileNameSize = 0;

    connectWidget();

    /*connectButton 누를 시 json 데이터를 파싱*/
    connect(ui->connectButton, &QPushButton::clicked, [=]{
        //클라이언트의 폴더에 있는 html또는 json형태의 데이터만 파싱가능 http://192.168.0.48:8001/test.html(x)
        ui->UrlLineEdit->text();

        /*드라이브에 다운로드된 HTML문서를 링킹*/
        QString Link = ui->UrlLineEdit->text().toUtf8();
        ui->JsonPasingTable->clear();
        QFile file( Link );
        if( file.open( QIODevice::ReadOnly ) )
        {
            QByteArray bytes = file.readAll();
            file.close();

            ui->urlAllData_2->setPlainText(bytes);

            QJsonParseError jsonError;
            QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
            if( jsonError.error != QJsonParseError::NoError )
            {
                cout << "fromJson failed: " << jsonError.errorString().toStdString() << endl;
                return ;
            }
            if( document.isObject() )
            {
                QJsonObject jsonObj = document.object();
                QStringList idnamebody;
                idnamebody << "P1" << "P2" << "P3" << "P4";
                for( auto idnamebody: idnamebody )
                {
                    if( jsonObj.contains( idnamebody ) )
                    {
                        QJsonObject obj = jsonObj.value( idnamebody ).toObject();
#if 0
                        QStringList keys = obj.keys();
                        for( auto key: keys )
                        {
                            auto value = obj.take( key );
                            if( value.isDouble() )
                            {
                                qDebug() << key << " : " << value.toDouble();
                            }
                            else if( value.isString() )
                            {
                                qDebug() << key << " : " << value.toString();
                            }

                        }
#else
//                        person1.insert("id", obj1.id);
//                        person1.insert("name", obj1.name);
//                        person1.insert("gender", obj1.gender);
//                        person1.insert("age", obj1.age);
//                        person1.insert("phoneNumber", obj1.phoneNumber);
//                        person1.insert("address", obj1.address);
//                        person1.insert("date", obj1.date);


                        /*ID, Name, Gender, Age, phoneNumber, Address, date*/
                        qDebug() << "ID:" << obj["id"].toInt();
                        qDebug( ) << "Name:" << obj["name"].toString( );
                        qDebug( ) << "Gender:" << obj["gender"].toString( );
                        qDebug( ) << "Age:" << obj["age"].toString( );
                        qDebug( ) << "phoneNumber:" << obj["phoneNumber"].toString( );
                        qDebug( ) << "Address:" << obj["address"].toString( );
                        qDebug( ) << "date:" << obj["date"].toString( );

                        int id = obj["id"].toInt();
                        QString name = obj["name"].toString( );
                        QString gender = obj["gender"].toString( );
                        QString age = obj["age"].toString( );
                        QString phone = obj["phoneNumber"].toString( );
                        QString address = obj["address"].toString( );
                        QString date = obj["date"].toString( );

                        //TreeWidgetItem Header

                        QTreeWidgetItem* subItem = new QTreeWidgetItem;
                        subItem->setText(0, QString::number(id));
                        subItem->setText(1, name);
                        subItem->setText(2, gender);
                        subItem->setText(3, age);
                        subItem->setText(4, phone);
                        subItem->setText(5, address);
                        subItem->setText(6, date);
                        ui->JsonPasingTable->addTopLevelItem(subItem);
#endif
                    }
                    else{      //접속에 문제가 있는 경우
                        qDebug() << "Failure";
                        QMessageBox box;
                        box.setIcon(QMessageBox::Warning);
                        box.setWindowTitle("Warning Message");
                        box.setText("No search This Page");
                        box.exec();
                    }
                }
            }
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectWidget(){
    connect(ui->standByReceiveButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(&fileReceiver, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

bool Widget::connectToHost(QString host)
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    socket->connectToHost(host, PORT_NUMBER);       //id address, port
    return socket->waitForConnected();

}

bool Widget::writeData(QByteArray data)
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(IntToArray(data.size()));
        socket->write(data);
        return socket->waitForBytesWritten();
    }else{
        return false;
    }
}

/*파싱된 Json treeView에 있는 선택된 데이터 호출*/
void Widget::on_SelectSend_clicked()
{
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

    QList<QTreeWidgetItem*> selectedItemList = ui->JsonPasingTable->selectedItems();

    /*트리뷰위젯의 아이템을 선택하지 않고 버튼을 눌렀을때의 예외처리*/
    if(selectedItemList.isEmpty())
    {
        return;
    }

    if(selectedItemList.length() == 0)      //아이템이 선택되지 않았으면
    {
        ui->JsonPasingTable->topLevelItem(ui->JsonPasingTable->topLevelItemCount() - 1)->setSelected(true);
    }
    qDebug() << ui->JsonPasingTable->currentItem()->data(1,0);
    qDebug() << ui->JsonPasingTable->currentItem()->data(3,0);
    qDebug() << ui->JsonPasingTable->currentItem()->data(5,0);

    /*ID, Name, Gender, Age, phoneNumber, Address, date*/
    QString id = ui->JsonPasingTable->currentItem()->data(0,0).toString();
    QString name = ui->JsonPasingTable->currentItem()->data(1,0).toString();
    QString gender = ui->JsonPasingTable->currentItem()->data(2,0).toString();
    QString age = ui->JsonPasingTable->currentItem()->data(3,0).toString();
    QString phone = ui->JsonPasingTable->currentItem()->data(4,0).toString();
    QString address = ui->JsonPasingTable->currentItem()->data(5,0).toString();
    QString date = ui->JsonPasingTable->currentItem()->data(6,0).toString();

    if(fd_flag)
    {
        //        QString status = QString("Processing file %1 of %2: %3")
        //                        .arg(i).arg(total).arg(fileName);@
        QString textData, sendData;
        textData = QString("Button click : %1, %2, %3, %4, %5, %6, %7, %8\n")
                .arg(cnt).arg(id).arg(name).arg(gender).arg(age).arg(phone).arg(address).arg(date);
        sendData = QString("Socket data : %1, %2, %3, %4, %5, %6, %7, %8\n")
                .arg(cnt).arg(id).arg(name).arg(gender).arg(age).arg(phone).arg(address).arg(date);
        //textData = QString("(textData) ID Data : %1\n").arg(index);
        //sendData = QString("(sendData) ID Data : %1\n").arg(index);

        /*자신의 UI textBrowser에서는 textData가 보내지는 데이터는 sendData가 보내짐*/
        ui->textBrowser->insertPlainText(textData);
        send_flag = writeData(sendData.toStdString().c_str());

        if(!send_flag)
            ui->textBrowser->insertPlainText("Socket send fail\n");
        else
            cnt++;
    }
}


/*정수를 배열로 바꾸는 함수 구현부*/
QByteArray IntToArray(qint32 source)    //숫자가 4바이트인지를 확인하기 위해 qint32(signed int)자료형을 사용
{
    /*캐스트 사용을 피하고, 객체를 직렬화 하는 Qt방식*/
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void Widget::start(){                   // 파일 수신 준비 슬롯
    ui->standByReceiveButton->setEnabled(false);
    byteReceived = 0;
    fileNameSize = 0;
    fileReceiver.listen(QHostAddress::AnyIPv4, 8100);
    ui->recevierStatusLabel->setText(QStringLiteral("stand by receiving...."));
}

void Widget::acceptConnection(){        // 파일 전송 클라이언트 연결 슬롯
    /* nextPendingConnection :
      보류 중인 다음 연결을 연결된 QTcpSocket 개체로 반환합니다.
      소켓은 서버의 자식으로 생성됩니다.
      즉, QTcpServer 개체가 제거되면 소켓이 자동으로 삭제됩니다.
      메모리 낭비를 방지하기 위해 개체 작업을 완료한 후에는 개체를
      명시적으로 삭제하는 것이 좋습니다.*/
    tcpServerConnection = fileReceiver.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(upDateServerProagress()));
    ui->recevierStatusLabel->setText(QStringLiteral("receive Connect!!"));
    fileReceiver.close();
}

void Widget::upDateServerProagress(){   // 파일 상태 최신화 슬롯
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);

    /*받는 파일 용량을 검사 하고 그 상태를 프로그래스 바와 status 텍스트에 출력*/
    if(byteReceived <= sizeof(qint64) * 2){
        /*bytesAvailable: QIODevice:: bytesAvailable() const. 읽기 대기 중인 수신 바이트 수를 반환합니다.*/
        /*파일 중간에 수신받는 과정(흐름)*/
        if(fileNameSize == 0 && tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2){
            in >> totalBytes >> fileNameSize;
            byteReceived += sizeof(qint64) * 2;
        }
        /*파일 수신 완료 과정*/
        if(fileNameSize != 0 && tcpServerConnection->bytesAvailable() >= fileNameSize){
            in >> fileName;
            byteReceived += fileNameSize;
            localFile = new QFile(fileName);
            /*만일 해당 파일에 대한 정보가 없을 경우 에러메세지 호출*/
            if(!localFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this, QStringLiteral("Server"),
                                     QStringLiteral("Can't open this file")
                                     .arg(fileName).arg(localFile->errorString()));
                return;
            }
        }
        else return;
    }

    /*수신받는 데이터 스트림 과정이 전체데이터 보다 작은 경우*/
    if(byteReceived < totalBytes){
        /*수신받는 데이터에 서버로 팬딩된 데이터로 전송*/
        byteReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    ui->receiverProgress->setMaximum(totalBytes);
    ui->receiverProgress->setValue(byteReceived);
    /*수신완료된 데이터를 byte로 표시*/
    ui->recevierStatusLabel->setText(QStringLiteral("receive Data(%1) bytes")
                                     .arg(byteReceived));

    /*수신데이터 완료*/
    if(byteReceived == totalBytes){
        tcpServerConnection->close();               //커넥션 닫기
        ui->standByReceiveButton->setEnabled(true); //송신 버튼 활성화
        localFile->close();                         //로컬파일 닫기
        start();//update와 같은 의미
    }
}

void Widget::on_findFilebutton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    /*파일을 준비하면 전송 버튼 활성화*/
    if(!fileName.isEmpty()) ui->UrlLineEdit->setText(fileName);
}

