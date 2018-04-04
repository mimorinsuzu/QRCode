#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include <QDialog>

namespace Ui {
class ConfirmBox;
}

class ConfirmBox : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmBox(QWidget *parent = 0);
    ~ConfirmBox();

private:
    Ui::ConfirmBox *ui;

private slots:
    void on_btnOk_clicked();
};

#endif // CONFIRMBOX_H
