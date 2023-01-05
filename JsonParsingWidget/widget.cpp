#include "widget.h"
#include "ui_widget.h"
using namespace std;
//임의의 구조체 선언
struct IdNameBody
{
    int id;
    QString name;
    QString body;
    IdNameBody(int _id, QString _name, QString _body)
    {
        id = _id;
        name = _name;
        body = _body;
    }
};

//void CreateJson(const QString &path);
//void ReadJson(const QString &path);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->UrlLineEdit->setText(QString("D:/HTTPQt/Htmlzip/test.html"));

//    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, [=]{
//        int val = ui->horizontalSlider->value();
//        ui->urlAllData->setText(QString::number(val));
//    });

//    connect(ui->urlAllData, &QLineEdit::textChanged, this, [=]{
//        int val = ui->urlAllData->text().toInt();
//        ui->horizontalSlider->setValue(val);
//    });

    QString Link = ui->UrlLineEdit->text().toUtf8();


    //JsonParsingTable 초기화 이후 ID, Name, Body 헤더 표시
    ui->JsonPasingTable->clear();
    QTreeWidgetItem* header = new QTreeWidgetItem;

    header->setText(0, "ID");
    header->setText(1, "Name");
    header->setText(2, "Body");

    ui->JsonPasingTable->setHeaderItem(header);

    //테이블 컬럼 자동 조정

    ui->JsonPasingTable->header()->setStretchLastSection(true);
    ui->JsonPasingTable->header()->setCascadingSectionResizes(true);

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


