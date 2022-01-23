#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtdesigncal.h"
#include <qpushbutton.h>
#include <qtextedit.h>
#include "calculatorUI.h"



class QtDesignCal : public QWidget
{
    Q_OBJECT

public:
    QtDesignCal(QWidget *parent = Q_NULLPTR);

    calculatorUI cal;

public slots:
    void jumpCal();
    void readTxt();


private:
    Ui::QtDesignCalClass ui;
    // 自动生成密钥的按钮
    QPushButton* automaticBtn;
    // 手动生成密钥的按钮
    QPushButton* manualBtn;
    // 密文输入窗口
    QTextEdit* cipherText1;
    // 公钥显示窗口
    QTextEdit* cipherText2;
    // UI 初始化与创建
    void iniUI();
    void select();

signals:
    void SendData(QString);
};
