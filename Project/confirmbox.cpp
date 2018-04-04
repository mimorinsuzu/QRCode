#include "confirmbox.h"
#include "ui_confirmbox.h"

ConfirmBox::ConfirmBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmBox)
{
    ui->setupUi(this);

    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
      this->setAttribute(Qt::WA_DeleteOnClose);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //确定按钮
    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
    //设置按钮样式
    QFont iconFont;
    iconFont.setPointSize(10);
    ui->btnMenu_Close->setFont(iconFont);
    ui->btnMenu_Close->setText("X");
}

ConfirmBox::~ConfirmBox()
{
    delete ui;
}

void ConfirmBox::on_btnOk_clicked()
{
    done(1);
    this->close();
}
