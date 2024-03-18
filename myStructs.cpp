#include"myStructs.h"

TimeRange::TimeRange(const QDateTime& begin, const QDateTime& end): m_begin(begin),m_end(end){}

TimeRange::TimeRange()
{
    m_begin=QDateTime::currentDateTime();
}
TimeRange::TimeRange(const TimeRange& tr)
{
    m_begin=tr.getBegin();
    m_end=tr.getEnd();
}

TimeRange::TimeRange(const QString &tr_str)
{
    QStringList beginAndEnd = tr_str.split("-to-");
    m_begin = QDateTime::fromString(beginAndEnd[0]);
    m_end = QDateTime::fromString(beginAndEnd[1]);

}


QDateTime TimeRange::getBegin() const
{
    return m_begin;
}
QDateTime TimeRange::getEnd() const
{
    return m_end;
}

void TimeRange::setBegin(const QDateTime &begin)
{
    m_begin=begin;
}

void TimeRange::setEnd(const QDateTime &end)
{
    m_end=end;
}

void TimeRange::update(const TimeRange &tr)
{
    m_begin=tr.getBegin();
    m_end=tr.getEnd();
}

QString TimeRange::toString() const
{
    return m_begin.toString()+"-to-"+m_end.toString();
}
