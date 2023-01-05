#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>

namespace Ui {
class ClientForm;
}

class ClientForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = nullptr);
    ~ClientForm();

private:
    Ui::ClientForm *ui;
};

#endif // CLIENTFORM_H
