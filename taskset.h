#ifndef TASKSET_H
#define TASKSET_H

#include"task.h"
#include<QString>
#include<QVector>
#include<QDateTime>
#include<QUuid>

// 任务集类
class TaskSet
{
public:
    TaskSet(const QUuid& userId);
    TaskSet(const QString& name, const QUuid& userId);
    TaskSet(const TaskSet& taskSet) = default;  // 默认复制构造函数



    // 从数据库加载
    // static TaskSet loadTaskFromDatabase();
    static QVector<TaskSet> loadTasksetsFromDB(const QString& u_id_str, const QString& dbFileName);


    bool saveToDB(const QString& dbFileName) const;             // 保存到数据库 TODO
    bool deleteSelfFromDB(const QString& dbFileName) const;     // 将自身从数据库删除

    QString getName() const;
    // QVector<QUuid> getTaskids() const;
    QUuid getTaskSetId() const;
    QUuid getUserId() const;



    // 编辑
    void setName(const QString& name);
    void changeTaskName(int taskId, const QString& name);
    void changeTaskDescription(int taskId, const QString& description);
    // void addTaskTag(int taskId, const QString& tag);
    // void deleteTaskTag(int taskId, const QString& tag);
    void changeTaskBegin(int taskId, const QDateTime& begin);
    void changeTaskEnd(int taskId, const QDateTime& end);
    void setTaskIsFinished(int taskId, bool isFinished);
    // void addTaskId(const QUuid& taskId);
    // void deleteTaskId(const QUuid& taskId);

private:
    QUuid m_id;                     // 任务集id
    QString m_name;                 // 任务集名称
    // QVector<QUuid> m_tasks;         // 任务id集
    QUuid m_userId;                 // 所属用户的id

    TaskSet(const QString& ts_id, const QString& ts_name, const QUuid& userId);          // 私有构造函数，用于从数据库加载任务集


};

#endif // TASKSET_H
