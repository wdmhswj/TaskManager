#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "login.h"
#include "register_w.h"
#include "user.h"
#include <QButtonGroup>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "ttextdelegate.h"
#include "tcomboboxdelegate.h"
#include "tdatetimedelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr, const QString dbFileName="database.db");
    ~Widget();

    void mainWidgetInitialization();
private:
    Ui::Widget *ui;
    QButtonGroup btnGroup;

    LogIn* m_login;                                         // 登陆窗口
    register_w* m_register_w;                               // 注册窗口
    QString m_dbFileName;                                   // 数据库文件名
    bool isLogin = false;                                   // 是否成功登陆
    User* m_currentLoginUser = nullptr;                     // 当前登陆成功的用户
    QVector<TaskSet> m_tasksets;                            // 任务集容器
    QVector<Task> m_tasks;                                  // 任务容器
    QStandardItemModel  *m_model;                           // 数据模型
    QItemSelectionModel *m_selection;                       // Item选择模型
    TTextDelegate* m_textDelegate;                          // 文本输入部件
    TComboBoxDelegate* m_comboDelegate;                     // 任务状态代理
    tDateTimeDelegate* m_dateTimeDelegate;                  // 日期时间代理

    // 私有函数
    bool database_init();                                   // 数据库初始化
    // bool loadTasksByTasksetIdFromDB();                      // 依据任务集id从数据库加载改任务集的所有任务
    // bool loadTasksetsFromBD();                              // 从数据库加载该用户的所有任务集
    bool loadTaskSetsTocomboBox();                          // 加载登陆用户的任务集
    void loadTasksToView(int index);                        // 加载更新任务列表
    // void addTaskToTaskSet(const QUuid& taskid);             // 将新添加的任务更新到当前任务集
    // void deleteTaskFromTaskSet(const QUuid& taskid);        // 将删除的任务剔除当前任务集
    // void saveTaskToDB(const Task& task);                    // 保存任务到数据库
    void test();


private slots:
    void do_register();
    void do_registerValidate();
    void do_returnToLogin();
    void do_loginValidate();
    //自定义槽函数，与QItemSelectionModel的currentChanged()信号连接
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_comboBox_currentIndexChanged(int index);
    void do_saveToData(const QModelIndex &index);                                                                 // 数据模型中的数据保存到内存变量中
    void on_pushButton_addTaskSet_clicked();
    void on_pushButton_deleteTaskSet_clicked();
    void on_pushButton_deleteTask_clicked();
    void on_pushButton_addTask_clicked();

    bool on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // WIDGET_H
