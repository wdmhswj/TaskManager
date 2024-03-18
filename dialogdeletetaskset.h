#ifndef DIALOGDELETETASKSET_H
#define DIALOGDELETETASKSET_H

#include <QDialog>
#include "taskset.h"

namespace Ui {
class DialogDeleteTaskSet;
}

class DialogDeleteTaskSet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeleteTaskSet(QVector<TaskSet>& taskSets, QWidget *parent = nullptr);
    ~DialogDeleteTaskSet();
    QString getSelectedTaskSetId() const;
private:
    Ui::DialogDeleteTaskSet *ui;
};

#endif // DIALOGDELETETASKSET_H
