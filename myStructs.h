#ifndef MYSTRUCTS_H
#define MYSTRUCTS_H

#include<QDateTime>

// 表示开始结束的时间范围
struct TimeRange
{
private:
    QDateTime m_begin;
    QDateTime m_end;
public:
    TimeRange(const QDateTime& begin, const QDateTime& end);
    TimeRange();
    TimeRange(const TimeRange& tr);
    TimeRange(const QString& tr_str);
    ~TimeRange()=default;
    QDateTime getBegin() const;
    QDateTime getEnd() const;

    void setBegin(const QDateTime& begin);
    void setEnd(const QDateTime& end);
    void update(const TimeRange& tr);

    QString toString() const;
};

// 任务的状态标记
enum TaskStateFlag
{
    Unstarted,  // 未开始
    Ongoing,    // 正在进行
    Finished,   // 已完成
    Unfinished  // 未完成（过期）
};

#endif // MYSTRUCTS_H
