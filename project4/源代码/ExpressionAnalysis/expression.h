#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<expnode.h>
#include<sstream>
#include<cmath>
#include<stack>
class Expression
{
public:
    Expression();
    Expression(std::string);
    ~Expression();

    std::string GetInfix();                         // 获取中缀表达式
    ExpNode* GetNodes();                            // 获取结点
    double GetExpValue(std::string varInput);       // 获取给定变量的赋值后的表达式的值
    void CompoundExpr(Expression &newE, std::string op, Expression &E); // 合并表达式
    void MergeConst();                              // 合并常数项的运算

    bool isValid();
    bool isCalValid();

private:
    void PrefixToNodes();                           //前缀表达式转化为表达式二叉树
    void NodesToInfix(ExpNode* root);               // 结点转为中缀表达式
    double CalExpValue(ExpNode* root, std::map<char, double> varVal);      // 计算表达式的值
    void SetPrefix(std::string prefix);             // 设置前缀表达式的值，在合并表达式的时候会用到
    void MergeConst(ExpNode* &root);                 //合并常数运算

    void DeleteNodes(ExpNode* &root);               //删除二叉树
    std::string SimplifyNumString(std::string s);   // 化简数字字符串
    void CutHeadAndTailSpace(std::string& s);       // 删除字符串前后空白字符

    // 基本判断函数
    bool isNum(std::string& s);
    bool isNum(char c);
    bool isLetter(std::string& s);
    bool isLetter(char s);
    bool isOperator(std::string &c);
    bool isOperator(char c);
    bool isTriFunc(std::string s);

    double StringToNum(std::string& s);
    double StringToNum(char& s);

    // 数据
    bool isvalid;        // 判断输入的前缀公式是否有效
    bool isCalvalid;     // 判断输入的变量赋值后的公式是否有效
    std::string prefix;  // 前缀表达式
    std::string infix;   // 中缀表达式
    ExpNode* expRoot;    // 表达式树的根节点
};

#endif // EXPRESSION_H
