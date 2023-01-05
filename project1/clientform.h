#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>

class QSqlQuery;
class QSqlTableModel;

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = nullptr);
    ~ClientForm();
    void loadData();

private slots:
    void on_AddPushButton_clicked();

//    void on_modifyPushButton_clicked();

private:
    Ui::ClientForm *ui;
    QSqlQuery * query;
    QSqlTableModel * queryModel;
    int makeId();
};

#endif // CLIENTFORM_H
