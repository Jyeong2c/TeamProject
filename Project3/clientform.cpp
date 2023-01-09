#include "clientform.h"
#include "ui_clientform.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QDateTime>
#include <QMenu>
#include <QSqlRelationalTableModel>

ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);

    QAction* removeAction = new QAction(tr("&Remove"));              //삭제 액션 생성
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));  //triggered시 삭제

    menu = new QMenu;                                                //메뉴 생성
    menu->addAction(removeAction);                                   //삭제 메뉴 추가

    //시그널이 발생하면 showContextMenu 호출
    ui->clientTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->clientTableView, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));
    //엔터키를 눌리면 searchPushButton 이 호출
    //    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

}

ClientForm::~ClientForm()
{
    delete ui;
    QSqlDatabase clDB = QSqlDatabase::database("clientDB");  //데이터베이스 닫기
    if(clDB.isOpen()){                                               //만약 파일이 열리면
        queryModel->submitAll();
        delete queryModel;
        clDB.close();
        //        QSqlDatabase::removeDatabase("client");
    }
}


void ClientForm::loadData()                                   //파일 저장 함수
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase clDB = QSqlDatabase::addDatabase("QSQLITE","clientDB");
    clDB.setDatabaseName("clientDB.db");                             //데이터베이스 이름설정

    if(clDB.open()){                                                 //조건문
        query = new QSqlQuery(clDB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
        query->exec("CREATE TABLE IF NOT EXISTS client(id INTEGER Primary Key,"
                    "name VARCHAR(20) NOT NULL,gender VARCHAR(20), age VARCHAR(20),"
                    "phonenumber VARCHAR(20),address VARCHAR(20), date VARCHAR(20));");

        queryModel = new QSqlTableModel(this, clDB);
        queryModel->setTable("client");
        queryModel->select();

        /*테이블 헤더 설정*/
        queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
        queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Gender"));
        queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
        queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("PhoneNumber"));
        queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Address"));
        queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("date"));

        ui->clientTableView->setModel(queryModel);

        if(clDB.open()){                                                 //조건문
            query = new QSqlQuery(clDB);
            //query 문을 이용하여 테이블 생성 및 PK 키 설정
            query->exec("CREATE TABLE IF NOT EXISTS image(id INTEGER Primary Key,"
                        "name VARCHAR(20) NOT NULL,gender VARCHAR(20), age VARCHAR(20),"
                        "imageType VARCHAR(20),imagePath VARCHAR(20), date VARCHAR(20));");

            queryModel = new QSqlTableModel(this, clDB);
            queryModel->setTable("image");
            queryModel->select();


            modelMain = new QSqlRelationalTableModel(this);
            modelMain->setTable("imageTable");
            modelMain->setRelation(5, QSqlRelation("imageType", "id", "name"));

            /*테이블 헤더 설정*/
            queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
            queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
            queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Gender"));
            queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
            queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("imageType"));
            queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("imagePath"));
            queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("date"));

            createRelationalTable();
            ui->imageTableView->setModel(queryModel);


//            modelMain->setRelation(4, QSqlRelation(imageDB, "id", DEVICE\_IP));

        }

    }
}

void ClientForm::createRelationalTable()
{
//    QSqlQuery query;
    query->exec("CREATE TABLE IF NOT EXISTS image(id INTEGER Primary Key,"
                "name VARCHAR(20) NOT NULL,gender VARCHAR(20), age VARCHAR(20),"
                "imageType VARCHAR(20),imagePath VARCHAR(20), date VARCHAR(20));");


    query->exec("insert into image values(1, '조재영', 'male', '1997-02-02','100','abc','date')");
    query->exec("insert into image values(2, '한은지', 'female', '1997-02-02','5000','abc','date')");
//    query->exec("insert into image values(3, 'Sam', 100, 1)");

    query->exec("create table imageType(id varchar(20), name varchar(20))");
    query->exec("insert into imageType values('100', 'CT')");
    query->exec("insert into imageType values('5000', 'Scaner')");
    query->exec("insert into imageType values('80000', 'X-ray')");

//    query->exec("create table country(id int, name varchar(20))");
//    query->exec("insert into country values(1, 'USA')");
//    query->exec("insert into country values(47, 'Norway')");
//    query->exec("insert into country values(49, 'Germany')");
}
void ClientForm::showContextMenu(const QPoint &pos)           //마우스 위치 설정
{
    QPoint globalPos = ui->clientTableView->mapToGlobal(pos);        //TableView에 마우스 설정
    menu->exec(globalPos);
}


int ClientForm::makeId( )                                     //id 값 설정
{
    if(queryModel->rowCount() == 0) {                                // 값이 없으면 client id는 1부터 시작하게 return 값을 설정
        return 1;
    } else {
        auto id = queryModel->data(queryModel->index(queryModel->rowCount()-1,0)).toInt();
        return ++id;                                                 //이전 값에 ++id 된 값으로 증가
    }
}


void ClientForm::on_addPushButton_clicked()
{
    QString name, number, address,gender,age,date;
    int id = makeId( );                                              //설정한 id값을 id에 대입
    name = ui->nameLineEdit->text();
    gender = ui->genderComboBox->currentText();
    age = ui->ageLineEdit->text();
    number = ui->phoneNumberLineEdit->text();                        //입력한 값을 number에 대입
    address = ui->addressLineEdit->text();                           //입력한 값을 address에 대입
    date = date.append(QDateTime::currentDateTime().toString());

    QSqlDatabase clDB = QSqlDatabase::database("clientDB");  //어떤 DB파일인지 알려준다
    if(clDB.isOpen() &&name.length()) {

        query->exec(QString("INSERT INTO client VALUES (%1, '%2', '%3', '%4','%5','%6','%7')")
                    .arg(id).arg(name).arg(gender).arg(age).arg(number).arg(address).arg(date));

        queryModel->select();

        ui->clientTableView->resizeColumnsToContents();
        clearText();
    }
}

void ClientForm::on_modifyPushButton_clicked()
{

    QModelIndex index = ui->clientTableView ->currentIndex();
    if(index.isValid()){
        QString name, number, address,gender,age;
        int id;
        id = ui->idLineEdit->text().toInt();
        name = ui->nameLineEdit->text();                             //LineEdit에 있는 정보를 name에 대입
        gender = ui->genderComboBox->currentText();
        age = ui->ageLineEdit->text();
        number = ui->phoneNumberLineEdit->text();                    //LineEdit에 있는 정보를 number에 대입
        address = ui->addressLineEdit->text();                       //LineEdit에 있는 정보를 address에 대입

        query->prepare("UPDATE client SET name = ?, gender =?, age = ?, phonenumber = ?, address = ? WHERE id =?;");
        query->bindValue(5,id);
        query->bindValue(0,name);
        query->bindValue(1,gender);
        query->bindValue(2,age);
        query->bindValue(3,number);
        query->bindValue(4,address);

        query->exec();
        queryModel->select();
        ui->clientTableView->resizeColumnsToContents();              //clientTableView의 사이즈를 설정
        clearText();
    }
}

void ClientForm::removeItem()                                 //정보 삭제
{

    QModelIndex index = ui->clientTableView->currentIndex();
    int id =index.sibling(index.row(),0).data().toInt();
    if(index.isValid()){

        query->prepare("DELETE FROM client WHERE id = ?;");
        query->addBindValue(id);
        query->exec();
        queryModel->select();
        ui->clientTableView->resizeColumnsToContents();
    }
}

//void ClientForm::on_enterPushButton_clicked()
//{
//    ui->imageTableView->clear();                                   //treeWidget을 정리
//    ui->clientTableView->clear();
//    int i = ui->searchComboBox->currentIndex();                      //찾을 항목을 comboBox로 보여줌
//    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
//                   : Qt::MatchCaseSensitive;
//    QModelIndexList clients = queryModel->match(queryModel->index(0,i),
//                                                Qt::EditRole, ui->searchLineEdit->text(),-1,Qt::MatchFlags(flag));

//    foreach(auto ix,clients) {
//        int id =ix.sibling(ix.row(),0).data().toInt();
//        QString name = ix.sibling(ix.row(),1).data().toString();
//        QString  =ix.sibling(ix.row(),2).data().toString();
//        QString address = ix.sibling(ix.row(),3).data().toString();
//        QStringList clientStrings;
//        clientStrings << QString::number(id)<<name<<number<<address;
//        new QTreeWidgetItem(ui->searchTreeWidget,clientStrings);
//        for(int i = 0; i<ui->clientTableView->columnCount(); i++)
//            ui->searchTreeWidget->resizeColumnToContents(i);
//    }

//}


/*clientTableView에 있는 정보를 클릭할 때*/
void ClientForm::on_clientTableView_clicked(const QModelIndex &index)
{
    QModelIndex ix = ui->clientTableView->currentIndex();
    QString name,gender,age,phoneNumber,address;
    int id;
    id = ix.sibling(ix.row(),0).data().toInt();
    name = ix.sibling(ix.row(),1).data().toString();
    gender = ix.sibling(ix.row(),2).data().toString();
    age = ix.sibling(ix.row(),3).data().toString();
    phoneNumber = ix.sibling(ix.row(),4).data().toString();
    address =ix.sibling(ix.row(),5).data().toString();

    ui->idLineEdit->setText(QString::number(id));
    ui->nameLineEdit->setText(name);                                 //선택한 정보를 lineEdit에 보여줍니다
    ui->genderComboBox->setCurrentText(gender);
    ui->ageLineEdit->setText(age);
    //    ui->ageDateEdit->sectionText(age);
    //    ui->ageDateEdit->setDate(QDate::fromString(age));
    ui->phoneNumberLineEdit->setText(phoneNumber);                   //선택한 정보를 lineEdit에 보여줍니다
    ui->addressLineEdit->setText(address);                           //선택한 정보를 lineEdit에 보여줍니다
}

void ClientForm::clearText()
{
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();                             //LineEdit에 있는 정보를 name에 대입
    ui->genderComboBox->clearEditText();
    ui->ageLineEdit->clear();
    ui->phoneNumberLineEdit->clear();                   //LineEdit에 있는 정보를 number에 대입
    ui->addressLineEdit->clear();                  //LineEdit에 있는 정보를 address에 대입
}
