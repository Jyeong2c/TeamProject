#ifndef IDNAMEBODY_H
#define IDNAMEBODY_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>


/*Json 파일에 ID, 이름, 내용이 들어간 클래스*/
class IdNameBody
{
public:
    /*일반 생성자*/
    IdNameBody(int = 0, QString = "", QString = "", QString = "",
               QString = "", QString = "", QString = "");

    /*Json파일을 생성하기 위한 함수*/
    void CreateJson(const QString &);

private:
    /*Json 데이터로 선언되는 멤버함수 : ID, 이름, 내용*/
    /*ID, Name, Gender, Age, phoneNumber, Address, date*/
    int id;
    QString name;
    QString gender;
    QString age;
    QString phoneNumber;
    QString address;
    QString date;
};

#endif // IDNAMEBODY_H
