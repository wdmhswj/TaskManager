#include "dialogdeletetaskset.h"
#include "ui_dialogdeletetaskset.h"

DialogDeleteTaskSet::DialogDeleteTaskSet(QVector<TaskSet>& taskSets, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDeleteTaskSet)
{
    ui->setupUi(this);
    int taskSets_num = taskSets.size();

    for(int i=0; i<taskSets_num; ++i){
        ui->comboBox->addItem(taskSets[i].getName(), taskSets[i].getTaskSetId().toString());   // 任务集：名称，id
    }
}

DialogDeleteTaskSet::~DialogDeleteTaskSet()
{
    delete ui;
}

QString DialogDeleteTaskSet::getSelectedTaskSetId() const
{
    qDebug()<<"getSelectedTaskSet";
    qDebug()<<ui->comboBox->currentText();
    qDebug()<<ui->comboBox->currentData();
    return ui->comboBox->currentData().toString();
}
