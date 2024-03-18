#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QUuid>
#include "myStructs.h"


class Task
{
public:
    // 构造析构
    Task(const QUuid& taskSetId); // 没有指定任何成员变量，默认初始化
    Task(const QString& name, const QString& description, // 成员变量依次初始化
         const TimeRange& timeRange, TaskStateFlag taskStateFlag, const QUuid& taskSetId);
    Task(const Task& task) = default;

    Task(const QString& t_id, const QString& t_name, const QString& t_description,
         const QString& t_timeRange, int t_isFinished,
         int t_state, const QUuid& taskSetId);
    ~Task()=default;

    // 从数据库加载
    // static Task loadTaskFromDatabase();
    static QVector<Task> loadTasksFromDB(const QVector<QUuid>& tasksetIds, const QString& dbFileName);


    bool saveToDB(const QString& dbFileName) const;                     // 保存到数据库 TODO
    bool deleteSelfFromDB(const QString &dbFileName) const;             // 将自身从数据库删除
    // 获取成员变量
    QString getName() const;
    QString getDescription() const;
    // QString getTags() const;
    TimeRange getTimeRange() const;
    TaskStateFlag getTaskStateFlag() const;
    bool getIsFinished() const;
    QUuid getTaskId() const;
    QUuid getTaskSetId() const;



    // 编辑
    void setName(const QString& name);
    void setDescription(const QString& description);
    // void addTag(const QString& tag);
    // void deleteTag(const QString& tag);
    void setBegin(const QDateTime& begin);
    void setEnd(const QDateTime& end);
    void setTimeRange(const TimeRange& tr);
    void setIsFinished(bool isFinished);
    void updateTaskStateFlag();

private:
    QUuid m_id;                         // 任务ID
    QString m_name;                     // 名称
    QString m_description;              // 内容描述
    // QString  m_tags;   // 分类标签
    TimeRange m_timeRange;              // 时间范围
    TaskStateFlag m_taskStateFlag;      // 任务状态
    bool m_isFinished=false;            // 是否完成（默认初始化时为false）

    QUuid m_taskSetId;                  // 所属taskSet的id


};

#endif // TASK_H
