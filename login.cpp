#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QDebug>
#include <QPixmap>

LogIn::LogIn(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);

    QPixmap pixmap=QPixmap(":/new/prefix1/ikun.png").scaled(this->size());
    QPalette palette = this->palette();
    //设置主窗口背景图片
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(palette);


    //设置背景图片
    // QPalette pal = this->palette();
    // pal.setBrush(QPalette:,QBrush(QPixmap(":/new/prefix1/ikun.png").scaled(this->size())));
    // setPalette(pal);

    //设置无边框
    //    setWindowFlag(Qt::FramelessWindowHint);

    //设置窗口标题
    this->setWindowTitle("登录");

    //设置密文登录
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

}

LogIn::~LogIn()
{
    delete ui;
}

QString LogIn::getUserName() const
{
    return ui->lineEdit_username->text();
}

QString LogIn::getPassword() const
{
    return ui->lineEdit_password->text();
}

void LogIn::loginFailedOperation()
{
    QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码输入错误！"),QMessageBox::Ok);
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_username->setFocus();
}

void LogIn::on_pushButton_login_clicked()
{
    // if(ui->lineEdit_username->text() == "admin" && ui->lineEdit_password->text() == "123456"){
    //     this->close();
    //     qDebug()<<"hello world";
    // }else{
    //     QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码输入错误！"),QMessageBox::Ok);
    //     ui->lineEdit_username->clear();
    //     ui->lineEdit_password->clear();
    //     ui->lineEdit_username->setFocus();

    // }
    emit to_loginValidate();
}


void LogIn::on_pushButton_register_clicked()
{
    // 关闭登陆页面
    this->close();
    // 触发注册信号
    emit this->to_register();
}

