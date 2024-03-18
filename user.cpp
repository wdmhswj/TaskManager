#include "user.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QCryptographicHash>

User::User(const QString &username, const QString& password, const QString& email, const QString& dbFileName)
    :m_username(username), m_email(email), m_dbFileName(dbFileName)
{
    // 生成随机的UUID
    m_id=QUuid::createUuid();
    // 使用 SHA-256 哈希加密密码
    m_passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

// 创建默认任务集以及默认任务

    // 创建当前时间的 QDateTime
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 创建 100 年后的 QDateTime
    QDateTime futureDateTime = currentDateTime.addYears(100);

    TaskSet defaultTaskSet = TaskSet("默认任务集", m_id);
    Task defaultTask = Task("默认任务", "默认描述", TimeRange(currentDateTime, futureDateTime), Ongoing, defaultTaskSet.getTaskSetId());

    // defaultTaskSet.addTaskId(defaultTask.getTaskId());  // 添加默认任务的id到默认任务集中
    // m_tasksets.append(defaultTaskSet.getTaskSetId());   // 添加默认任务集的id到用户中

    // 默认任务集和默认任务保存到数据库
    if(!defaultTask.saveToDB(m_dbFileName))
        qDebug()<<"默认任务保存到数据库失败";
    if(!defaultTaskSet.saveToDB(m_dbFileName))
        qDebug()<<"默认任务集保存到数据库失败";

}

User::~User()
{

}

// Task User::loadTaskFromDatabase()
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


void User::setDbName(const QString &dbName)
{
    m_dbFileName=dbName;
}

User *User::loadUserFromDB(const QString &dbName, const QString &userName, const QByteArray &passwordHash)
{
    User* newUser = new User(userName, passwordHash, dbName);
    return newUser;
}

// int User::getTasksetsNum() const
// {
//     return m_tasksets.length();
// }

QUuid User::getUserId() const
{
    return m_id;
}

bool User::saveToDb()
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
    db.setDatabaseName(m_dbFileName);

    QSqlQuery query;
    query.prepare("INSERT INTO users (u_id, u_name, password_hash, email) VALUES (:userId, :userName, :passwordHash, :email)");
    query.bindValue(":userId", m_id.toString());   // m_id自动转化为QString类型
    query.bindValue(":userName", m_username);
    qDebug()<<"QString(m_passwordHash.toHex(): "<<QString(m_passwordHash.toHex());
    query.bindValue(":passwordHash", QString(m_passwordHash.toHex()));
    query.bindValue(":email", m_email);
    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data into users table:" << query.lastError().text();
        db.close();
        return false;
    }
    db.close();
    return true;

}

User::User(const QString &username, const QByteArray &passwordHash, const QString& dbFileName)
{
    m_username=username;
    m_passwordHash=passwordHash;
    m_dbFileName=dbFileName;
    // 从数据库查找并加载 用户ID、邮箱、任务集链表
    // 输出可用的数据库驱动列表
    // qDebug() << QSqlDatabase::drivers();
    // 添加 SQLite 驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    // 设置数据库文件路径
    db.setDatabaseName(m_dbFileName);

    // 设置SQL查询语句
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE u_name = :username AND password_hash = :passwordHash");
    query.bindValue(":username", m_username);
    query.bindValue(":passwordHash", QString(m_passwordHash.toHex()));

    // 执行SQL语句
    if(query.exec() && query.next()){
        m_id= query.value("u_id").toUuid();       // 加载用户id
        m_email = query.value("email").toString();  // 加载Email
        // // 进行另一条 SQL 查询
        // QSqlQuery anotherQuery;
        // anotherQuery.prepare("SELECT * FROM taskSets WHERE u_id = :user_id");
        // anotherQuery.bindValue(":user_id", m_id);
        // if (anotherQuery.exec()) {
        //     // 处理查询结果
        //     while(anotherQuery.next()){ // 可以有多行结果
        //         m_tasksets.append(anotherQuery.value("ts_id").toUuid());
        //     }

        // } else {
        //     qDebug() << "Error: Failed to execute taskSet_id query:" << anotherQuery.lastError().text();
        // }
    }else{
        qDebug()<<"用户数据从数据库加载失败";
    }
    db.close();
}
