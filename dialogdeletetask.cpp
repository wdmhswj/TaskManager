#include "dialogdeletetask.h"
#include "ui_dialogdeletetask.h"

DialogDeleteTask::DialogDeleteTask(QVector<Task>& tasks, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDeleteTask)
{
    ui->setupUi(this);
    int tasks_num = tasks.size();
    for(int i=0; i<tasks_num; ++i){
        ui->comboBox->addItem(tasks[i].getName(), tasks[i].getTaskId().toString());   // 任务集：名称，id
    }
}

DialogDeleteTask::~DialogDeleteTask()
{
    delete ui;
}

QString DialogDeleteTask::getSelectedTaskId() const
{
    qDebug()<<"getSelectedTaskId";
    qDebug()<<ui->comboBox->currentText();
    qDebug()<<ui->comboBox->currentData();
    return ui->comboBox->currentData().toString();
}
