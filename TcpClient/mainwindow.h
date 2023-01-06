#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);


private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    const int PORT_NUMBER = 1024;
    int cnt = 0;
};
#endif // MAINWINDOW_H
