#include "clientform.h"
#include "ui_clientform.h"

ClientForm::ClientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientForm)
{
    ui->setupUi(this);
}

ClientForm::~ClientForm()
{
    delete ui;
}
