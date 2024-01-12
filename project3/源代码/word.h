#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>
#include <algorithm>
#include <QString>

class Word
{
public:
    Word();
    Word(QString& qword); // 传入 QString 字符串，方便直接对源字符进行单个字符串匹配
    Word(std::string word, int freq = 0);
    bool operator<(const Word& w); // 重载运算符，便于进行 sort 操作

    void WordInLine(std::string s, int linenumber);// KMP 算法，判断词是否在这个字符串中，这个字符串存储的是一行字符串，linenumber对应的行字符串对应原来文本的行号，在判断 word 在 s 中 后把行号存入 lines 容器中。
    void GetNext();// 获取 next 数组，string 版本
    void QStrGetNext();// 获取 next 数组，QString 版本
    std::vector<int> FindAllPos(QString& text); // 用于单个模式串的匹配，返回的 qword 在 text 中的所有位置，方便文本高亮显示

    std::string word;
    QString qword;
    int freq;  // 频数
    std::vector<int> next;  // next 数组
    std::vector<int> lines;
};

#endif // WORD_H
