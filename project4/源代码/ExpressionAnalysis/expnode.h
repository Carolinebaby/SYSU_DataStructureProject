#ifndef EXPNODE_H
#define EXPNODE_H
#include<string>
#include<map>
#include<vector>

class ExpNode
{
public:
    ExpNode();
    ExpNode(std::string);
    bool isLetter();    // 判断是否是字母
    bool isNum();       // 判断是否是数字
    bool isOperator();  // 判断是否是运算符
    bool isTriFunc();   // 判断是否是三角函数

    std::string c;      // 存储运算数或运算符
    ExpNode* left;      // 左子树
    ExpNode* right;     // 右子树
    int x,y;            // 绘图时候的 x,y 坐标
};

#endif // EXPNODE_H
