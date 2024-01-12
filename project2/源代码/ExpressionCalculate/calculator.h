#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

class Calculator
{
public:
    Calculator(std::string& exp, int x = 0);
    // 获取计算结果
    std::string GetResult();
    // 判断计算的式子是否有效（例如计算过程中的除法运算的分母为 0，则无效）
    bool Getisvalid();
private:
    // 表达式计算 的 主要功能函数
    bool ExpCalc();
    bool CalcNum(std::stack<double>&, std::stack<char>&);

    // 把多项式的 x 替换成 xvalue，然后进行 ExpCalc，xvalue 默认为 0
    void PolyToExp(int xvalue);

    // 其他函数
    int StringToNum(std::string& s); // 判断 string 类是否为数字
    int StringToNum(char& s);  // 判断char 类是否为数字
    std::string NumSimplify(std::string s);  // 数字简化：6.0000=>6, 6.50000=>6.5
    bool isPoly();  // 判断是否为包含未知数（是否是多项式）
    bool isValid(int xvlue = 0);  // 判断是否有效

    // 数据成员
    std::string exp;  // 表达式的字符串形式
    double expresult;  // 表达式的计算结果
    bool isvalid = true;  // 是否有效

    // 下面为静态变量，所有类对象共享的数据
    static std::map<char, int> opidx;  // 存储 运算符与 isHigher数组的下标的对应关系：char类型运算符-下标
    static int isHigher[8][8]; // isHigher[left][right] 右边是否比左边的优先级高，-1 左边优先级更高， 0 优先级相等， 1 右边优先级更高
};

#endif // CALCULATOR_H
