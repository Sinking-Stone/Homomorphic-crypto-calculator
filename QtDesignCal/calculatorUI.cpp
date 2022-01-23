#include "calculatorUI.h"
#include <iostream>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "calculator.h"

using namespace std;

calculatorUI::calculatorUI(QWidget* parent) : QDialog(parent) {
    // 设置窗口最大化或最小化窗口
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    // 记录括号的数量
    bracket_cnt = 0;
    // 界面创建与布局
    iniUI();
    // 信号与槽的关联
    iniSignalSlots();
    // 设置窗口的标题
    setWindowTitle("Calculator");
}
calculatorUI::~calculatorUI() {

}

/**
 * iniUI()函数主要负责整体UI界面的设计,按每一排进行设计,在窗口缩放时按钮可以随界面大小而改变.
 *
 * iniSignalSlots()函数是将按钮中的表示的功能与按钮进行绑定,点击按钮时会执行相应的函数功能.
 *
 * on_numBtn()函数是将按钮上的文本输入到公式表达式输入框中,即0-9输入到公式表达式输入框中.
 *
 * on_bracketLeftBtn(),on_bracketRightBtn(),on_backspaceBtn(),on_clearBtn(),
 * on_divisionBtn(),on_multiplicationBtn(),on_minusBtn(),on_dotBtn()
 * on_equalsBtn(),on_plusBtn()这11个函数的功能分别是在公式表达式输入框中输入:左括号,
 * 右括号,删除一个字符,清空所有字符,除号,乘号,减号或负号,小数点,等于号,加号. 这些函数功能
 * 之所以写的比较复杂就是为了能让用户输入合法的表达式,方便在计算的时候没有错误.
 *
 * LastMatchingBoth()函数与MatchingBoth()函数是辅助函数,前者是反向匹配参数中的两个字符串
 * 后者的功能是正向匹配参数中的两个字符串.
 */

void calculatorUI::iniUI() {
    // 设置button样式
    setStyleSheet(
        //正常状态样式
        "QPushButton{"
        "background-color:rgba(172,4,244,100);"     //背景色（也可以设置图片）
        "border-style:outset;"                     //边框样式（inset/outset）
        "border-width:4px;"                        //边框宽度像素
        "border-radius:10px;"                      //边框圆角半径像素
        "border-color:rgba(255,255,255,30);"       //边框颜色
        "font:bold 40px;"                          //字体，字体大小
        "color:rgba(0,0,0,100);"                   //字体颜色
        "padding:6px;"                             //填衬
        "}"
        //鼠标按下样式
        "QPushButton:pressed{"
        "font:bold 55px;"
        "}"
        //鼠标悬停样式
        "QPushButton:hover{"
        "background-color:rgba(137,4,194,200);"
        "border-color:rgba(255,255,255,200);"
        "color:rgba(0,0,0,200);"
        "}"
    );

    // 固定窗口大小
    // setFixedSize(574,700);
    // 创建文本框，用于输入用户计算的字符串
    formulaEdit = new QLineEdit();
    formulaEdit->setStyleSheet(
        "border:2px solid mediumvioletred ;"
        "border-radius: 10px;"
        "min-height:40px;"
        "font-family:微软雅黑; font-size: 30px;"
    );
    // 设置显示字符串向右边对齐
    formulaEdit->setAlignment(Qt::AlignRight);
    // 设置对话框不能输入
    formulaEdit->setReadOnly(true);
    // 将计算的结果保存在这个文本框中
    resultEdit1 = new QTextEdit();
    resultEdit1->setStyleSheet(
        "border:2px solid mediumvioletred ;"
        "border-radius: 10px;min-height:40px;"
        "font-family:微软雅黑; font-size: 30px;"
    );
    resultEdit1->setPlaceholderText(u8"结果及其密文显示区");
    // 设置显示字符串向右边对齐
    resultEdit1->setAlignment(Qt::AlignRight);
    // 设置对话框不能输入
    resultEdit1->setReadOnly(true);

    // 将计算公式的结果保存在这个文本框中
    resultEdit2 = new QTextEdit();
    resultEdit2->setStyleSheet(
        "border:2px solid mediumvioletred ;"
        "border-radius: 10px;min-height:40px;"
        "font-family:微软雅黑; font-size: 30px;"
    );
    resultEdit2->setPlaceholderText(u8"公式密文显示区");
    // 设置显示字符串向右边对齐
    resultEdit2->setAlignment(Qt::AlignRight);
    // 设置对话框不能输入
    resultEdit2->setReadOnly(true);

    // 逐行创建QPushButton，并水平布局
    // 左括号
    bracketLeftBtn = new QPushButton(tr("("));
    // 右括号
    bracketRightBtn = new QPushButton(tr(")"));
    // 清空一个
    backspaceBtn = new QPushButton(tr("<-"));
    // 清除
    clearBtn = new QPushButton(tr("C"));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay1 = new QHBoxLayout;
    HLay1->addWidget(bracketLeftBtn);
    HLay1->addWidget(bracketRightBtn);
    HLay1->addWidget(backspaceBtn);
    HLay1->addWidget(clearBtn);

    // 数字7
    num7Btn = new QPushButton(tr("7"));
    // 数字8
    num8Btn = new QPushButton(tr("8"));
    // 数字9
    num9Btn = new QPushButton(tr("9"));
    // 除号
    divisionBtn = new QPushButton(tr("/"));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay2 = new QHBoxLayout;
    HLay2->addWidget(num7Btn);
    HLay2->addWidget(num8Btn);
    HLay2->addWidget(num9Btn);
    HLay2->addWidget(divisionBtn);

    // 数字4
    num4Btn = new QPushButton(tr("4"));
    // 数字5
    num5Btn = new QPushButton(tr("5"));
    // 数字6
    num6Btn = new QPushButton(tr("6"));
    // 乘号
    multiplicationBtn = new QPushButton(tr("*"));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay3 = new QHBoxLayout;
    HLay3->addWidget(num4Btn);
    HLay3->addWidget(num5Btn);
    HLay3->addWidget(num6Btn);
    HLay3->addWidget(multiplicationBtn);

    // 数字1
    num1Btn = new QPushButton(tr("1"));
    // 数字2
    num2Btn = new QPushButton(tr("2"));
    // 数字3
    num3Btn = new QPushButton(tr("3"));
    // 减号
    minusBtn = new QPushButton(tr("-"));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay4 = new QHBoxLayout;
    HLay4->addWidget(num1Btn);
    HLay4->addWidget(num2Btn);
    HLay4->addWidget(num3Btn);
    HLay4->addWidget(minusBtn);

    // 数字0
    num0Btn = new QPushButton(tr("0"));
    // 指数运算符
    expBtn = new QPushButton(tr("e"));
    // 幂运算符
    indBtn = new QPushButton(tr("^"));
    // 加号
    plusBtn = new QPushButton(tr("+"));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay5 = new QHBoxLayout;
    HLay5->addWidget(num0Btn);
    HLay5->addWidget(expBtn);
    HLay5->addWidget(indBtn);
    HLay5->addWidget(plusBtn);

    // 小数点
    dotBtn = new QPushButton(tr("."));
    // 等号
    equalsBtn = new QPushButton(tr("="));
    // QHBoxLayout: 在水平的方向上排列控件 左右排列
    QHBoxLayout* HLay6 = new QHBoxLayout;
    HLay6->addWidget(dotBtn);
    HLay6->addWidget(equalsBtn);

    // 创建垂直布局，并设置为主布局
    // QVBoxLayout:在垂直的方向上排列控件 上下排列
    QVBoxLayout* VLay = new QVBoxLayout;
    VLay->addWidget(formulaEdit);
    VLay->addWidget(resultEdit1);
    VLay->addWidget(resultEdit2);
    VLay->addLayout(HLay1);
    VLay->addLayout(HLay2);
    VLay->addLayout(HLay3);
    VLay->addLayout(HLay4);
    VLay->addLayout(HLay5);
    VLay->addLayout(HLay6);
    setLayout(VLay);
}

// 将信号与槽进行绑定
void calculatorUI::iniSignalSlots() {

    // connect表示信号与槽的连接，第一个参数是信号发送的对象指针
    // 第二个参数是要发送的对象信号函数，可以使用SIGNAL宏进行类型转换
    // 第三个参数是信号接收的对象指针，第四个参数是接收信号后要执行的槽函数，可以使用SLOT宏进行类型转换
    // 左括号按钮点击后执行on_bracketLeftBtn操作，余下操作均相似
    connect(bracketLeftBtn, SIGNAL(clicked()), this, SLOT(on_bracketLeftBtn()));
    connect(bracketRightBtn, SIGNAL(clicked()), this, SLOT(on_bracketRightBtn()));
    connect(backspaceBtn, SIGNAL(clicked()), this, SLOT(on_backspaceBtn()));
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(on_clearBtn()));

    connect(num7Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num8Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num9Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(divisionBtn, SIGNAL(clicked()), this, SLOT(on_divisionBtn()));

    connect(num4Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num5Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num6Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(multiplicationBtn, SIGNAL(clicked()), this, SLOT(on_multiplicationBtn()));

    connect(num1Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num2Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(num3Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(minusBtn, SIGNAL(clicked()), this, SLOT(on_minusBtn()));

    connect(num0Btn, SIGNAL(clicked()), this, SLOT(on_numBtn()));
    connect(dotBtn, SIGNAL(clicked()), this, SLOT(on_dotBtn()));
    connect(equalsBtn, SIGNAL(clicked()), this, SLOT(on_equalsBtn()));
    connect(plusBtn, SIGNAL(clicked()), this, SLOT(on_plusBtn()));


    connect(expBtn, SIGNAL(clicked()), this, SLOT(on_expBtn()));
    connect(indBtn, SIGNAL(clicked()), this, SLOT(on_indBtn()));
}

// 函数作用：将用户输入的文本在文本框中显示出来，主要是数字内容的显示
void calculatorUI::on_numBtn() {
    // sender()转换成QPushButton类型的对象
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // formulaStr的数据来自于formulaEdit中的数据
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 刚开始的时候，不允许输入‘）’
    if ((tmp.length() && tmp[0] == ')') || (tmp.length() && tmp[0] == 'e'))
    {
        return;
    }
    // 将按钮上的字符，插入到公式文本的最后一个位置上
    formulaEdit->insert(btn->text());
}

// 函数作用：插入左括号
void calculatorUI::on_bracketLeftBtn() {
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // formulaStr的数据来自于formulaEdit中的数据
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);

    // 匹配tmp 和 ")0123456789." ，判断tmp中是否有")0123456789."中的字符
    // 要是tmp右边的一位有")0123456789."这一字符串中的任意一个则不进行插入操作
    if (tmp.length())
    {
        if (MatchingBoth(tmp, ")0123456789.") != -1)
        {
            return;
        }
    }

    tmp = formulaStr.right(2);
    if (tmp.length() == 2) {
        // 前面不能有连续两次加减乘除
        if (tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '*' || tmp[0] == '/' || tmp[0] == '(')
        {
            if (tmp[1] == '+' || tmp[1] == '-' || tmp[1] == '*' || tmp[1] == '/') {
                return;
            }
        }
    }
    // 在表达式中插入左括号
    formulaEdit->insert("(");
    // 括号数量加一
    bracket_cnt++;
}

// 函数作用：插入右括号
void calculatorUI::on_bracketRightBtn() {

    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 若是前面没有括号(即左括号),无法进行插入右括号
    if (bracket_cnt == 0)
    {
        return;
    }

    // 如果要插入右括号的前面是"+-*/^.(",这些对运算没有意义的符号,则不进行插入右括号
    if (tmp.length())
    {
        if (MatchingBoth(tmp, "+-*/^.(") != -1)
        {
            return;
        }
    }
    // 前面字符串长度为0,也无法进行插入右括号
    else
    {
        return;
    }
    formulaEdit->insert(")");
    // 保证走括号的数量与右括号的数量是一样的
    bracket_cnt--;
}

// 函数作用：删除一个字符
void calculatorUI::on_backspaceBtn() {
    // 要是字符串不为空,就进行删除
    if (formulaEdit->text() != "")
    {
        formulaEdit->backspace();
    }
    if (resultEdit1->toPlainText() != "") {
        resultEdit1->clear();
        resultEdit2->clear();
    }
}

// 函数作用：清空所有的字符串
void calculatorUI::on_clearBtn() {

    // 只要满足公式字符串和结果输出字符串不为空都对所有的数据进行清空
    if (formulaEdit->text() != "" || resultEdit1->toPlainText() != "")
    {
        formulaStr.clear();
        formulaEdit->clear();
        resultEdit1->clear();
        resultEdit2->clear();
    }
}

// 函数作用：插入除法符号
void calculatorUI::on_divisionBtn() {
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 如果要插入除号的前面是"(.+-*/^",这些对运算没有意义的符号,则不进行插入除号
    if (tmp.length())
    {
        if (MatchingBoth(tmp, "(.+-*/^") != -1)
        {
            return;
        }
    }
    // 前面字符串长度为0,也无法进行插入除号
    else
    {
        return;
    }
    // 以上条件都不满足才插入除号
    formulaEdit->insert("/");
}

// 函数作用：插入乘法符号
void calculatorUI::on_multiplicationBtn()
{
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 如果要插入乘号的前面是"(.+-*/^",这些对运算没有意义的符号,则不进行插入乘号
    if (tmp.length())
    {
        if (MatchingBoth(tmp, "(.+-*/^") != -1)
        {
            return;
        }
    }
    // 前面字符串长度为0,也无法进行插入乘号
    else
    {
        return;
    }
    // 以上条件都不满足才插入乘号
    formulaEdit->insert("*");
}


// 函数作用：插入减法符号
void calculatorUI::on_minusBtn()
{
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 减号或者负号前面不能有'.'
    if ((tmp.length() && tmp[0] == '.') || (tmp.length() && tmp[0] == '^'))
    {
        return;
    }

    tmp = formulaStr.right(2);
    if (tmp.length() == 2) {
        // 前面不能有连续两次加减乘除
        if (tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '^' || tmp[0] == '*' || tmp[0] == '/' || tmp[0] == '(')
        {
            if (tmp[1] == '+' || tmp[1] == '-' || tmp[1] == '*' || tmp[1] == '/' || tmp[1] == '^') {
                return;
            }
        }
    }
    // 以上条件都不满足才插入减号或负号
    formulaEdit->insert("-");
}

// 函数作用：插入小数点
void calculatorUI::on_dotBtn()
{
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);

    if (tmp.length())
    {
        // 小数点前只能是数字
        if (MatchingBoth(tmp, "0123456789") == -1)
        {
            // 没有找到数字
            return;
        }
    }
    // 要是前面没有任何的符号(即:是开头的第一位),则插入'0.',作为小数存在
    else
    {
        formulaEdit->insert("0.");
        return;
    }
    // 反向查找,查看formulaStr在插入之前是不是含有"+-*/.()"符号
    int pos = LastMatchingBoth(formulaStr, "+-*/.^()");
    // pos要是不等于-1,证明formulaStr中有"+-*/.()",则不进行插入
    if (pos != -1 && formulaStr[pos] == '.')
    {
        return;
    }
    // 以上条件都不满足才插入小数点
    formulaEdit->insert(".");
}

// 函数作用：等于号功能
void calculatorUI::on_equalsBtn()
{
    // 清空操作
    resultEdit1->clear();
    resultEdit2->clear();
    // 首先定义result为空
    QString result = "";
    // 定义公式表达式为空
    QString equation="";
    // 清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 引入Calculator函数,进行计算
    calculator calculator;
    // 返回结果保存在result中
    result = calculator.Result(formulaStr, equation);
    qDebug() <<u8"表达式: " << equation;
    // 若返回结果为Error,则进行弹窗警告
    if (result == "Error")
    {
        QMessageBox::warning(this, "Warning", "Error!", QMessageBox::Yes);
    }
    // 若是除数为0,进进行弹窗警告
    else if (result == NULL)
    {
        QMessageBox::warning(this, "Warning", "Divisor cannot be zero!", QMessageBox::Yes);
    }
    // 其他没有错误的情况则插入等于号,并在结果框中显示结果
    else
    {
        // formulaEdit->insert("=");
        resultEdit1->setPlainText(result);
        resultEdit2->setPlainText(equation);
    }
}

// 函数作用：插入加法符号
void calculatorUI::on_plusBtn()
{
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 加号前面不能有'.'
    if ((tmp.length() && tmp[0] == '.') || (tmp.length() && tmp[0] == '^'))
    {
        return;
    }
    // 在输入加号之前表达式中必须有其他符号(数字)
    else if (tmp.length() == 0)
    {
        return;
    }

    tmp = formulaStr.right(2);
    if (tmp.length() == 2) {
        // 前面不能有连续两次加减乘除
        if (tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '*' || tmp[0] == '/' || tmp[0] == '^' || tmp[0] == '(')
        {
            if (tmp[1] == '+' || tmp[1] == '-' || tmp[1] == '*' || tmp[1] == '/' || tmp[1] == '^') {
                return;
            }
        }
    }
    // 以上条件都不满足才插入加号
    formulaEdit->insert("+");
}

// 函数作用：反向匹配str1 和str2，判断str1中是否有str2中的字符
int calculatorUI::LastMatchingBoth(QString& str1, const char* str2)
{
    // 循环匹配，要是匹配上，则返回匹配的位置，没有匹配则返回-1
    for (int i = str1.length(); i >= 0; i--)
    {
        for (unsigned int j = 0; j < strlen(str2); j++)
        {
            if (str1[i] == str2[j])
            {
                return i;
            }
        }
    }
    return -1;
}

// 函数作用：匹配str1 和str2，判断str1中是否有str2中的字符
int calculatorUI::MatchingBoth(QString& str1, const char* str2)
{
    // 循环匹配，要是匹配上，则返回匹配的位置，没有匹配则返回-1
    for (int i = 0; i < str1.length(); i++)
    {
        for (unsigned int j = 0; j < strlen(str2); j++)
        {
            if (str1[i] == str2[j])
            {
                return i;
            }
        }
    }
    return -1;
}

// 以e为底的指数符号
void calculatorUI::on_expBtn() {

    // 清空操作
    resultEdit1->clear();
    resultEdit2->clear();
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);

    // 如果要插入乘号的前面是"(+-*/",这些对运算才进行插入
    if (tmp.length())
    {
        if (MatchingBoth(tmp, "(+-*/") == -1)
        {
            return;
        }
    }
    // 以上条件都不满足才插入e
    formulaEdit->insert("e");
}

// 幂符号
void calculatorUI::on_indBtn() {
    // 首先清空formulaStr中的内容
    formulaStr.clear();
    // 获取当前文本框中的内容
    formulaStr = formulaEdit->text();
    // 获取最右边一个子串
    QString tmp = formulaStr.right(1);
    // 如果要插入乘号的前面是"(.+-*/^",这些对运算没有意义的符号,则不进行插入乘号
    if (tmp.length())
    {
        if (MatchingBoth(tmp, "(.+-*/^") != -1)
        {
            return;
        }
    }
    // 前面字符串长度为0,也无法进行插入乘号
    else
    {
        return;
    }
    // 以上条件都不满足才插入乘号
    formulaEdit->insert("^");
}










