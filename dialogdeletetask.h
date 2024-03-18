#ifndef DIALOGDELETETASK_H
#define DIALOGDELETETASK_H

#include <QDialog>
#include "task.h"

namespace Ui {
class DialogDeleteTask;
}

class DialogDeleteTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeleteTask(QVector<Task>& tasks, QWidget *parent = nullptr);
    ~DialogDeleteTask();
    QString getSelectedTaskId() const;

private:
    Ui::DialogDeleteTask *ui;
};

#endif // DIALOGDELETETASK_H
