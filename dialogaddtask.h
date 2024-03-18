#ifndef DIALOGADDTASK_H
#define DIALOGADDTASK_H

#include <QDialog>
#include "myStructs.h"

namespace Ui {
class DialogAddTask;
}

class DialogAddTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddTask(QWidget *parent = nullptr);
    ~DialogAddTask();
    void getNewTask(QString* name, QString* description,
                    QDateTime* begin, QDateTime* end,
                    TaskStateFlag* state);

private:
    Ui::DialogAddTask *ui;
};

#endif // DIALOGADDTASK_H
