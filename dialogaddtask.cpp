#include "dialogaddtask.h"
#include "ui_dialogaddtask.h"

DialogAddTask::DialogAddTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAddTask)
{
    ui->setupUi(this);
    ui->comboBox->addItem("未开始", 0);
    ui->comboBox->addItem("进行中", 1);
    ui->comboBox->addItem("已完成", 2);
    ui->comboBox->addItem("已过期", 3);
}

DialogAddTask::~DialogAddTask()
{
    delete ui;
}

void DialogAddTask::getNewTask(QString *name, QString *description, QDateTime *begin, QDateTime *end, TaskStateFlag *state)
{
    *name = ui->lineEdit_name->text();
    *description = ui->lineEdit_description->text();
    *begin = ui->dateTimeEdit_begin->dateTime();
    *end = ui->dateTimeEdit_end->dateTime();
    *state = static_cast<TaskStateFlag>(ui->comboBox->currentData().toInt());
}
