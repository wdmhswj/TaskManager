#include "widget.h"
#include "./ui_widget.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QByteArray>
#include <QOverload>
#include <QToolButton>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QInputDialog>
#include <QDateTime>
#include "tasklist.h"
#include "dialogdeletetaskset.h"
#include "dialogdeletetask.h"
#include "dialogaddtask.h"


Widget::Widget(QWidget *parent, const QString dbFileName)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_dbFileName(dbFileName)
{
    ui->setupUi(this);
    // 初始化数据库
    if(!database_init()){
        qDebug()<<"数据库初始化失败";
        return;
    }
    m_login = new LogIn();  // 动态创建login窗口
    m_register_w = nullptr;  // 初始时不创建注册页面
    m_login->show();        // 显示login窗口
    connect(this->m_login, &LogIn::to_register, this, &Widget::do_register);              // login窗口 -> register窗口
    connect(this->m_login, &LogIn::to_loginValidate, this, &Widget::do_loginValidate);    // login窗口 -> 验证

}

void Widget::test(){
    qDebug()<<"void Widget::test()";
    // m_register_w->close();
    qDebug()<<"m_tasksets.size(): "<<m_tasksets.size();
    for(TaskSet& taskSet: m_tasksets){
        qDebug()<<"tasksetName: "<<taskSet.getName();
        qDebug()<<"tasksetId: "<<taskSet.getTaskSetId();
    }
    qDebug()<<"m_tasks.size(): "<<m_tasks.size();
    for(Task& tmp: m_tasks){
        qDebug()<<"taskName: "<<tmp.getName();
        qDebug()<<"taskId: "<<tmp.getTaskId();
    }
    m_currentLoginUser->test();
}

void Widget::mainWidgetInitialization()
{
    qDebug()<<"mainWidgetInitialization() started";
    /*
     * 要实现的功能:
     * 1. 侧边栏加载登陆用户的所有任务集名称
     * 2. 默认选中默认任务集（如果没有默认任务集，则不选择）
     * 3. 任务列表窗口的设置（具体设置由tasklist类窗口确定S）
     * 4. 侧边栏点击与窗口变化的设置
     *
     * 要做的准备：
     * 1. 从数据库加载该用户的所有任务集 =》m_tasksets
     * 2. 从数据库加载该用户的所有任务 =》m_tasks
    */

    // !!!!!
    if(m_currentLoginUser == nullptr){                      // 确认当前登陆用户不为空
        qDebug()<<"当前用户为空";
        return;
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//布局排版是全部伸展开的效果


    // 从数据库加载该用户的所有任务集 =》m_tasksets
    m_tasksets = TaskSet::loadTasksetsFromDB(m_currentLoginUser->getUserId().toString(), m_dbFileName);

    // 从数据库加载该用户的所有任务 =》m_tasks
    QVector<QUuid> tmp_tasksetids;
    for(int i=0; i<m_tasksets.size(); i++){
        tmp_tasksetids.append(m_tasksets[i].getTaskSetId());
    }
    // qDebug()<<"tmp_tasksetids.size(): "<<tmp_tasksetids.size();
    m_tasks = Task::loadTasksFromDB(tmp_tasksetids, m_dbFileName);
    // qDebug()<<"m_tasks.size(): "<<m_tasks.size();

    test();

    m_model = new QStandardItemModel(1, 6, this);                   // 创建数据模型（初始时1行，5列）
    m_selection = new QItemSelectionModel(m_model, this);           // 创建选择模型

    //选择当前单元格变化时的信号与槽
    connect(m_selection,&QItemSelectionModel::currentChanged,
            this,&Widget::do_currentChanged);
    connect(this->m_model, &QStandardItemModel::dataChanged, this, &Widget::do_saveToData);    // 数据模型变化时的信号与槽


    //为tableView设置数据模型
    ui->tableView->setModel(m_model);                                       // 设置数据模型
    ui->tableView->setSelectionModel(m_selection);                          // 设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    if(!loadTaskSetsTocomboBox()){                                          // 加载任务集到下拉栏
        qDebug()<<"任务集加载失败";
    }

    m_textDelegate = new TTextDelegate(this);
    ui->tableView->setItemDelegateForColumn(0, m_textDelegate);             // 任务名称
    ui->tableView->setItemDelegateForColumn(1, m_textDelegate);             // 描述
    m_dateTimeDelegate = new tDateTimeDelegate(this);
    ui->tableView->setItemDelegateForColumn(2, m_dateTimeDelegate);         // 开始日期时间
    ui->tableView->setItemDelegateForColumn(3, m_dateTimeDelegate);         // 截止日期时间
    // ui->tableView->setItemDelegateForColumn(3, m_textDelegate);
    // ui->tableView->setItemDelegateForColumn(4, m_textDelegate);
    // ui->tableView->setItemDelegateForColumn(5, m_textDelegate);

    m_comboDelegate = new TComboBoxDelegate(this);
    QStringList strList;
    strList<<"未开始"<<"进行中"<<"已完成"<<"已过期";
    m_comboDelegate->setItems(strList,false);
    ui->tableView->setItemDelegateForColumn(4, m_comboDelegate);            // 任务状态

    m_comboDelegate = new TComboBoxDelegate(this);
    strList.clear();
    strList<<"√"<<"×";
    m_comboDelegate->setItems(strList,false);
    ui->tableView->setItemDelegateForColumn(5, m_comboDelegate);            // 是否完成

    // 设置表头名称
    QStringList list = { "名称", "描述", "开始时间", "截止时间", "状态", "是否完成" };
    m_model->setHorizontalHeaderLabels(list);


    this->show();


}

Widget::~Widget()
{
    delete ui;
    delete m_login;
    delete m_register_w;
    delete m_currentLoginUser;
}

bool Widget::database_init()
{
    // 输出可用的数据库驱动列表
    // qDebug() << QSqlDatabase::drivers();

    // 添加 SQLite 驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(m_dbFileName);

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return false;
    }

    // 执行 SQL 查询
    QSqlQuery query;

    // 创建表
    // 创建用户table
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ( "
                    "u_id TEXT PRIMARY KEY, "
                    "u_name TEXT, "
                    "password_hash TEXT, "
                    "email TEXT)"
                    )) {
        qDebug() << "-Error1: Failed to create table:" << query.lastError().text();
    }
    // 创建任务集table
    if (!query.exec("CREATE TABLE IF NOT EXISTS taskSets ( "
                    "ts_id TEXT PRIMARY KEY, "
                    "ts_name TEXT, "
                    "u_id TEXT, "
                    "FOREIGN KEY (u_id) REFERENCES users(u_id))"
                    )) {
        qDebug() << "Error2: Failed to create table:" << query.lastError().text();
    }
    // 创建任务table
    if (!query.exec("CREATE TABLE IF NOT EXISTS tasks ( "
                    "t_id TEXT PRIMARY KEY, "
                    "t_name TEXT, "
                    "t_description TEXT, "
                    "t_timeRange TEXT, "
                    "t_isFinished INTEGER CHECK (t_isFinished IN (0, 1)), "
                    "t_state INTEGER CHECK (t_isFinished IN (0,1,2,3)), "
                    "ts_id TEXT, "
                    "FOREIGN KEY (ts_id) REFERENCES taskSets(ts_id))"
                    )) {
        qDebug() << "Error3: Failed to create table:" << query.lastError().text();
    }



    // 关闭数据库连接
    db.close();

    return true;
}

// bool Widget::loadTasksByTasksetIdFromDB()
// {

// }

// bool Widget::loadTasksetsFromBD()
// {
//     if(m_currentLoginUser == nullptr){                      // 确认当前登陆用户不为空
//         qDebug()<<"当前用户为空";
//         return false;
//     }
//     // 输出可用的数据库驱动列表
//     // qDebug() << QSqlDatabase::drivers();

//     // 添加 SQLite 驱动
//     QSqlDatabase db;
//     if(QSqlDatabase::contains("qt_sql_default_connection"))
//         db = QSqlDatabase::database("qt_sql_default_connection");
//     else
//         db = QSqlDatabase::addDatabase("QSQLITE");

//     // 设置数据库文件路径
//     db.setDatabaseName(m_dbFileName);

//     // 打开数据库连接
//     if (!db.open()) {
//         qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
//         return false;
//     }

//     // 执行 SQL 查询
//     QSqlQuery query;

//     // 从 taskSets 表中查询所有外键 u_id 等于用户id的项
//     query.prepare("SELECT * FROM taskSets WHERE u_id = ?");
//     query.bindValue(0, m_currentLoginUser->getUserId());

//     if(query.exec()){
//         while(query.next()){
//             // 通过数据库数据创建任务集对象

//         }
//     }

// }

void Widget::do_register()
{
    qDebug()<<"void Widget::do_register()";
    // m_login->close(); // 关闭登陆窗口（不删除）

    m_register_w = new register_w();
    connect(this->m_register_w, &register_w::to_registerValidate, this, &Widget::do_registerValidate);  // register -> 验证
    connect(this->m_register_w, &register_w::to_returnToLogin, this, &Widget::do_returnToLogin);  // register -> login
    m_register_w->show();
}

void Widget::do_registerValidate()
{
    // qDebug()<<"void Widget::do_registerValidate()";
    // 数据库操作
    // 输出可用的数据库驱动列表
    // qDebug() << QSqlDatabase::drivers();

    QString password1=m_register_w->getPassword1();
    QString password2=m_register_w->getPassword2();
// 注册失败
    // 输入密码的长度
    if(password1.length()<8){
        QMessageBox::warning(this,tr("注册失败"),tr("密码长度需大于等于8位！"),QMessageBox::Ok);
        return;
    }
    // 输入2次密码是否相同
    if(password1 != password2){
        QMessageBox::warning(this,tr("注册失败"),tr("输入2次密码不相同！"),QMessageBox::Ok);
        return;
    }

    // 添加 SQLite 驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(m_dbFileName);

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return;
    }

    // 执行 SQL 查询
    QSqlQuery query;

    QString username=m_register_w->getUserName();

    // 用户名是否重复
    query.prepare("SELECT COUNT(*) FROM users WHERE u_name = ?");
    query.bindValue(0, username);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if(count>0){
            QMessageBox::warning(this,tr("注册失败"),tr("该用户名已存在！"),QMessageBox::Ok);
            db.close();
            return;
        }
    }
    db.close();




// 注册成功：保存用户信息到数据库，关闭注册窗口，打开登陆窗口
    User newUser = User(username, password1, m_register_w->getEmail(), m_dbFileName);   // 创建新用户对象 (会自动创建默认任务集和默认任务）
    newUser.saveToDb();                                                                 // 新用户数据保存到数据库

    QMessageBox::information(this, tr("注册成功"),tr("注册成功！"),QMessageBox::Ok);
    m_register_w->deleteLater();                                            // 删除注册窗口
    m_login->show();

}

void Widget::do_returnToLogin()
{
    // m_register_w->deleteLater();                                            // 删除注册窗口
    m_login->show();
}

void Widget::do_loginValidate()
{
    QString username = m_login->getUserName(); // 要查询的用户名
    if(username.size()==0 || m_login->getPassword().size()==0){
        qDebug() << "用户名或密码为空！";
        QMessageBox::warning(this, tr("登陆失败"),tr("用户名或密码为空！"),QMessageBox::Ok);
        return;
    }
    QByteArray passwordHash = QCryptographicHash::hash(m_login->getPassword().toUtf8(), QCryptographicHash::Sha256);   // 要检查的密码哈希

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

    // 打开数据库连接
    if (!db.open()) {
        qDebug() << "Error: Failed to connect to database:" << db.lastError().text();
        return;
    }

    // 查找是否有相同的用户名

    // 查找密码哈希值是否相等


    // 准备查询语句，检查是否存在相同的用户名
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE u_name = :username AND password_hash = :passwordHash");
    query.bindValue(":username", username);
    query.bindValue(":passwordHash", QString(passwordHash.toHex()));

    // 执行查询
    if (query.exec()) {
        // 遍历查询结果
        if (query.next()) {
            // QString storedPasswordHash = query.value("password_hash").toString();
            // qDebug()<<"storedPasswordHash: "<<storedPasswordHash;
            // QByteArray storedPasswordHash = query.value("password_hash").toLa;
            // // 检查密码哈希值是否相等
            // if (QString(passwordHash.toHex()) == storedPasswordHash) {
            // 用户名和密码哈希值匹配
            qDebug() << "Username and password hash match!";
            m_login->deleteLater();                                                       // 删除登陆窗口
            QMessageBox::information(this, tr("登陆成功"),tr("登陆成功！"),QMessageBox::Ok); // 弹窗：登陆成功
            // 创建当前用户
            m_currentLoginUser = User::loadUserFromDB(m_dbFileName, username, passwordHash);
            // m_currentLoginUser->test();

            mainWidgetInitialization();
            // 先暂时直接全部退出
            // this->deleteLater();
        } else {
            // 密码哈希值不匹配
            qDebug() << "Password hash or UserName does not match!";
            m_login->loginFailedOperation();                // 清除错误用户名和密码
            return;
        }
    } else {
        qDebug() << "Error: Failed to execute query:" << query.lastError().text();
    }



    // if(ui->lineEdit_username->text() == "admin" && ui->lineEdit_password->text() == "123456"){
    //     this->close();
    //     qDebug()<<"hello world";
    // }else{
    //     QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码输入错误！"),QMessageBox::Ok);
    //     ui->lineEdit_username->clear();
    //     ui->lineEdit_password->clear();
    //     ui->lineEdit_username->setFocus();

    // }
}

void Widget::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //选择单元格变化时的响应
    Q_UNUSED(previous);

    if (current.isValid())  //当前模型索引有效
    {
        // labCellPos->setText(QString::asprintf("当前单元格：%d行，%d列",
        //                                       current.row(),current.column()));    //显示模型索引的行和列号
        QStandardItem *aItem=m_model->itemFromIndex(current);  //从模型索引获得Item
        // this->labCellText->setText("单元格内容："+aItem->text()); //显示item的文字

        QFont   font=aItem->font(); //获取item的字体
        // ui->actFontBold->setChecked(font.bold()); //更新actFontBold的复选状态
    }
}




bool Widget::loadTaskSetsTocomboBox()
{
    qDebug()<<"loadTaskSets()";
    if(m_currentLoginUser == nullptr){
        qDebug()<<"当前用户为空！";
        return false;
    }
    ui->comboBox->clear();  // 先清空内容
    int taskSets_num = m_tasksets.size();
    for(int i=0; i<taskSets_num; ++i){
        ui->comboBox->addItem(m_tasksets[i].getName(), m_tasksets[i].getTaskSetId().toString());   // 任务集：名称，id
    }
    return true;

}

void Widget::loadTasksToView(int index)
{
    qDebug()<<"loadTasksToView";
    // 先清空
    int rowCount = m_model->rowCount();
    if(rowCount>0){
        // 删除数据模型中除了表头的所有行数据
        m_model->removeRows(0, rowCount);
    }

    // 根据index找到对应的任务集
    QUuid tasksetId= QUuid::fromString(ui->comboBox->itemData(index).toString());

    QStandardItem   *aItem;
    int i=0;
    for(const Task& tmp_task: m_tasks){
        if(tmp_task.getTaskSetId() == tasksetId){
            // 提交任务到列表窗口
            aItem=new QStandardItem(tmp_task.getName());    //创建item
            aItem->setTextAlignment(Qt::AlignCenter);       // 设置文本居中
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);// 使用自定义的数据角色存储隐藏的 id
            m_model->setItem(i,0,aItem);

            aItem=new QStandardItem(tmp_task.getDescription());
            aItem->setTextAlignment(Qt::AlignCenter);
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);
            m_model->setItem(i,1,aItem);

            QDateTime begin = tmp_task.getTimeRange().getBegin();
            qDebug()<<begin;
            aItem=new QStandardItem(begin.toString("yyyy-MM-dd HH:mm:ss"));
            aItem->setTextAlignment(Qt::AlignCenter);
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);
            m_model->setItem(i,2,aItem);

            QDateTime end = tmp_task.getTimeRange().getEnd();
            qDebug()<<end;
            aItem=new QStandardItem(end.toString("yyyy-MM-dd HH:mm:ss"));
            aItem->setTextAlignment(Qt::AlignCenter);
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);
            m_model->setItem(i,3,aItem);

            QString taskStateFlag;
            switch(tmp_task.getTaskStateFlag()){
            case Unstarted:
                taskStateFlag="未开始";
                break;
            case Ongoing:
                taskStateFlag="进行中";
                break;
            case Finished:
                taskStateFlag="已完成";
                break;
            case Unfinished:
                taskStateFlag="已过期";
                break;
            }
            aItem=new QStandardItem(taskStateFlag);
            aItem->setTextAlignment(Qt::AlignCenter);
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);
            m_model->setItem(i,4,aItem);
            aItem=new QStandardItem(tmp_task.getIsFinished()?"√":"×");
            aItem->setTextAlignment(Qt::AlignCenter);
            aItem->setData(tmp_task.getTaskId().toString(), Qt::UserRole + 1);
            m_model->setItem(i,5,aItem);
            ++i;
        }
    }
    }


void Widget::on_comboBox_currentIndexChanged(int index)
{
    qDebug()<<"on_comboBox_currentIndexChanged";

    loadTasksToView(index);
}

void Widget::do_saveToData(const QModelIndex &index)
{
    qDebug()<<"do_saveToData";
    // 根据index找到item
    QStandardItem *aItem = m_model->itemFromIndex(index);
    // 根据item获取任务id
    QString taskId_str = aItem->data(Qt::UserRole + 1).toString();
    // 根据任务id找到内存中对应的任务对象
    for(Task& task: m_tasks){
        if(task.getTaskId().toString()==taskId_str){
            switch(index.column()){
            case 0:
                task.setName(aItem->text());
                break;
            case 1:
                task.setDescription(aItem->text());
                break;
            case 2:
            {
                QDateTime begin = QDateTime::fromString(aItem->text(), "yyyy-MM-dd HH:mm:ss");
                task.setBegin(begin);
                break;
            }
            case 3:
            {
                QDateTime end = QDateTime::fromString(aItem->text(), "yyyy-MM-dd HH:mm:ss");
                task.setEnd(end);
                break;
            }
            case 4:
                task.updateTaskStateFlag();
                break;
            case 5:
                task.setIsFinished((aItem->text()=="√"?true:false));
                break;
            }
        }
    }

    // 修改任务
}


void Widget::on_pushButton_addTaskSet_clicked()
{
    QString dlgTitle = "输入文字对话框";
    QString txtLabel = "请输入新建任务集的名称";
    QString iniInput = "无标题任务集";
    QLineEdit::EchoMode echoMode = QLineEdit::Normal;                           // 正常文字输入
    bool ok = false;                                                            // 对话框返回参数
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, iniInput, &ok);
    if(ok && !text.isEmpty()){
        TaskSet newTaskSet = TaskSet(text, m_currentLoginUser->getUserId());    // 新建任务集
        m_tasksets.append(newTaskSet);                                          // 使用默认复制构造函数添加到容器中
        // 更新任务集列表
        ui->comboBox->addItem(newTaskSet.getName(), newTaskSet.getTaskSetId().toString());   // 任务集：名称，id

    }


   test();
}


void Widget::on_pushButton_deleteTaskSet_clicked()
{
    DialogDeleteTaskSet* dialog = new DialogDeleteTaskSet(m_tasksets, this);    //创建对话框
    dialog->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    int ret=dialog->exec();   //以模态方式显示对话框，用户关闭对话框时返回 DialogCode值
    if (ret==QDialog::Accepted)     //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    {
        QString taskSetId_str=dialog->getSelectedTaskSetId();
        // 找到id对应的任务集并删除，相应的任务也要删除
        for(int i=0; i<m_tasksets.size(); ++i){
            if(m_tasksets[i].getTaskSetId().toString() == taskSetId_str){
                TaskSet taskSet = m_tasksets[i];
                // 删除属于该任务集及所有任务, 同时同步到数据库
                m_tasksets.remove(i);
                taskSet.deleteSelfFromDB(m_dbFileName);
                for(int j=0; j<m_tasks.size(); ++j){
                    if(m_tasks[j].getTaskSetId().toString()==taskSetId_str){
                        Task task = m_tasks[j];
                        m_tasks.remove(j);
                        task.deleteSelfFromDB(m_dbFileName);
                    }
                }
                // m_tasks.erase(
                //     std::remove_if(m_tasks.begin(), m_tasks.end(), [taskSetId_str](const Task &task) {
                //         return task.getTaskSetId().toString()==taskSetId_str;
                //     }),
                //     m_tasks.end()
                //     );
                loadTaskSetsTocomboBox();   // 更新任务集下拉列表
                break;
            }
        }

    }
    delete dialog;    //删除对话框
}


void Widget::on_pushButton_deleteTask_clicked()
{
    // dialog
    // 获取当前任务集的任务
    QVector<Task> currentTasks;
    QUuid taskSetId = ui->comboBox->currentData().toUuid();
    for(const Task& task: m_tasks){
        if(task.getTaskSetId()==taskSetId){
            currentTasks.append(task);
        }
    }
    // for(const TaskSet& tmp: m_tasksets){
    //     if(tmp.getTaskSetId()==tasksetId){
    //         // 该任务集中的任务id
    //         QVector<QUuid> taskIds = tmp.getTaskids();
    //         // 在所有任务中找到对应任务
    //         for(int i=0; i<taskIds.size(); ++i){
    //             const QUuid& tmp_uuid=taskIds[i];
    //             for(const Task& tmp_task: m_tasks){
    //                 if(tmp_uuid == tmp_task.getTaskId()){
    //                     currentTasks.append(tmp_task);
    //                 }
    //             }
    //         }
    //     }
    // }


    DialogDeleteTask* dialog = new DialogDeleteTask(currentTasks, this);    //创建对话框
    dialog->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    int ret=dialog->exec();   //以模态方式显示对话框，用户关闭对话框时返回 DialogCode值
    if (ret==QDialog::Accepted)     //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    {
        QString taskId_str=dialog->getSelectedTaskId();
        // 找到id对应的任务集并删除
        for(int i=0; i<m_tasks.size(); ++i){
            if(m_tasks[i].getTaskId().toString() == taskId_str){
                Task task = m_tasks[i];
                m_tasks.remove(i);
                task.deleteSelfFromDB(m_dbFileName);
                qDebug()<<"test1";
                // deleteTaskFromTaskSet(QUuid::fromString(taskId_str));   // 更新任务集
                // qDebug()<<"test0";
                loadTasksToView(ui->comboBox->currentIndex());   // 更新任务视图列表
                break;
            }
        }
    }
    delete dialog;    //删除对话框
    // 获取任务id

    // 删除任务，同时 更新当前任务集的任务id

}


void Widget::on_pushButton_addTask_clicked()
{
    // dialog
    DialogAddTask* dialog = new DialogAddTask(this);    //创建对话框
    dialog->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); //设置对话框固定大小
    int ret=dialog->exec();   //以模态方式显示对话框，用户关闭对话框时返回 DialogCode值
    if (ret==QDialog::Accepted)     //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    {
        QString name, description;
        QDateTime begin, end;
        TaskStateFlag state;


        dialog->getNewTask(&name,&description,&begin,&end,&state);
        if(name.size()==0 || description.size()==0){
            qDebug()<<"任务名称或描述不能为空！";
            QMessageBox::warning(this,tr("添加任务失败"),tr("任务名称或描述不能为空！"),QMessageBox::Ok);
            return;
        }
        // 创建新任务
        QUuid taskSetId= ui->comboBox->currentData().toUuid();                      // 任务集id
        Task newTask = Task(name, description, TimeRange(begin, end), state, taskSetId);
        m_tasks.append(newTask);
        // 更新视图列表
        loadTasksToView(ui->comboBox->currentIndex());
    }
    delete dialog;    //删除对话框
}


bool Widget::on_pushButton_clicked()
{
    qDebug()<<"save to db";

    bool flag = true;
    // 保存任务到数据库
    for(const Task& task: m_tasks){
        if(!task.saveToDB(m_dbFileName)){
            qDebug()<<"任务保存失败";
            flag=false;
        }
    }

    // 保存任务集到数据库
    for(const TaskSet& taskSet: m_tasksets){
        if(!taskSet.saveToDB(m_dbFileName)){
            qDebug()<<"任务集保存失败";
            flag=false;
        }
    }
    if(flag){
        QMessageBox::information(this,tr("保存数据到数据库"),tr("用户数据保存成功！"),QMessageBox::Ok);
        return true;
    }else{
        QMessageBox::warning(this,tr("保存数据到数据库"),tr("用户数据保存发生未知错误！"),QMessageBox::Ok);
        return false;
    }


}


void Widget::on_pushButton_2_clicked()
{

    // 先保存数据
    if(on_pushButton_clicked()){
        this->close();
    }
    // 保存失败则不退出
}

