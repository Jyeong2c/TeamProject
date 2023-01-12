#include "imagemanagementform.h"
#include "ui_imagemanagementform.h"

#include <QFileDialog>
#include <QGraphicsView>
#include <QPixmap>

#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlTableModel>

/*서버를 연동하기 위한 Qt 헤더*/
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "imageview.h"

ImageManagementForm::ImageManagementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageManagementForm)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 150 << 1000;
    ui->splitter2->setSizes(sizes);

    QList<int> listsize;
    listsize << 1000 << 220;
    ui->splitter->setSizes(listsize);

    //connect(ui->imageRoadButton, SIGNAL(clicked()), this, SLOT(reloadImages()));
    connect(ui->imageRoadButton, SIGNAL(clicked()), this, SLOT(fileLoad()));
    connect(ui->zoomInToolButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui->zoomOutToolButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(ui->leftRotateToolButton, SIGNAL(clicked()), this, SLOT(leftRotate()));
    connect(ui->rightRotateToolButton, SIGNAL(clicked()), this, SLOT(rightRotate()));

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));



//    graphicsScene = new QGraphicsScene(this);
//    imageView = new ImageView(ui->listWidget);
////    imageView->setScene(graphicsScene);

//    ui->listWidget->setIconSize(QSize(100, 100));
//    ui->listWidget->setFlow(QListWidget::LeftToRight);

//    reloadImages();


    graphicsScene = new QGraphicsScene;
    imageView = new ImageView();
//    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    imageView->setScene(graphicsScene);
//    imageView->grabGesture(Qt::PinchGesture);

    listWidget = new QListWidget(ui->listWidget);
    listWidget->setIconSize(QSize(100, 100));
    listWidget->setResizeMode(QListWidget::Adjust);
    listWidget->setFlow(QListWidget::LeftToRight);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));

    reloadImages();
}

/*데이터 베이스 서버 연동*/
void ImageManagementForm::loadData()                                   //파일 저장 함수
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("paientDB.db");                             //데이터베이스 이름설정

    if(DB.open()){                                                 //조건문
        paitentQuery = new QSqlQuery(DB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
        paitentQuery->exec("CREATE TABLE IF NOT EXISTS paient(chartNum INTEGER Primary Key,"
                    "name VARCHAR(20) NOT NULL, age VARCHAR(20),imagePath VARCHAR(20));");

        paitentQueryModel = new QSqlTableModel(this, DB);
        paitentQueryModel->setTable("paient");
        paitentQueryModel->select();

        /*테이블 헤더 설정*/
        paitentQueryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ChartNum"));
        paitentQueryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        paitentQueryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
        paitentQueryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("ImagePath"));

        ui->paitentTableview->setModel(paitentQueryModel);

        /*이미지 경로는 숨기기*/
        ui->paitentTableview->hideColumn(3);


        /*요청한 HTTP 경로에 JSON 데이터 파싱*/

        /*스택 위의 임시 이벤트 루프(event loop)*/
        QEventLoop eventLoop;

        /*"finished()"가 불려지면 이벤트 루프를 종료*/
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)),
                         &eventLoop, SLOT(quit()));

        /*HTTP 요청(1-2)*/
        for(int patientNum = 1; patientNum <= 2; patientNum++){
            QNetworkRequest req(QUrl(QString("http://127.0.0.1:3000/patient/%1/").arg(patientNum)));
            QNetworkReply *reply = mgr.get(req);
            eventLoop.exec();       //"finished()" 가 호출 될 때까지 블록

            /*요청에 이상이 없는 경우*/
            if(reply->error() == QNetworkReply::NoError){
                QString strReply = (QString)reply->readAll();

                //JSON 파싱
                qDebug() << "Response:" << strReply;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
                QJsonObject jsonObj = jsonResponse.object();

                qDebug() << "id : " << jsonObj["id"].toInt();
                qDebug() << "name : " << jsonObj["name"].toString();
                qDebug() << "age : " << jsonObj["age"].toInt();
                qDebug() << "localimage : " << jsonObj["localimage"].toString();
                qDebug() << "Failure" << reply->errorString();
                paitentQuery->exec(QString::fromStdString("INSERT INTO paient VALUES (%1,'%2','%3','%4')")
                                   .arg(jsonObj["id"].toInt()).arg(jsonObj["name"].toString())
                                   .arg(jsonObj["age"].toInt()).arg(jsonObj["localimage"].toString()));
                delete reply;
            }else{  //오류시
                qDebug() << "Failure" << reply->errorString();
                delete reply;
            }
        }


        paitentQuery->exec("INSERT INTO paient VALUES (1000,'JaeYeong','28','..')");
        paitentQuery->exec("INSERT INTO paient VALUES (1001,'Yuna','26','..')");
        paitentQuery->exec("INSERT INTO paient VALUES (1002,'Jaehyun','27','..')");
        paitentQuery->exec("INSERT INTO paient VALUES (1003,'은지','29','..')");
        paitentQueryModel->select();
        ui->paitentTableview->resizeColumnsToContents();

    }
}





ImageManagementForm::~ImageManagementForm()
{
    delete ui;
}

void ImageManagementForm::reloadImages(){
    QDir dir(".");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    graphicsScene->clear();

    listWidget->clear();
    for(int i=0; i<fileInfoList.count(); i++){
        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, listWidget);
        item->setStatusTip(fileInfoList.at(i).fileName());
        listWidget->addItem(item);
    };
}

void ImageManagementForm::selectItem(QListWidgetItem *item){
    imageView->resetTransform();
    graphicsScene->clear();
    graphicsScene->addPixmap(QPixmap(item->statusTip()).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->graphicsView1->setScene(graphicsScene);
}

void ImageManagementForm::setImage(QString path){
    imageView->resetTransform();
    graphicsScene->clear();
    graphicsScene->addPixmap(QPixmap(path).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QFileInfo fileInfo(path);
    for(int i=0; i< ui->listWidget->count(); i++){
        if(ui->listWidget->item(i)->statusTip() == fileInfo.fileName()){
            ui->listWidget->setCurrentRow(i);
            break;
        }
    }
}

QString ImageManagementForm::currentImage(){
    return (ui->listWidget->currentRow() >= 0)? ui->listWidget->currentItem()->statusTip():"";
}

void ImageManagementForm::reset(){
    imageView->resetTransform();
}


//--------------------------------------------------------------------------------------

void ImageManagementForm::fileLoad()
{
    QFileDialog dlg;
    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");

    QImage *imgSize = new QImage(filePath);
    QPixmap buf = QPixmap::fromImage(*imgSize);
    buf = buf.scaled(imgSize->width(), imgSize->height());

    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(buf);
    ui->graphicsView1->setScene(scene);

    //ui->graphicsView2->setScene(scene);
}

void ImageManagementForm::zoomIn()
{
    ui->graphicsView1->scale(1.25,1.25);
}

void ImageManagementForm::zoomOut()
{
    ui->graphicsView1->scale(0.8, 0.8);
}

void ImageManagementForm::leftRotate()
{
    ui->graphicsView1->rotate(90);
}

void ImageManagementForm::rightRotate()
{
    ui->graphicsView1->rotate(-90);
}

