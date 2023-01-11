#include "clientform.h"
#include "ui_clientform.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QDateTime>


ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);

    //ui->tabWidget->setTabPosition(QTabWidget::South);
//    ui->tabWidget->maximumSize();



}

ClientForm::~ClientForm()
{
    delete ui;
    delete ui;                                                       //ui닫기
    QSqlDatabase clDB = QSqlDatabase::database("client");  //데이터베이스 닫기
    if(clDB.isOpen()){                                               //만약 파일이 열리면
        queryModel->submitAll();
        delete queryModel;
        clDB.close();
        QSqlDatabase::removeDatabase("client");
    }
}



void ClientForm::loadData()                                   //파일 저장 함수
{
    //QSQLITE 파일로 데이터 베이스 생성                  //데이터베이스가 2개 이상 사용으로 이름 설정
    QSqlDatabase clDB = QSqlDatabase::addDatabase("QSQLITE","client");
    clDB.setDatabaseName("clientDB.db");                             //데이터베이스 이름설정

    if(clDB.open()){                                                 //조건문
        query = new QSqlQuery(clDB);
        //query 문을 이용하여 테이블 생성 및 PK 키 설정
        query->exec("CREATE TABLE IF NOT EXISTS clientDB(id INTEGER Primary Key,"
                    "name VARCHAR(20), gender VARCHAR(20),age VARCHAR(20),date VARCHAR(20);");

        queryModel = new QSqlTableModel(this, clDB);                 //Model에 데이터를 저장
        queryModel->setTable("clientDB");
        queryModel->select();

        /*테이블 헤더 설정*/
        queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));       //0번째 인덱스 ID 설정
        queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));     //1번째 인덱스 Name
        queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Gender"));   //2번째 인덱스 phoneNumber
        queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));       //3번째 인덱스 Address
        queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));


//        ui->clientTableView->setModel(queryModel);
        ui->tableView->setModel(queryModel);
    }

//    for(int i = 0; i < queryModel->rowCount(); i++) {                          //Model의 크기만큼
//        int id = queryModel->data(queryModel->index(i, 0)).toInt();            //id 데이터
//        QString name = queryModel->data(queryModel->index(i, 1)).toString();   //name 데이터
//        emit clientAdd(id, name);                                              //시그널을 보냅니다
//    }

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

void ClientForm::on_AddPushButton_clicked()
{
    QString name, gender, date;
    int age,id;
    id = makeId( );                                              //설정한 id값을 id에 대입
    name = ui->nameLineEdit->text();                                 //입력한 값을 name에 대입
    gender = ui->genderLineEdit->text();                        //입력한 값을 number에 대입
    age = ui->ageLineEdit->text().toInt();                           //입력한 값을 address에 대입
    date = date.append(QDateTime::currentDateTime().toString());

    QSqlDatabase clDB = QSqlDatabase::database("client");  //어떤 DB파일인지 알려준다
    if(clDB.isOpen() &&name.length()) {
        //query문을 이용하여 값을 넣어준다
        query->exec(QString("INSERT INTO clientDB VALUES (%1, '%2', '%3', %4,'%5')")
                    .arg(id).arg(name).arg(gender).arg(age).arg(date));
        queryModel->select();

    }
//    emit clientAdd(id, name);                                        //시그널 발생
//    clearText();
}

//void ClientForm::showContextMenu(const QPoint &pos)           //마우스 위치 설정
//{
//    QPoint globalPos = ui->clientTableView->mapToGlobal(pos);        //TableView에 마우스 설정
//    menu->exec(globalPos);
//}
//void ClientForm::removeItem()                                 //정보 삭제
//{

//    QModelIndex ix = ui->clientTableView->currentIndex();
//    int id =ix.sibling(ix.row(),0).data().toInt();
//    int row = ix.row();
//    if(ix.isValid()){

//        query->prepare("DELETE FROM clientDB WHERE id = ?;");
//        query->addBindValue(id);
//        query->exec();
//        queryModel->select();
//        ui->clientTableView->resizeColumnsToContents();
//    }
//    clearText();                                                     //다음 입력값을 받기 위해 text에 입력했던 값을 지움
//    emit clientRemove(row);
//}
//void ClientForm::on_modifyPushButton_clicked()
//{
//    QModelIndex index = ui->clientTableView ->currentIndex();        //
//    int row = index.row();
//    if(index.isValid()){
//        QString name, number, address;
//        int id;
//        id = ui->idLineEdit->text().toInt();                         //LineEdit에 있는 정보를 id에 대입
//        name = ui->nameLineEdit->text();                             //LineEdit에 있는 정보를 name에 대입
//        number = ui->phoneNumberLineEdit->text();                    //LineEdit에 있는 정보를 number에 대입
//        address = ui->addressLineEdit->text();                       //LineEdit에 있는 정보를 address에 대입

//        //UPDATE 문을 이용하여 값을 변환 시켜준다
//        query->prepare("UPDATE clientDB SET name = ? ,number = ?,address = ?  WHERE id = ?;");
//        query->bindValue(3,id);
//        query->bindValue(0,name);
//        query->bindValue(1,number);
//        query->bindValue(2,address);
//        query->exec();
//        queryModel->select();
//        ui->clientTableView->resizeColumnsToContents();              //clientTableView의 사이즈를 설정

//        emit clientModify(id,row,name);                              //시그널 발생

//    }
//}

