#include "qtdesigncal.h"
#include "seal/seal.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include "calculator.h"
#include <QFileDialog>
#include <fstream>

using namespace std;
using namespace seal;

bool isCipher = false;
QString path = "";

QtDesignCal::QtDesignCal(QWidget *parent)
    : QWidget(parent)
{
    iniUI();
    select();
    setWindowTitle(u8"密文选择");
}

void QtDesignCal::iniUI() {
    setStyleSheet(
        "QPushButton{"
        "font-family:Microsoft Yahei;"
        "font: 40px;"
        "color:white;"
        "border-style:outset;"
        "border-radius:5px;"
        "background-color:rgb(14 , 150 , 254);"
        "}"
        //鼠标按下样式
        "QPushButton:pressed{"
        "background-color:rgb(14 , 135 , 228);"
        "padding-left:3px;"
        "padding-top:3px;"
        "}"
        //鼠标悬停样式
        "QPushButton:hover{"
        "background-color:rgb(44 , 137 , 255);"
        "}"
    );
    automaticBtn = new QPushButton(u8"自动生成密钥");
    manualBtn = new QPushButton(u8"手动添加密钥");
    cipherText1 = new QTextEdit();
    cipherText1->setPlaceholderText(u8"密钥显示区");
    cipherText1->setReadOnly(true);
    cipherText1->setStyleSheet(
        "border-style:inset;"
        "border:2px solid mediumvioletred ;"
        "border-radius: 10px;"
        "min-height:400px;"
        "font-family:微软雅黑; font-size: 30px;"
    );
    cipherText2 = new QTextEdit();
    cipherText2->setPlaceholderText(u8"公钥显示区");
    cipherText2->setReadOnly(true);
    cipherText2->setStyleSheet(
        "border-style:inset;"
        "border:2px solid mediumvioletred ;"
        "border-radius: 10px;"
        "min-height:400px;"
        "font-family:微软雅黑; font-size: 30px;"
    );
    QHBoxLayout* HLay1 = new QHBoxLayout;
    HLay1->addWidget(automaticBtn);
    HLay1->addWidget(manualBtn);

    QHBoxLayout* HLay2 = new QHBoxLayout;
    HLay2->addWidget(cipherText1);
    HLay2->addWidget(cipherText2);

    QVBoxLayout* VLay = new QVBoxLayout;
    VLay->addLayout(HLay1);
    VLay->addLayout(HLay2);
    setLayout(VLay);
}

void QtDesignCal::select() {
    calculator* tmp = new calculator;
    connect(automaticBtn, SIGNAL(clicked()), this, SLOT(jumpCal()));
    connect(manualBtn, SIGNAL(clicked()), this, SLOT(readTxt()));
}

void QtDesignCal::jumpCal() {
    cal.setWindowTitle(u8"计算器");
    cal.resize(530, 700);
    cal.show();
    this->close();
}

void QtDesignCal::readTxt() {

    isCipher = true;

    cipherText1->setPlainText("");
    cipherText2->setPlainText("");
    automaticBtn->setText(u8"确定提交");
    // 定义文件对话框类
    QFileDialog* fileDialog = new QFileDialog(this);
    // 定义文件对话框标题
    fileDialog->setWindowTitle(tr(u8"选择密钥文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    // 设置文件过滤器
    fileDialog->setNameFilter(tr("text files(*.txt *.doc *.docx *.dat);;All files(*.*)"));
    // 设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    // 设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    // 读取所有选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
    if (fileNames.size()!=1)
    {
        QMessageBox::information(this, u8"提醒", u8"请选择一个文件");
        return;
    }
    // 将文件显示在密文框中
    path = fileNames[0];
    ifstream inFile(path.toStdString(), ios::in | ios::binary);

    Plaintext::pt_coeff_type s;
    QString fileTxt = "";
    int i = 0;
    //一直读到文件结束
    while (inFile.read((char*)&s, sizeof(s))) { 
        fileTxt += QString::number(s) + "\n";
        i++;
        if (i > 30) break;
    }
    inFile.close();
    cipherText1->append(fileTxt);

    cipherText1->moveCursor(QTextCursor::End);


    // 创建一个用户输入的私钥

    EncryptionParameters parms(scheme_type::CKKS);

    size_t poly_modulus_degree = 16384;
    // 设置多相式模数度数
    parms.set_poly_modulus_degree(poly_modulus_degree);
    // 设定系数模, 应与16384相对应
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 40, 40, 40, 40, 40, 60 }));
    double scale = pow(2.0, 40);
    // 定义一个产生器
    auto context = SEALContext::Create(parms);

    // 创建一个钥匙生成器
    KeyGenerator keygen(context);

    //// 创造一个私钥
    SecretKey secret_key = keygen.secret_key();

    filebuf buf;
    if (buf.open(path.toStdString(), ios::in | ios::binary) == nullptr) {
        return;
    }
    istream is(&buf);
    secret_key.load(context, is);

    // 重新构建密钥
    KeyGenerator keygen2(context, secret_key);

    auto public_key = keygen2.public_key();

    QString publicTxt = "";
    for (int i = 0; i < 30; i++)
    {
        publicTxt += QString::number(public_key.data().int_array()[i]);
        publicTxt += "\n";
        qDebug() << QString::number(public_key.data().int_array()[i]) + "\n";
    }
    cipherText2->append(publicTxt);
    cipherText2->moveCursor(QTextCursor::End);
}

