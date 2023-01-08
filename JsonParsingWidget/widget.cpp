#include "widget.h"
#include "ui_widget.h"

using namespace std;
/*내일은 지정된 서버에서 파일을 보내는 방법을 강구 해봐함*/
/*서버에서 JSON데이터를 사용하는데 ui로 데이터를 생성해보는 연구 필요*/

static inline QByteArray IntToArray(qint32 source); //정수형을 문자열로 바꾸는 인라인 함수

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
<<<<<<< Updated upstream
    //클라이언트의 폴더에 있는 html또는 json형태의 데이터만 파싱가능 http://192.168.0.48:8001/test.html(x)
    ui->UrlLineEdit->setText(QString("D:/GitDesktop/TeamProject/JsonHttp/test.html"));
=======
    ui->UrlLineEdit->setText(QString("C:/QtGit/TeamProject/JsonHttp/test.html"));
>>>>>>> Stashed changes

    /*D 드라이브에 다운로드된 HTML문서를 링킹*/
    QString Link = ui->UrlLineEdit->text().toUtf8();

    /*소켓 사용 할당 변수*/
    socket = new QTcpSocket(this);
    qDebug("[%s] %s : %d", __FILE__, __FUNCTION__, __LINE__);

    /*해당 IP에 연결*/
    fd_flag = connectToHost("192.168.0.48");       //localhost

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");


    /*ID, NAME, BODY데이터를 보내는 버튼 커넥트 함수*/
//    connect(ui->IDButton, SIGNAL(clicked()), this, SLOT(on_IDButton_clicked()));
//    connect(ui->NameButton, SIGNAL(clicked()), this, SLOT(on_NameButton_clicked()));
//    connect(ui->BodyButton, SIGNAL(clicked()), this, SLOT(on_BodyButton_clicked()));



    /*JsonParsingTable 초기화 이후 ID, Name, Body 헤더 표시*/
    ui->JsonPasingTable->clear();
    QTreeWidgetItem* header = new QTreeWidgetItem;

    header->setText(0, "ID");
    header->setText(1, "Name");
    header->setText(2, "Body");

    ui->JsonPasingTable->setHeaderItem(header);

    /*TreeView column 조정 중*/
    ui->JsonPasingTable->header()->setStretchLastSection(true);
    ui->JsonPasingTable->header()->setCascadingSectionResizes(true);

    /*connectButton 누를 시 json 데이터를 파싱*/
    connect(ui->connectButton, &QPushButton::clicked, [=]{
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
                        qDebug() << "ID:" << obj["id"].toInt();
                        qDebug( ) << "Name:" << obj["name"].toString( );
                        qDebug( ) << "Body:" << obj["body"].toString( );

                        int id = obj["id"].toInt();
                        QString name = obj["name"].toString( );
                        QString body = obj["body"].toString( );

                        //TreeWidgetItem Header

                        QTreeWidgetItem* subItem = new QTreeWidgetItem;
                        subItem->setText(0, QString::number(id));
                        subItem->setText(1, name);
                        subItem->setText(2, body);
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
    qDebug() << ui->JsonPasingTable->currentItem()->data(0,0);
    qDebug() << ui->JsonPasingTable->currentItem()->data(1,0);
    qDebug() << ui->JsonPasingTable->currentItem()->data(2,0);

    QString ID = ui->JsonPasingTable->currentItem()->data(0,0).toString();
    QString Name = ui->JsonPasingTable->currentItem()->data(1,0).toString();
    QString Body = ui->JsonPasingTable->currentItem()->data(2,0).toString();

    if(fd_flag)
    {
//        QString status = QString("Processing file %1 of %2: %3")
//                        .arg(i).arg(total).arg(fileName);@
        QString textData, sendData;
        textData = QString("Button click : %1, %2, %3, %4\n").arg(cnt).arg(ID).arg(Name).arg(Body);
        sendData = QString("Socket data : %1, %2, %3, %4\n").arg(cnt).arg(ID).arg(Name).arg(Body);
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




