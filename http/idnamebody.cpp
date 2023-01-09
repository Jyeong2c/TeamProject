#include "idnamebody.h"
#include <iostream>
#include <QDateTime>

using namespace std;

IdNameBody::IdNameBody(int _id, QString _name, QString _gender, QString _age,
                       QString _phone, QString _address, QString _date)
{/*
    int id;
    QString name;
    QString gender;
    QString age;
    QString phoneNumber;
    QString address;
    QString date;*/

    /*ID, Name, Gender, Age, phoneNumber, Address, date*/
    id = _id;
    name = _name;
    gender = _gender;
    age = _age;
    phoneNumber = _phone;
    address = _address;
    date = _date;
}

/*명시적으로 작성한 Json 인코딩 함수 (비효율적)*/
void IdNameBody::CreateJson(const QString &path)
{
    /*ID, Name, Gender, Age, phoneNumber, Address, date*/
    IdNameBody obj1(1, "jeong jae hyeon", "male", "27", "010-2464-2739",
                    "Asan", QDateTime::currentDateTime().toString());
    QJsonObject person1;
    person1.insert("id", obj1.id);
    person1.insert("name", obj1.name);
    person1.insert("gender", obj1.gender);
    person1.insert("age", obj1.age);
    person1.insert("phoneNumber", obj1.phoneNumber);
    person1.insert("address", obj1.address);
    person1.insert("date", obj1.date);

    IdNameBody obj2(2, "jeong yu na", "female", "25", "010-5675-4389",
                    "Busan", QDateTime::currentDateTime().toString());
    QJsonObject person2;
    person2.insert("id", obj2.id);
    person2.insert("name", obj2.name);
    person2.insert("gender", obj2.gender);
    person2.insert("age", obj2.age);
    person2.insert("phoneNumber", obj2.phoneNumber);
    person2.insert("address", obj2.address);
    person2.insert("date", obj2.date);

    IdNameBody obj3(3, "jo jae yeong", "male", "28", "010-4805-4333",
                    "Busan", QDateTime::currentDateTime().toString());
    QJsonObject person3;
    person3.insert("id", obj3.id);
    person3.insert("name", obj3.name);
    person3.insert("gender", obj3.gender);
    person3.insert("age", obj3.age);
    person3.insert("phoneNumber", obj3.phoneNumber);
    person3.insert("address", obj3.address);
    person3.insert("date", obj3.date);

    IdNameBody obj4(4, "Han en zee", "female", "29", "010-8075-7593",
                    "Suwon", QDateTime::currentDateTime().toString());
    QJsonObject person4;
    person4.insert("id", obj4.id);
    person4.insert("name", obj4.name);
    person4.insert("gender", obj4.gender);
    person4.insert("age", obj4.age);
    person4.insert("phoneNumber", obj4.phoneNumber);
    person4.insert("address", obj4.address);
    person4.insert("date", obj4.date);

    QJsonObject content;
    content.insert( "P1", person1 );
    content.insert( "P2", person2 );
    content.insert( "P3", person3 );
    content.insert( "P4", person4 );

    QJsonDocument document;
    document.setObject( content );
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file( path );
    /*파일을 저수준 언어(C)로 접근하기위한 과정*/
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        //iStream.setCodec( "utf-8" );      //코덱을 할 필요가 없어짐
        iStream << bytes;
        file.close();
    }
    else
    {
        cout << "file open failed: " << path.toStdString() << endl;
    }
}
