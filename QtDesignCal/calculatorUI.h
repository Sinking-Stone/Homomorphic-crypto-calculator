#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QtWidgets/qtextedit.h>
class calculatorUI : public QDialog
{
	Q_OBJECT
private:

    // 文本框定义，公式编辑区
    QLineEdit* formulaEdit;
    // 结果展示1区
    QTextEdit* resultEdit1;
    // 结果展示2区
    QTextEdit* resultEdit2;

    // 左括号
    QPushButton* bracketLeftBtn;
    // 右括号
    QPushButton* bracketRightBtn;
    // 清空号
    QPushButton* backspaceBtn;
    // 清除
    QPushButton* clearBtn;
    // 数字7
    QPushButton* num7Btn;
    // 数字8
    QPushButton* num8Btn;
    // 数字9
    QPushButton* num9Btn;
    // 除号
    QPushButton* divisionBtn;
    // 数字4
    QPushButton* num4Btn;
    // 数字5
    QPushButton* num5Btn;
    // 数字6
    QPushButton* num6Btn;
    // 乘号
    QPushButton* multiplicationBtn;
    // 数字1
    QPushButton* num1Btn;
    // 数字2
    QPushButton* num2Btn;
    // 数字3
    QPushButton* num3Btn;
    // 减号
    QPushButton* minusBtn;
    // 数字0
    QPushButton* num0Btn;
    // 小数点
    QPushButton* dotBtn;
    // 等号
    QPushButton* equalsBtn;
    // 加号
    QPushButton* plusBtn;
    // 指数运算符
    QPushButton* expBtn;
    // 幂运算符
    QPushButton* indBtn;

    // UI 初始化与创建
    void iniUI();
    // 初始化信号与槽的链接
    void iniSignalSlots();

private slots:

    // 定义各按钮的槽函数
    // 数字 0-9
    void on_numBtn();
    // 左括号
    void on_bracketLeftBtn();
    // 右括号
    void on_bracketRightBtn();
    // 退格键
    void on_backspaceBtn();
    // 清除
    void on_clearBtn();
    // 除号
    void on_divisionBtn();
    // 乘号
    void on_multiplicationBtn();
    // 减号
    void on_minusBtn();
    // 小数点
    void on_dotBtn();
    // 等号
    void on_equalsBtn();
    // 加号
    void on_plusBtn();
    // 指数符号
    void on_expBtn();
    // 幂符号
    void on_indBtn();

public:

    // 公式字符串
    QString formulaStr;
    // 括号的数量
    int bracket_cnt;

    calculatorUI(QWidget* parent = 0);
    ~calculatorUI();
    // 反向匹配str1 和str2，判断str1中是否有str2中的字符
    int LastMatchingBoth(QString& str1, const char* str2);
    // 匹配str1 和str2，判断str1中是否有str2中的字符
    int MatchingBoth(QString& str1, const char* str2);
};

