#ifndef M_LINEEDIT_H
#define M_LINEEDIT_H
#include <QLineEdit>
#include <QMouseEvent>


class m_lineEdit :public QLineEdit
{
    Q_OBJECT
public:
    m_lineEdit();

    explicit m_lineEdit(QWidget *parent = nullptr);
protected:
    //重写mousePressEvent事件
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    //自定义clicked()信号,在mousePressEvent事件发生时触发
    void clicked();

};

#endif // M_LINEEDIT_H
