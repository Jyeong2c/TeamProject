#include "idnamebody.h"
#include <iostream>

using namespace std;

IdNameBody::IdNameBody(int _id, QString _name, QString _body)
{
    id = _id;
    name = _name;
    body = _body;
}

/*명시적으로 작성한 Json 인코딩 함수 (비효율적)*/
void IdNameBody::CreateJson(const QString &path)
{
    IdNameBody obj1(1, "jeong jae hyeon", "He is Team Leader");
    QJsonObject person1;
    person1.insert("id", obj1.id);
    person1.insert("name", obj1.name);
    person1.insert("body", obj1.body);

    IdNameBody obj2(2, "jeong yu na", "She is Project Manager");
    QJsonObject person2;
    person2.insert("id", obj2.id);
    person2.insert("name", obj2.name);
    person2.insert("body", obj2.body);

    IdNameBody obj3(3, "jo jae yeong", "He is Project team");
    QJsonObject person3;
    person3.insert("id", obj3.id);
    person3.insert("name", obj3.name);
    person3.insert("body", obj3.body);

    IdNameBody obj4(4, "Han en gee", "She is Project team");
    QJsonObject person4;
    person4.insert("id", obj4.id);
    person4.insert("name", obj4.name);
    person4.insert("body", obj4.body);

    QJsonObject content;
    content.insert( "P1", person1 );
    content.insert( "P2", person2 );
    content.insert( "P3", person3 );
    content.insert( "P4", person4 );

    QJsonDocument document;
    document.setObject( content );
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file( path );
    //파일을 저수준 언어(C)로 접근하기위한 과정
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
