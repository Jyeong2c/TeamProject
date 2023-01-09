#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>

class QSqlQuery;
class QSqlTableModel;
class QSqlRelationalTableModel;

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = nullptr);
    ~ClientForm();
    void loadData() ;

signals:
    void clientAdd(int ,QString);

private slots:
    void on_addPushButton_clicked();
    void showContextMenu(const QPoint &);
    void on_modifyPushButton_clicked();
    void removeItem();
//    void on_enterPushButton_clicked();

    void on_clientTableView_clicked(const QModelIndex &index);

private:
    Ui::ClientForm *ui;
    QSqlQuery *query;
    QSqlTableModel *queryModel;
    int makeId();
    QMenu * menu;
    void clearText();
    QSqlRelationalTableModel    *modelMain;
    void createRelationalTable();
//    QSqlRelationalTableModel    *modelDevice;


};

#endif // CLIENTFORM_H
