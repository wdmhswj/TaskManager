#ifndef REGISTER_W_H
#define REGISTER_W_H

#include <QDialog>


namespace Ui {
class register_w;
}

class register_w : public QDialog
{
    Q_OBJECT

public:
    explicit register_w(QWidget *parent = nullptr);
    ~register_w();
    QString getUserName() const;
    QString getPassword1() const;
    QString getPassword2() const;
    QString getEmail() const;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void to_registerValidate();
    void to_returnToLogin();
private:
    Ui::register_w *ui;
};

#endif // REGISTER_W_H
