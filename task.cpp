#include "task.h"
#include <QtSql>

Task::Task(const QUuid& taskSetId)
{
    m_id=QUuid::createUuid();
    m_name="无标题任务";
    m_timeRange=TimeRange();    // 开始时间为当前时间，结束值invalid
    m_taskStateFlag=Ongoing;
    m_taskSetId=taskSetId;
}

QString Task::getName() const
{
    return m_name;
}

QString Task::getDescription() const
{
    return m_description;
}

// QString Task::getTags() const
// {
//     return m_tags;
// }

TimeRange Task::getTimeRange() const
{
    return m_timeRange;
}

TaskStateFlag Task::getTaskStateFlag() const
{
    return m_taskStateFlag;
}

bool Task::getIsFinished() const
{
    return m_isFinished;
}

QUuid Task::getTaskId() const
{
    return m_id;
}

QUuid Task::getTaskSetId() const
{
    return m_taskSetId;
}

void Task::setName(const QString& name)
{
    m_name=name;
}

void Task::setDescription(const QString& description)
{
    m_description=description;
}

// void Task::deleteTag(const QString& tag)
// {
//     QStringList allTags = m_tags.split(", ");
//     QString res_tags;
//     for(QString tmp: allTags){
//         if(tmp!=tag) res_tags+=tmp; // 保留剩余标签
//     }
//     m_tags = res_tags;
// }

void Task::setBegin(const QDateTime& begin)
{
    m_timeRange.setBegin(begin);
    updateTaskStateFlag();
}

void Task::setEnd(const QDateTime &end)
{
    m_timeRange.setEnd(end);
    updateTaskStateFlag();
}

void Task::setTimeRange(const TimeRange &tr)
{
    m_timeRange.update(tr);
    updateTaskStateFlag();
}

void Task::setIsFinished(bool isFinished)
{
    m_isFinished=isFinished;
    updateTaskStateFlag();
}

void Task::updateTaskStateFlag()
{
    if(m_isFinished==true){ // 只有已完成，状态就为完成
        m_taskStateFlag=Finished;
    }else if(QDateTime::currentDateTime()<m_timeRange.getBegin()){
        m_taskStateFlag=Unstarted;
    }else if(QDateTime::currentDateTime()>=m_timeRange.getBegin() && QDateTime::currentDateTime()<m_timeRange.getEnd()){
        m_taskStateFlag=Ongoing;
    }else{
        m_taskStateFlag=Unfinished;
    }
}

Task::Task(const QString &t_id, const QString &t_name, const QString &t_description, const QString &t_timeRange, int t_isFinished, int t_state, const QUuid& taskSetId)
{
    m_id = QUuid::fromString(t_id);
    m_name = t_name;
    m_description = t_description;
    // m_tags = t_tags;
    m_timeRange = TimeRange(t_timeRange);
    if(t_isFinished==1) m_isFinished=true;
    else m_isFinished=false;
    switch(t_state){
    case 0:
        m_taskStateFlag=Unstarted;
        break;
    case 1:
        m_taskStateFlag=Ongoing;
        break;
    case 2:
        m_taskStateFlag=Finished;
        break;
    case 3:
        m_taskStateFlag=Unfinished;
        break;
    default:
        break;
    }
    m_taskSetId=taskSetId;
}

// void Task::addTag(const QString& tag)
// {
//     m_tags += ", "+tag;
// }

//
Task::Task(const QString& name, const QString& description, const TimeRange &timeRange, TaskStateFlag taskStateFlag, const QUuid& taskSetId)
    : m_name(name), m_description(description), m_timeRange(timeRange), m_taskStateFlag(taskStateFlag), m_taskSetId(taskSetId)
{
    m_id=QUuid::createUuid();
}

// Task::Task(const Task &task)
// {
//     m_id=QUuid::createUuid();   // 生成新的id
//     m_name=task.getName();
//     m_description=task.getDescription();
//     m_tags=task.getTags();
//     m_timeRange=task.getTimeRange();
//     m_taskStateFlag=task.getTaskStateFlag();
//     m_isFinished=task.getIsFinished();
// }

// Task Task::loadTaskFromDatabase()
// {
//     // qDebug() << QSqlDatabase::drivers();
//     // 添加 SQLite 驱动
//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     // 设置数据库文件路径
//     db.setDatabaseName("database.db");
//     // 打开数据库连接
//     if (!db.open()) {
//         qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
//         return {};
//     }
//     // 执行 SQL 查询
//     QSqlQuery query;
//     if (!query.exec("SELECT * FROM your_table")) {
//         qDebug() << "Error:" << query.lastError().text();
//     } else {
//         while (query.next()) {
//             // 处理查询结果
//         }
//     }
//     // 关闭数据库连接
//     db.close();
// }

QVector<Task> Task::loadTasksFromDB(const QVector<QUuid>& tasksetIds, const QString& dbFileName)
{
    // 输出可用的数据库驱动列表
    // qDebug() << QSqlDatabase::drivers();

    // 添加 SQLite 驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(dbFileName);

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return {};
    }

    // 执行 SQL 查询
    QSqlQuery query;
    QVector<Task> res;
    for(int i=0; i<tasksetIds.size(); i++){
        query.prepare("SELECT * FROM tasks WHERE ts_id = ?");
        query.bindValue(0, tasksetIds[i].toString());
        if(query.exec()){
            while(query.next()){
                Task tmp = Task(query.value("t_id").toString(),
                                query.value("t_name").toString(),
                                query.value("t_description").toString(),
                                query.value("t_timeRange").toString(),
                                query.value("t_isFinished").toInt(),
                                query.value("t_state").toInt(),
                                tasksetIds[i]);
                res.append(tmp);
            }
        }
    }

    db.close();

    return res;
}

bool Task::saveToDB(const QString &dbFileName) const
{
    // 输出可用的数据库驱动列表
    // qDebug() << QSqlDatabase::drivers();

    // 添加 SQLite 驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(dbFileName);

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    // 执行 SQL 查询
    QSqlQuery anotherquery;
    anotherquery.prepare("SELECT COUNT(*) FROM tasks WHERE t_id = :taskId");
    anotherquery.bindValue(":taskId", m_id.toString());
    if(anotherquery.exec() && anotherquery.next()){
        int count = anotherquery.value(0).toInt();      // 是否是新建的任务
        if(count>0){
            QSqlQuery query;
            query.prepare("UPDATE tasks SET t_name = :taskName, t_description = :taskDescription, "
                          "t_timeRange = :taskTimeRange, t_isFinished = :taskIsFinished, "
                          "t_state = :taskState, ts_id = :taskSetId WHERE t_id = :taskId");
            query.bindValue(":taskName", m_name); // 更新任务的名称
            query.bindValue(":taskDescription", m_description); // 更新任务的描述
            query.bindValue(":taskTimeRange", m_timeRange.toString()); // 更新任务的时间范围
            query.bindValue(":taskIsFinished", (m_isFinished ? 1 : 0)); // 更新任务是否完成的值
            int taskStateFlag = m_taskStateFlag;
            query.bindValue(":taskState", taskStateFlag); // 更新任务的状态值
            query.bindValue(":taskSetId", m_taskSetId.toString()); // 更新任务所属的任务集ID
            query.bindValue(":taskId", m_id.toString()); // 更新项的主键
            if (!query.exec()) {
                qDebug() << "Error: Failed to update data in tasks table:" << query.lastError().text();
                db.close();
                return false;
            }
        }else{
            QSqlQuery query;
            query.prepare("INSERT INTO tasks (t_id, t_name, t_description, t_timeRange, t_isFinished, t_state, ts_id) "
                          "VALUES (:taskId, :taskName, :taskDescription, :taskTimeRange, :taskIsFinished, :taskState, :taskSetId)");
            query.bindValue(":taskId", m_id.toString()); // 替换为你想要插入的任务ID
            query.bindValue(":taskName", m_name); // 替换为任务的名称
            query.bindValue(":taskDescription", m_description); // 替换为任务的描述
            // query.bindValue(":taskTags", m_tags); // 替换为任务的标签
            query.bindValue(":taskTimeRange", m_timeRange.toString()); // 替换为任务的时间范围
            query.bindValue(":taskIsFinished", (m_isFinished?1:0)); // 替换为任务是否完成的值，0 表示未完成，1 表示已完成
            int taskStateFlag = m_taskStateFlag;
            query.bindValue(":taskState", taskStateFlag); // 替换为任务的状态值
            query.bindValue(":taskSetId", m_taskSetId.toString()); // 替换为任务所属的任务集ID
            if (!query.exec()) {
                qDebug() << "Error: Failed to insert data into tasks table:" << query.lastError().text();
                db.close();
                return false;
            }
        }
    }
    db.close();
    return true;


}

bool Task::deleteSelfFromDB(const QString &dbFileName) const
{
    // 添加 SQLite 驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(dbFileName);

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    // 执行 SQL 查询
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE t_id = :taskId");
    query.bindValue(":taskId", m_id);

    if (!query.exec()) {
        qDebug() << "Error deleting task from table tasks:" << query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;

}


