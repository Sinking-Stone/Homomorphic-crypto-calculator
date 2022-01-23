#pragma once

#include <QString>
#include <QStack>
#include <QQueue>


class calculator
{

private:
    // 分离中缀
    QQueue<QString> Split(const QString& formulaStr);
    // 将中缀队列转换为后缀队列
    QQueue<QString> Transfer(QQueue<QString>& infix);
    // 将后缀队列计算出结果
    QString Calculate(QQueue<QString>& formulaStr, QString& equation);

    QString Calculate(QString& l, QString& op, QString& r);
    QString ValidNum(QString str);
    QString Cryptograph(QString str);



public:

    QString Result(QString& formulaStr, QString& equation);


};

