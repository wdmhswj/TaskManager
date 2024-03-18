#include "register_w.h"
#include "ui_register_w.h"

register_w ::register_w(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::register_w)
{
    ui->setupUi(this);

    //设置窗口标题
    this->setWindowTitle("注册");

    //设置密文登录
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_ensure_password->setEchoMode(QLineEdit::Password);
    // //设置图片
    // QPixmap *pix = new QPixmap(":/new/prefix1/register.jpg");
    // QSize sz = ui->label_image->size();
    // ui->label_image->setPixmap(pix->scaled(sz));

    // QPixmap pixmap=QPixmap(":/new/prefix1/register.jpg").scaled(this->size());
    // QPalette palette = this->palette();
    // //设置主窗口背景图片
    // palette.setBrush(QPalette::Window,QBrush(pixmap));
    // this->setPalette(palette);

}

register_w ::~register_w()
{
    delete ui;

}

QString register_w::getUserName() const
{
    return ui->lineEdit_username->text();
}

QString register_w::getPassword1() const
{
    return ui->lineEdit_password->text();
}

QString register_w::getPassword2() const
{
    return ui->lineEdit_ensure_password->text();
}

QString register_w::getEmail() const
{
    return ui->lineEdit_email->text();
}


void register_w::on_pushButton_clicked()
{
    // 触发信号由主widget接收
    emit to_registerValidate();
    qDebug()<<"emit to_registerValidate()";

    // if(ui->lineEdit_password->text() != ui->lineEdit_ensure_password->text())   // 2次密码不相同
    // {

    // }else if()
    // if(ui->lineEdit_username->text() == "admin" && ui->lineEdit_password->text() == "123456"){
    //     this->close();
    //     qDebug()<<"hello world";
    // }else{
    //     QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码输入错误！"),QMessageBox::Ok);
    //     ui->lineEdit_username->clear();
    //     ui->lineEdit_password->clear();
    //     ui->lineEdit_username->setFocus();

    // }
}


void register_w::on_pushButton_2_clicked()
{
    this->deleteLater();
    emit to_returnToLogin();
}

