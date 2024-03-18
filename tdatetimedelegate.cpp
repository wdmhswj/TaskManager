#include "tdatetimedelegate.h"
#include <QDateTimeEdit>

tDateTimeDelegate::tDateTimeDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *tDateTimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QDateTimeEdit *editor = new QDateTimeEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd HH:mm:ss"); // 设置日期时间格式
    return editor;
}

void tDateTimeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateTime value = index.model()->data(index, Qt::EditRole).toDateTime();
    QDateTimeEdit *dateTimeEdit = static_cast<QDateTimeEdit *>(editor);
    dateTimeEdit->setDateTime(value);
}

void tDateTimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *dateTimeEdit = static_cast<QDateTimeEdit *>(editor);
    QDateTime value = dateTimeEdit->dateTime();
    model->setData(index, value.toString("yyyy-MM-dd HH:mm:ss"), Qt::EditRole);
}

void tDateTimeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
