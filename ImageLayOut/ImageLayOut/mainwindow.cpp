#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imagemanagementform.h"
#include "patientcareform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* WindowTitle 지정 */
    imageManagementForm = new ImageManagementForm(this);
    imageManagementForm->setWindowTitle(tr("Image Management Page"));
    connect(imageManagementForm, SIGNAL(destroyed()), imageManagementForm, SLOT(deleteLater()));

    patientCareForm = new PatientCareForm(this);
    patientCareForm->setWindowTitle(tr("Patient CareForm Page"));
    connect(patientCareForm, SIGNAL(destroyed()), patientCareForm, SLOT(deleteLater()));

    imageManagementForm->loadData();

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(imageManagementForm);
    ui->mdiArea->addSubWindow(patientCareForm);
    ui->mdiArea->setActiveSubWindow(cw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

