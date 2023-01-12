#ifndef PATIENTCAREFORM_H
#define PATIENTCAREFORM_H

#include <QWidget>

namespace Ui {
class PatientCareForm;
}

class PatientCareForm : public QWidget
{
    Q_OBJECT

public:
    explicit PatientCareForm(QWidget *parent = nullptr);
    ~PatientCareForm();

private:
    Ui::PatientCareForm *ui;
};

#endif // PATIENTCAREFORM_H
