#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

    QString getUserName() const;
    QString getPassword() const;

    void loginFailedOperation();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_register_clicked();
signals:
    void to_register();
    void to_loginValidate();
private:
    Ui::LogIn *ui;
};

#endif // LOGIN_H
