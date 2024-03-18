#include "taskset.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


TaskSet::TaskSet(const QUuid& userId)
    :m_name("无标题任务集"), m_userId(userId)
{
    m_id=QUuid::createUuid();
}

TaskSet::TaskSet(const QString &name, const QUuid& userId)
    :m_name(name), m_userId(userId)
{
    m_id=QUuid::createUuid();
}

// TaskSet::TaskSet(const TaskSet &taskSet)
// {

// }

// TaskSet TaskSet::loadTaskFromDatabase()
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

QVector<TaskSet> TaskSet::loadTasksetsFromDB(const QString& u_id_str, const QString& dbFileName)
{
    qDebug()<<"loadTasksetsFromDB";
    // 输出可用的数据库驱动列表
    // // qDebug() << QSqlDatabase::drivers();

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

    // 从 taskSets 表中查询所有外键 u_id 等于用户id的项
    query.prepare("SELECT * FROM taskSets WHERE u_id = ?");
    query.bindValue(0, u_id_str);

    QVector<TaskSet> res_tasksets;
    if(query.exec()){
        while(query.next()){
            // 通过数据库数据创建任务集对象
            QString ts_id = query.value("ts_id").toString();
            QString ts_name = query.value("ts_name").toString();
            // qDebug()<<"ts_id: "<<ts_id<<" "<<"ts_name: "<<ts_name;
            // QVector<QUuid> tmp_tasks;
            // QSqlQuery anotherquery;
            // anotherquery.prepare("SELECT * FROM tasks WHERE ts_id = ?");
            // anotherquery.bindValue(0, ts_id);
            // if(anotherquery.exec()){
            //     while(anotherquery.next()){
            //         tmp_tasks.append(QUuid::fromString(anotherquery.value("t_id").toString()));
            //         qDebug()<<"anotherquery.value(\"t_id\").toString()"<<anotherquery.value("t_id").toString();
            //     }
            // }
            res_tasksets.append(TaskSet(ts_id, ts_name, QUuid::fromString(u_id_str)));
        }
    }
    db.close();
    return res_tasksets;
}

bool TaskSet::saveToDB(const QString &dbFileName) const
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
    anotherquery.prepare("SELECT COUNT(*) FROM taskSets WHERE ts_id = :taskSetId");
    anotherquery.bindValue(":taskSetId", m_id.toString());
    if(anotherquery.exec() && anotherquery.next()){
        int count = anotherquery.value(0).toInt();      // 是否是新建的任务集
        if(count>0){
            QSqlQuery query;
            query.prepare("UPDATE taskSets SET ts_name = :taskSetName, u_id = :userId WHERE ts_id = :taskSetId");
            query.bindValue(":taskSetName", m_name);
            query.bindValue(":userId", m_userId.toString());
            query.bindValue(":taskSetId", m_id.toString());
            if(!query.exec()){
                qDebug() << "Error: Failed to update data in taskSets table:" << query.lastError().text();
                db.close();
                return false;
            }
        }else{
            QSqlQuery query;
            query.prepare("INSERT INTO taskSets (ts_id, ts_name, u_id) "
                          "VALUES (:taskSetId, :taskSetName, :UserId)");
            query.bindValue(":taskSetId", m_id.toString()); // 替换为你想要插入的任务ID
            query.bindValue(":taskSetName", m_name); // 替换为任务的名称
            query.bindValue(":UserId", m_userId.toString()); // 替换为任务的描述

            if (!query.exec()) {
                qDebug() << "Error: Failed to insert data into taskSets table:" << query.lastError().text();
                db.close();
                return false;
            }
        }
    }
    db.close();
    return true;

}

bool TaskSet::deleteSelfFromDB(const QString &dbFileName) const
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
    query.prepare("DELETE FROM taskSets WHERE ts_id = :taskSetId");
    query.bindValue(":taskSetId", m_id);

    if (!query.exec()) {
        qDebug() << "Error deleting taskSet from table taskSets:" << query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;
}

QString TaskSet::getName() const
{
    return m_name;
}

// QVector<QUuid> TaskSet::getTaskids() const
// {
//     return m_tasks;
// }

QUuid TaskSet::getTaskSetId() const
{
    return m_id;
}

QUuid TaskSet::getUserId() const
{
    return m_userId;
}

void TaskSet::setName(const QString &name)
{
    m_name=name;
}

void TaskSet::changeTaskName(int taskId, const QString &name)
{
    // m_tasks[taskId].setName(name);
}

void TaskSet::changeTaskDescription(int taskId, const QString &description)
{
    // m_tasks[taskId].setDescription(description);
}

// void TaskSet::addTaskTag(int taskId, const QString &tag)
// {
//     // m_tasks[taskId].addTag(tag);
// }

// void TaskSet::deleteTaskTag(int taskId, const QString &tag)
// {
//     // m_tasks[taskId].deleteTag(tag);
// }

void TaskSet::changeTaskBegin(int taskId, const QDateTime &begin)
{

    // m_tasks[taskId].setBegin(begin);
}

void TaskSet::changeTaskEnd(int taskId, const QDateTime &end)
{
    // m_tasks[taskId].setEnd(end);
}

void TaskSet::setTaskIsFinished(int taskId, bool isFinished)
{
    // m_tasks[taskId].setIsFinished(isFinished);
}

// void TaskSet::addTaskId(const QUuid &taskId)
// {
//     m_tasks.append(taskId);
// }

// void TaskSet::deleteTaskId(const QUuid &taskId)
// {
//     for(int i=0; i<m_tasks.size(); ++i){
//         if(m_tasks[i]==taskId){
//             m_tasks.remove(i);
//             break;
//         }
//     }

// }

TaskSet::TaskSet(const QString &ts_id, const QString &ts_name, const QUuid& userId)
{
    m_id = QUuid::fromString(ts_id);
    m_name = ts_name;
    m_userId = userId;
}
