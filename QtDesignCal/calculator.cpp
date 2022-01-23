#include "calculator.h"
#include <typeinfo>
#include <QString>
#include <QDebug>
#include "operation.cpp"
#include "qtdesigncal.h"
#include <fstream>

#define CIPHER_LEN 10

extern bool isCipher;
extern QString path;


QString cipherTxt = "";
vector<QString> inputNum;


// 分离中缀,formulaStr表示的是整个从formulaEdit中传来的表达式
QQueue<QString> calculator::Split(const QString& formulaStr)
{
    inputNum.clear();
    // 中缀队列
    QQueue<QString> infix;
    // 数字
    QString num = "";
    // 循环遍历公式表达式中的每一个字符
    for (int i = 0; i < formulaStr.length(); i++)
    {
        // 判断小数点和数字，连续的小数点和数字组成一个数
        if ((formulaStr[i] == '.') || ((formulaStr[i] >= '0') && (formulaStr[i] <= '9')))
        {
            num += formulaStr[i];
        }
        // 判断括号和乘除号，若是则数字已完,将数入队,再将运算符入队
        else if (formulaStr[i] == '(' || formulaStr[i] == ')'
            || formulaStr[i] == '*' || formulaStr[i] == '/' || formulaStr[i] == '^')
        {
            // 若数字不为空，将数字入队infix中，并清空num中的内容
            if (!num.isEmpty())
            {
                infix.enqueue(num);
                num.clear();
            }
            // 将运算符入队
            infix.enqueue(formulaStr[i]);
        }
        // 判断加减号，特殊处理
        else if (formulaStr[i] == '+' || formulaStr[i] == '-')
        {
            // 表达式开头，是正负号
            if (i == 0)
            {
                num += formulaStr[i];
            }
            // 前面是左括号或者运算符，是正负号
            else if (formulaStr[i - 1] == '(' || formulaStr[i - 1] == '+' || formulaStr[i - 1] == '-' ||
                formulaStr[i - 1] == '*' || formulaStr[i - 1] == '/' || formulaStr[i - 1] == '^')
            {
                num += formulaStr[i];
            }
            // 否则是加减运算符
            else
            {
                // 若数字不为空，将数字入队infix中，并清空num中的内容
                if (!num.isEmpty())
                {
                    infix.enqueue(num);
                    num.clear();
                }
                // 将运算符入队
                infix.enqueue(formulaStr[i]);
            }
        }
        // e要特殊处理
        else if (formulaStr[i] == 'e') {
            infix.enqueue(formulaStr[i]);
        }
    }

    // 遍历完成，判断是否还有数字，若有数字则继续入队，并清空num数值
    if (!num.isEmpty())
    {
        infix.enqueue(num);
        num.clear();
    }
    // 查看infix中的元素
    // qDebug()<<infix;
    // 最后返回队列,队列中元素是用户输入的中缀表达式中的每一个元素
    for (QString x: infix)
    {
        inputNum.push_back(x);
    }
    return infix;
}

// 将中缀队列转换为后缀队列,infix中的元素是中缀表达式中的每一个元素,返回的每一个元素还是字符串
QQueue<QString> calculator::Transfer(QQueue<QString>& infix)
{
    // 栈
    QStack<QString> stack;
    // 后缀队列
    QQueue<QString> suffix;
    // 表示是否转换成功
    bool num_ok;
    QString tmp;
    //当中缀队列不空时
    while (!infix.isEmpty())
    {
        // 出队列保存在tmp中
        tmp = infix.dequeue();
        // toDouble将字符串转化为双精度值的数值,并将返回的结果保存在num_ok中
        // 由于没有接受的结果,所以重在判断tmp中的数字能不能转化成符合要求的数字
        tmp.toDouble(&num_ok);
        // 若是数值,则将infix中的数值元素入栈
        if (num_ok == true || tmp == 'e')
        {
            stack.push(tmp);
        }
        // 若不是数值,而是+,-运算符,则将
        else if (tmp == "+" || tmp == "-")
        {
            // 取出栈顶运算符并入队列
            while (!stack.isEmpty() && (stack.top() != "("))
            {
                suffix.enqueue(stack.pop());
            }
            // +或-运算符也要入栈
            stack.push(tmp);
        }
        // 若不是数值,而是*,/,^运算符,
        else if (tmp == "*" || tmp == "/" || tmp == "^")
        {
            // 若栈不为空,并且栈顶不是(,+,-等运算符,则取出栈顶元素并入队列
            while (!stack.isEmpty() && (stack.top() != "(") &&
                (stack.top() != "+") && (stack.top() != "-"))
            {
                suffix.enqueue(stack.pop());
            }
            // *或/运算符也要入栈
            stack.push(tmp);
        }
        // 若tmp为(,也将其入栈
        else if (tmp == "(")
        {
            stack.push(tmp);
        }
        // 若tmp为),也将其入栈
        else if (tmp == ")")
        {
            // 当栈不为空,且栈顶不为(,则取出栈顶运算符并入队列
            while (!stack.isEmpty() && (stack.top() != "("))
            {
                suffix.enqueue(stack.pop());
            }
            //若栈顶为(,即证明()内无内容,直接舍弃即可
            if (stack.top() == "(")
            {
                stack.pop();
            }
        }
    }
    // 遍历完成，判断栈里是否为空,有元素取出栈顶运算符并入队列
    while (!stack.isEmpty() && (stack.top() != "("))
    {
        suffix.enqueue(stack.pop());
    }
    return suffix;
}


// 函数作用: 检查是否为合法数据
QString calculator::ValidNum(QString str)
{
    QString num;
    // 判断是否小数,不是小数直接返回结果
    if (str.indexOf(".") == -1)
    {
        return Cryptograph(str);
    }
    // 若是小数,需要避免0被去掉
    while (str.length() > 1)
    {
        num = str.right(1);
        // 对于小数若末尾为小数点或0则去掉
        if (num == "." || num == "0")
        {
            // chop(n)函数去掉末尾的n个字符
            str.chop(1);
            // 若是小数点,证明已经清除完毕,是一个万丈的数据了,可直接返回
            if (num == ".")
            {
                return Cryptograph(str);
            }
        }
        // 若是不满足以上情况,证明是一个完整的数据,可直接返回
        else
        {
            return Cryptograph(str);
        }
    }
    return Cryptograph(str);
}

// 函数作用:将传来的数据进行计算
QString calculator::Calculate(QString& l, QString& op, QString& r)
{
    // 单项加密的所需要的参数
    IntArray<unsigned __int64> cipher;
    vector<double> input;
    qDebug() << "ccccccc";
    // 定义数据:运算符左边的,运算符右边的,结果
    double left, right, result;
    // 要返回的结果字符串
    QString resultStr = "";
    // 将字符串l转化为双精度值的数值left
    if (l == 'e') left = 2.718282;
    else left = l.toDouble();
    // 将字符串r转化为双精度值的数值right
    if (r == 'e') right = 2.718282;
    else right = r.toDouble();
    result = 0;

    if (l == 'e' && op == "^") {
        expOperation(right, input, cipher);
        result= input[0];
    }
    else if (op == "+")
    {
        input.push_back(left);
        input.push_back(right);
        addOperation(input, cipher);
        result = input[0];
    }
    else if (op == "-")
    {
        input.push_back(left);
        input.push_back(right);
        subOperation(input, cipher);
        result = input[0];
    }
    else if (op == "*")
    {
        input.push_back(left);
        input.push_back(right);
        mulOperation(input, cipher);
        result = input[0];
        
    }
    else if (op == "/")
    {
        if ((right > (-0.000000000000001)) && (right < (0.000000000000001)))
        {
            return NULL;
        }
        else
        {
            input.push_back(left);
            input.push_back(right);
            divOperation(input, cipher);
            result = input[0];
        }
    }
    else if (op == "^")
    {
        if (right == int(right)) {
            indOperation(left, right, input, cipher);
            result = input[0];
        }
        else return NULL;
    }
    cipherTxt = "";
    // 定义密文长度
    for (int i = 0; i < CIPHER_LEN; i++)
    {
        unsigned __int64 numStr = cipher[i];
        cipherTxt += QString::number(numStr);
    }
    qDebug() << "dddd";
    // QString::number返回与数字n等价的字符串，该字符串根据指定的格式和精度进行格式化
    resultStr = QString::number(result, 10, 6);
    return resultStr;
}

QString calculator::Cryptograph(QString str)
{
    str += u8"\n加密结果为: " + cipherTxt;
    return str;
}


// 函数作用:将后缀表达式的结果,通过调用其他结果的方式返回给出结果
QString calculator::Calculate(QQueue<QString>& suffix, QString& equation)
{
    qDebug() << "aaaaaaaaaaaaaaaa";
    equation = "";
    // 创建公式密文
    IntArray<unsigned __int64> cipher;

    // 创建一个栈
    QStack<QString> stack;
    // 定义公式中左操作数,右操作数,运算符,结果
    QString tmp, L, R, op, result;
    // 表示数字是否转换成功
    bool num_ok;

    // 当传来的后缀队列不为空时一直读取,直到为空
    while (!suffix.isEmpty())
    {
        // 读取队列中元素
        tmp = suffix.dequeue();
        tmp.toDouble(&num_ok);

        // 若是数字,则表示转换成功,将其压入栈中
        if (num_ok == true || tmp == 'e')
        {
            stack.push(tmp);
        }
        // 若不是数字,则表示转换失败,表示的是操作符
        else
        {
            // 要是有两个操作符表示不合法
            if (stack.size() < 2)
            {
                return "Error";
            }
            // 从栈中弹出两个数字,分别为左操作数和右操作数
            R = stack.pop();
            L = stack.pop();
            // 调用方法计算具体的运算
            result = Calculate(L, tmp, R);
            // 若运算结果为空,则退出整个运算
            if (result == NULL)
            {
                // cout<<"Calculate"<<endl;
                return result;
            }
            // 运算结果没有错误,则将结果压入栈中
            stack.push(result);
        }
    }
    qDebug() << "bbbbbbbbbbb";
    QString equationTxt = "";
    for (vector<QString>::iterator it = inputNum.begin(); it != inputNum.end(); it++)
    {
        tmp = *it;
        double d=tmp.toDouble(&num_ok);
        double e = 2.718282;
        if (num_ok == true) {
            aSingleEncryption(d, cipher);
            for (int j = 0; j < CIPHER_LEN; j++)
            {
                unsigned __int64 numStr = cipher[j];
                equationTxt += QString::number(numStr);
            }
        }
        else if (num_ok == false && tmp=='e') {
            aSingleEncryption(e, cipher);
            for (int j = 0; j < CIPHER_LEN; j++)
            {
                unsigned __int64 numStr = cipher[j];
                equationTxt += QString::number(numStr);
            }
        } 
        else {
            equationTxt += tmp;
        }
    }
    qDebug() <<"sdfghjkl" << equationTxt;
    // 将所有数字读入之后,进行加密
    // aSingleEncryption(inputNum, cipher);

    qDebug() << "equation: " << equation;
    equation = equationTxt;

    // 将上面的结果运算完之后栈中只剩下一个结果,即为运算的结果
    if (stack.size() == 1)
    {
        // 将结果弹栈,保存在str中
        QString str = stack.pop();
        // 调用ValidNum函数进行整理去掉无用的0和.,返回最终结果
        return ValidNum(str);
    }
    // 若是栈中没有结果,则为错误
    else
    {
        return "Error";
    }
}

// 相当于是程序的入口,因为在CalculatorDialog.cpp中调用了这个函数,直接进行计算
QString calculator::Result(QString& formulaStr, QString& equation)
{
    
    EncryptionParameters parms(scheme_type::CKKS);

    size_t poly_modulus_degree = 16384;
    poly_modulus_degreePt = &poly_modulus_degree;
    // 设置多相式模数度数
    parms.set_poly_modulus_degree(poly_modulus_degree);
    // 设定系数模, 应与16384相对应
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 40, 40, 40, 40, 40, 60 }));
    parmsPt = &parms;

    double scale = pow(2.0, 40);
    scalePt = &scale;
    // 定义一个产生器
    auto context = SEALContext::Create(parms);
    contextPt = &context;
    // 创建一个钥匙生成器
    KeyGenerator keygen(context);
    keygenPt = &keygen;

    qDebug() << isCipher;


    auto public_key = keygen.public_key();

    auto secret_key = keygen.secret_key();

    auto relin_keys = keygen.relin_keys_local();

    Encryptor encryptor(context, public_key);

    Evaluator evaluator(context);

    Decryptor decryptor(context, secret_key);

    CKKSEncoder encoder(context);

    if (path != "") {
        filebuf buf;
        if (buf.open(path.toStdString(), ios::in | ios::binary) == nullptr) {
            return "ERROR";
        }
        istream is(&buf);
        secret_key.load(context, is);
    }
    KeyGenerator keygen2(context, secret_key);

    auto public_key2 = keygen2.public_key();

    auto relin_keys2 = keygen2.relin_keys_local();

    Encryptor encryptor2(context, public_key2);

    Evaluator evaluator2(context);

    Decryptor decryptor2(context, secret_key);

    CKKSEncoder encoder2(context);

    if (isCipher == false) {
        
        public_keyPt = &public_key;
        // 创造一个私钥
        secret_keyPt = &secret_key;
        // 创建一个线性密钥
        relin_keysPt = &relin_keys;
        // 构造Encryptor的一个实例，为了能够加密，相当于创建一个加密机
        encryptorPt = &encryptor;
        // 对密文的计算使用Evaluator类执行，对密文的计算使用Evaluator类执行,Evaluator不会由持有密钥的同一方构造
        // 创建一个评价机，评价当前精度
        evaluatorPt = &evaluator;
        // 构造Decryptor类的一个实例，创建一个解密机
        // 希望对结果进行解密，以验证一切工作正常，因此还需要构造Decryptor类的一个实例。注意，解密器需要密钥。
        
        decryptorPt = &decryptor;
        // 构造CKKSEncoder类的一个实例
        // 对明文进行加密，注意，第二个参数是比例系数
        encoderPt = &encoder;
    }
    else {
        
        public_keyPt = &public_key2;
        relin_keysPt = &relin_keys2;

        encryptorPt = &encryptor2;
        evaluatorPt = &evaluator2;
       
        decryptorPt = &decryptor2;
       
        encoderPt = &encoder2;
    }

    // 创建队列,并调用Split()函数,将结果返回给infix
    QQueue<QString> infix = Split(formulaStr);
    qDebug() << "88888888";
    // infix通过Transfer函数将中缀队列转换为后缀队列,方便计算
    infix = Transfer(infix);
    qDebug() << "99999999999";
    // 调用计算函数,并返回计算结果,Calculat中只有一个函数,这个是调用的第二个函数
    return Calculate(infix, equation);
}

