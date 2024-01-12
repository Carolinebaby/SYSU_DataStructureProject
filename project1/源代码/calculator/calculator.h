#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

class Calculator
{
public:
    Calculator(std::string& s); // 构造函数，直接传入多项式的string形式，在函数体内调用 SimplifyPolynomial() 函数对多项式进行格式化
    //与外界接口函数
    std::string GetSimplifiedFx(); // 化简后的多项式
    std::string GetDF(); // 多项式的导函数
    std::string GetFxValue(double x);  //给定x值下的多项式的值
    std::string GetIntFx();
private:
    // 功能 1 化简多项式
    void SimplifyPolynomial();  //提取两个多项式以及之间的符号，调用相应加减乘化简
    std::pair<int, double> GetExpCoef(std::string monomial, int sign); // 获得单项的指数-系数
    std::pair<std::vector<int>, std::map<int, double>> PolyFormat(std::string& polynomial); // 将多项式格式化成 map(指数-系数) 和 vector（指数）
    std::pair<std::vector<int>, std::map<int, double>> PolyProduct(std::string& polynomial1, std::string& polynomial2); // 多项式相乘
    std::pair<std::vector<int>, std::map<int, double>> PolyAdd(std::string& polynomial1, std::string& polynomial2); // 多项式相加
    std::pair<std::vector<int>, std::map<int, double>> PolySubstract(std::string& polynomial1, std::string& polynomial2); //多项式相减

    // 功能 2 求导函数
    std::pair<std::vector<int>, std::map<int, double>> GetDerivative(); // 获取导函数的标准格式

    // 功能 3 求给定 x 的值
    double CalValue(double x);  // 根据给定的 x 值计算 f(x)的数值大小

    // 补充功能 求积分函数
    std::pair<std::vector<int>, std::map<int, double>> GetIntegral(); // 获取积分函数的标准格式，其中 exp = 0代表为 lnx

    // 其他函数
    int StringToNum(std::string& s); // 判断 string 类是否为数字
    int StringToNum(char& s);  // 判断char 类是否为数字
    std::string NumSimplify(std::string s);  // 数字简化：6.0000=>6, 6.50000=>6.5
    std::string FormatToString(std::pair<std::vector<int>, std::map<int, double>>& p,int isIntegral = 0);  // 将 标准格式转换为字符串


    // 数据成员
    std::string fxpolynomial;         // string 类型的多项式
    std::vector<int> fxexponent;      // 存储项的指数
    std::map<int, double> fxexpcoef;  // 存储项的 指数-系数 键值对

};

#endif // CALCULATOR_H
