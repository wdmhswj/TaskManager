#include "ttextdelegate.h"
#include <QLineEdit>

TTextDelegate::TTextDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *TTextDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //创建代理编辑组件
    Q_UNUSED(option);
    Q_UNUSED(index);

    // QSpinBox *editor = new QSpinBox(parent); //创建一个QSpinBox
    // editor->setFrame(false); //设置为无边框
    // editor->setMinimum(0);
    // editor->setMaximum(50000);

    QLineEdit* editor = new QLineEdit(parent);  // 用于单行文本输入的部件
    editor->setFrame(false); //设置为无边框
    editor->setMinimumSize(QSize(0, 0));
    editor->setMaximumSize(QSize(15000, 50000));

    return editor;  //返回此编辑器
}

void TTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();   //获取数据

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);  //强制类型转换
    lineEdit->setText(value);   //设置编辑器的文本

}

void TTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *spinBox = static_cast<QLineEdit*>(editor);     //强制类型转换
    //    spinBox->interpretText();       //解释数据，如果数据被修改后，就触发信号
    QString value = spinBox->text();   //获取spinBox的值
    model->setData(index, value, Qt::EditRole);     //更新到数据模型
}

void TTextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
