#include "patientcareform.h"
#include "ui_patientcareform.h"

PatientCareForm::PatientCareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientCareForm)
{
    ui->setupUi(this);
}

PatientCareForm::~PatientCareForm()
{
    delete ui;
}
