#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "memoalbum.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImageManagementForm;
class PatientCareForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ImageManagementForm *imageManagementForm;
    PatientCareForm *patientCareForm;
};
#endif // MAINWINDOW_H
