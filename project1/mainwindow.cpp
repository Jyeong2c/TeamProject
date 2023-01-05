#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientform.h"
#include <QTabWidget>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QToolBar *tool = new QToolBar;

//    tool->addAction("test");
    client = new ClientForm;
    QTabWidget *tab = new QTabWidget;

//    tab->addTab(QWidget(0), "Client");
//    tab->addTab(1, "View");
    client->loadData();

    QMdiSubWindow * cw = ui->mdiArea->addSubWindow(client);
   // ui->mdiArea->addSubWindow(client);
    ui->mdiArea->setActiveSubWindow(cw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

