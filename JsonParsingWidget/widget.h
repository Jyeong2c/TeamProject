#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QFile>
#include <iostream>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class IdNameBody;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

private slots:
    void on_SelectSend_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;

    std::unique_ptr<QFile> file;

    bool fd_flag = false;
    bool send_flag = false;
    const int PORT_NUMBER = 1024;
    int cnt = 0;
};
#endif // WIDGET_H
