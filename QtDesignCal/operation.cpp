#include "seal/seal.h"
#include <iostream>
using namespace std;
using namespace seal;

// 私钥的长度
#define SK_Num 294912
// 密文的长度
#define CIPHER_Num 262144

__declspec(selectany) EncryptionParameters* parmsPt;

__declspec(selectany) size_t* poly_modulus_degreePt;

__declspec(selectany) double* scalePt;

__declspec(selectany) shared_ptr<SEALContext>* contextPt;

__declspec(selectany) KeyGenerator* keygenPt;

__declspec(selectany) PublicKey* public_keyPt;

__declspec(selectany) SecretKey* secret_keyPt;

__declspec(selectany) RelinKeys* relin_keysPt;

__declspec(selectany) Encryptor* encryptorPt;

__declspec(selectany) Evaluator* evaluatorPt;

__declspec(selectany) Decryptor* decryptorPt;

__declspec(selectany) CKKSEncoder* encoderPt;


/// <summary>
/// 函数作用: 递归的进行归并相乘,使代码简洁高效的运行
/// 参数含义: a是要归并的密文向量,l是密文的左端点,r是密文的右端点
/// 主要流程: 密文向量不断递归,直到分成两两一组时,分别相乘,并进行重现性化与缩放bit直到
///           将所有乘得的数据保存在左侧,最终会在第一位a[0]处得到结果
/// </summary>

inline void mergeSort(vector<Ciphertext>& a, int l, int r)
{
    if (l == r)
        return;
    int mid = l + r >> 1;
    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);
    int i = l, j = mid + 1;
    (*evaluatorPt).multiply_inplace(a[i], a[j]);
    // 缩放大小回不到原来的数值
    (*evaluatorPt).relinearize_inplace(a[i], (*relin_keysPt));
    (*evaluatorPt).rescale_to_next_inplace(a[i]);
}

/// <summary>
/// 函数作用: 针对乘法操作将常用的流程语句,合并到一起
/// 参数含义: result是返回的结果,num是乘法操作的个数,vet是要进行乘法操作的数组,cipher是运算结果的密文
/// 主要流程: 将vet明文转化为明文多项式,再将明文多项式转化为密文,之后执行归并相乘,再将得到的结果存在cipher中
///           最后对密文解码转化为明文多项式,由明文多项式转化为明文,并存在result中
/// 函数意义: 主要是为了解耦,在运行乘法,除法,指数操作的时候,都用到了这段代码,故将其封装为一个函数
/// </summary>

inline Ciphertext encryptHelper(vector<double>& result, int num, double* vet, IntArray<unsigned __int64>& cipher) {
    // 为明文申请空间
    vector<Plaintext> pt;
    Plaintext p1;
    // 将输入数据映射为明文多项式
    for (int i = 0; i < num; i++)
    {
        (*encoderPt).encode(vet[i], (*scalePt), p1);
        pt.push_back(p1);
    }
    // 为密文申请空间
    vector<Ciphertext> ct;
    Ciphertext c1;
    // 将明文多项式转化为密文，实现加密
    for (vector<Plaintext>::iterator it = pt.begin(); it != pt.end(); it++)
    {
        (*encryptorPt).encrypt(*it, c1);
        ct.push_back(c1);
    }
    // 执行同态乘法操作,采用归并相乘的操作,所有结果保留在ct[0]中
    mergeSort(ct, 0, num - 1);
    // 将密文结果整合赋给第二个参数cipher
    cipher.resize(CIPHER_Num);
    for (int i = 0; i < CIPHER_Num; i++)
    {
        cipher[i] = ct[0].int_array()[i];
    }
    // 解密
    (*decryptorPt).decrypt(ct[0], p1);
    result.clear();
    (*encoderPt).decode(p1, result);
    return ct[0];
}

/// <summary>
/// 加法运算,支持多个数据同时相加,可返回密文结果
/// </summary>

inline void addOperation(vector<double>& input, IntArray<unsigned __int64>& cipher) {
    
    // 为明文申请空间
    vector<Plaintext> pt;
    Plaintext p1;
    // 将输入数据映射为明文多项式
    for (vector<double>::iterator it = input.begin(); it != input.end(); it++)
    {
        (*encoderPt).encode(*it, (*scalePt), p1);
        pt.push_back(p1);
    }
    // 为密文申请空间
    vector<Ciphertext> ct;
    Ciphertext c1;
    for (vector<Plaintext>::iterator it = pt.begin(); it != pt.end(); it++)
    {
        (*encryptorPt).encrypt(*it, c1);
        ct.push_back(c1);
    }
    // 执行同态加法操作
    c1 = ct[0];
    if (ct.size() > 1)
    {
        for (vector<Ciphertext>::iterator it = ct.begin() + 1; it != ct.end(); it++)
        {
            // 将所有的数据加法整合到c2中
            (*evaluatorPt).add_inplace(c1, *it);
        }
    }
    // 将密文结果整合赋给第二个参数cipher
    cipher.resize(CIPHER_Num);
    for (int i = 0; i < CIPHER_Num; i++)
    {
        cipher[i] = c1.int_array()[i];
    }
    // 解密
    (*decryptorPt).decrypt(c1, p1);
    input.clear();
    (*encoderPt).decode(p1, input);
}


/// <summary>
/// 减法运算,支持多个数据同时相减,主要运用加法,先将除第一个之外的所有数据
/// 处理成负数,之后与加法操作一样,可返回密文结果
/// </summary>

inline void subOperation(vector<double>& input, IntArray<unsigned __int64>& cipher) {

    // 对输入的数据进行，将第一项数据保持不变，其余全部取反
    if (input.size() > 1) {
        for (vector<double>::iterator it = input.begin() + 1; it != input.end(); it++)
        {
            *it *= -1;
        }
    }
    addOperation(input, cipher);
}

/// <summary>
/// 乘法运算,支持多个数据同时相乘,可返回密文结果
/// 主要思想就是用到归并相乘,归并相乘的目的就是为了能够将成数处在相同的深度
/// 本函数前几行代码就是为了构建2^n大小的数组,方便进行归并相乘
/// </summary>

inline void mulOperation(vector<double>& input, IntArray<unsigned __int64>& cipher) {

    // num为构建2^n的大小
    int num, tmp = input.size();
    if (tmp <= 1) num = 2;
    else num = int(pow(2, int(log2(tmp - 1)) + 1));
    // 动态声明数组个数,并全部赋值为1
    double* vet = new double[num];
    for (int i = 0; i < num; i++)
    {
        *(vet + i) = 1.0;
        if (i < input.size()) *(vet + i) = vet[i] * input[i];
    }
    encryptHelper(input, num, vet, cipher);
}

/// <summary>
/// 乘法运算,支持多个数据同时相乘,不可返回密文结果
/// 主要思想就是用到归并相乘,归并相乘的目的就是为了能够将成数处在相同的深度
/// </summary>
/// 

inline void divOperation(vector<double>& input, IntArray<unsigned __int64>& cipher) {

    // 创建数组所用的个数
    int num, tmp = input.size();
    if (tmp <= 1) num = 2;
    else num = int(pow(2, int(log2(tmp - 1)) + 1));
    // 动态声明数组个数,并全部赋值为1
    double* vet = new double[num];
    for (int i = 0; i < num; i++)
    {
        *(vet + i) = 1.0;
        if (i < input.size()) *(vet + i) = vet[i] * input[i];
        if (i > 0) vet[i] = 1 / vet[i];
        // 如何优雅的抛出异常 ???
        if (vet[i] == 0) throw invalid_argument("zero cannot be divided");
    }
    encryptHelper(input, num, vet, cipher);
}

/// <summary>
/// 指数运算,可返回密文结果
/// 目前可以实现底数为小数,指数为整数的运算,此函数if-else是为防止用户输入非法的范围
/// 主要思想与乘法除法相同,不同的是要确保指数的范围,且指数为正,为负时的情况不一样,需采用指数的绝对值运算
/// </summary>

inline void indOperation(double input1, double input2, vector<double>& result, IntArray<unsigned __int64>& cipher) {

    result.clear();
    int num;
    if (input2 == (int)input2 && input2 >= 1) {
        for (int i = 0; i < input2; i++)
            result.push_back(input1);
    }
    else if (input2 == (int)input2 && input2 <= -1) {
        for (int i = 0; i < abs(input2); i++)
            result.push_back(1.0 / input1);
    }
    else if (input2 == 0 && input1 != 0) {
        result.push_back(1.0);
    }
    else {
        // 如何优雅的抛出异常 ???
        throw invalid_argument("Illegal data");
    }
    input2 = abs(input2);
    // 创建数组所用的个数
    if (input2 <= 1) num = 2;
    else num = int(pow(2, int(log2(input2 - 1)) + 1));
    // 原本不用创建这个指针的,只是为解耦
    double* vet = new double[num];
    for (int i = 0; i < num; i++)
    {
        *(vet + i) = 1.0;
        if (i < result.size()) *(vet + i) = vet[i] * result[i];
    }
    encryptHelper(result, num, vet, cipher);
}

/// <summary>
/// 幂运算,可返回密文结果
/// 目前可以实现底数为e,指数为小数的运算,此函数主要采用泰勒展开来进行计算
/// 主要思想e^x=1+x/1!+x^2/2!+x^3/3!+⋯,第一个for循环确定加密的项,并构造乘法所需数组,进行乘法操作,
/// 后面进行层数之间的同步,不然不能够进行加法运算,最后进行加法操作
/// </summary>

inline void expOperation(double x, vector<double>& result, IntArray<unsigned __int64>& cipher) {

    // ans表示泰勒展开的第一项与第二项的和,temp为了记录阶乘数
    double temp, ans = 1.0 + x;
    Plaintext pf;
    // 先将常数项加密
    (*encoderPt).encode(ans, (*scalePt), pf);
    Ciphertext cf;
    (*encryptorPt).encrypt(pf, cf);
    vector<Ciphertext> ciph;
    // 泰勒展开20项
    for (int i = 1; i <= 20; i++)
    {
        vector<double> ret;
        Ciphertext Ctmp;
        int num = 0;
        // temp乘以1.0是为了在运算的时候将其当作小数类型
        temp = i * 1.0;
        // 构建当前展开项的数组,要构建正常2*i项的数组,并补全为2^n个元素
        if (i <= 1)  num = 2;
        else num = int(pow(2, int(log2(i * 2 - 1)) + 1));
        double* vet = new double[num];
        for (int j = 0; j < num; j++)
        {
            if (j < i * 2 && j % 2 == 0) vet[j] = x;
            else if (j < i * 2 && temp >= 1 && j % 2 == 1) {
                vet[j] = (1.0 / temp);
                temp--;
            }
            else vet[j] = 1.0;
        }
        // 调用常用关于乘法的辅组函数,并将返回相乘之后的密文结果
        Ctmp = encryptHelper(ret, num, vet, cipher);
        // 保存在ciph中,为相加做准备
        ciph.push_back(Ctmp);
    }
    // 更改层数,将所有的乘数结果拉取到同一层,18指的是规定按照第18项所在深度拉取
    parms_id_type last_parms_id = ciph[18].parms_id();
    cf.scale() = *scalePt;
    (*evaluatorPt).mod_switch_to_inplace(cf, last_parms_id);
    // 更改层数,将所有的乘数结果拉取到同一层
    for (vector<Ciphertext>::iterator it = ciph.begin(); it != ciph.end(); it++)
    {
        it->scale() = *scalePt;
        (*evaluatorPt).mod_switch_to_inplace(*it, last_parms_id);
    }
    for (vector<Ciphertext>::iterator it = ciph.begin() + 1; it != ciph.end(); it++)
    {
        // 将所有的数据加法整合到cf中
        (*evaluatorPt).add_inplace(cf, *it);
    }
    // 将密文结果整合赋给第二个参数cipher
    cipher.resize(CIPHER_Num);
    for (int i = 0; i < CIPHER_Num; i++)
    {
        cipher[i] = cf.int_array()[i];
    }
    (*decryptorPt).decrypt(cf, pf);
    result.clear();
    (*encoderPt).decode(pf, result);
}

/// <summary>
/// 实现单个数字的加密
/// </summary>

inline void aSingleEncryption(double& input,IntArray<unsigned __int64>& cipher) {

    cipher.resize(CIPHER_Num);
    // 为明文申请空间
    Plaintext p1;
    // 将输入数据映射为明文多项式
    (*encoderPt).encode(input, (*scalePt), p1);
    // 为密文申请空间
    Ciphertext c1;
    // 将明文多项式转化为密文，实现加密
    (*encryptorPt).encrypt(p1, c1);

    for (int i = 0; i < CIPHER_Num; i++)
    {
        cipher[i] = c1.int_array()[i];
    }
}