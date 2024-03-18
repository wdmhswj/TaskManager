#ifndef USER_H
#define USER_H

#include<QString>
// #include<QCryptographicHash>
#include<QByteArray>
#include<QDebug>
#include<QVector>
#include<QList>
#include<QUuid>
#include"taskset.h"


// 用户类
class User
{
public:
    User()=delete;  // 删除（禁用）默认构造函数
    User(const QString& username, const QString& password, const QString& email="", const QString& dbFileName = "database.db");    // 注册创建用户时使用
    ~User();


    // static Task loadTaskFromDatabase();                             // 从数据库加载
    void setDbName(const QString& dbName);                          // 设置数据库文件名称
    static User *loadUserFromDB(const QString& dbName,              // 从数据库加载用户
                                const QString& userName,
                                const QByteArray& passwordHash
                                );
    // int getTasksetsNum() const;                                     // 获取任务集的个数
    QUuid getUserId() const;                                      // 获取用户id

    bool saveToDb();    // 用户数据保存到数据库
    void test(){
        // 将哈希值转换为十六进制字符串
        QString hashString = QString(m_passwordHash.toHex());
        qDebug() << "Password hash (SHA-256):" << hashString;
        // 输出Uid
        qDebug() << m_id.toString();

        // // 输出任务集相关信息
        // for(QUuid taskSetid: m_tasksets){
        //     qDebug()<<taskSetid;
        // }
    }
private:
    QUuid m_id;    // 用户ID
    QString m_username; // 用户名
    QByteArray m_passwordHash;  // 密码哈希值
    QString m_email;    // 邮箱
    // QList<QUuid> m_tasksets;  // 任务集链表
    QString m_dbFileName;   // 数据库文件名

    // 私有函数

    User(const QString& username, const QByteArray& passwordHash, const QString& dbFileName); // 私有构造函数，用于从数据库加载用户


};


#endif // USER_H
