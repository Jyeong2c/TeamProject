#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientform.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    client = new ClientForm(this);
    client->setWindowTitle(tr("client"));
    connect(client, SIGNAL(destroyed()),                           //객체가 삭제하기 전에 시그널을 줌
            client, SLOT(deleteLater()));
    client->loadData();

    QMdiSubWindow * cw = ui->mdiArea->addSubWindow(client);
    ui->mdiArea->setActiveSubWindow(cw);

}

MainWindow::~MainWindow()
{
    delete ui;
}

