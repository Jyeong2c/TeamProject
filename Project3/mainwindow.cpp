#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    client = new ClientForm;
    QTabWidget *tab = new QTabWidget;

//    client->loadData();

    QMdiSubWindow * cw = ui->mdiArea->addSubWindow(client);
    ui->mdiArea->setActiveSubWindow(cw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

