#ifndef TASKLIST_H
#define TASKLIST_H

#include <QWidget>

namespace Ui {
class taskList;
}

class taskList : public QWidget
{
    Q_OBJECT

public:
    explicit taskList(QWidget *parent = nullptr);
    ~taskList();

private:
    Ui::taskList *ui;
};

#endif // TASKLIST_H
