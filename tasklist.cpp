#include "tasklist.h"
#include "ui_tasklist.h"

taskList::taskList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskList)
{
    ui->setupUi(this);
    this->show();
}

taskList::~taskList()
{
    delete ui;
}
