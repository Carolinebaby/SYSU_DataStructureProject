#include "word.h"
/******      无参构造函数      ******/
Word::Word(){}

Word::Word(QString& qword)
{
    this->qword = qword.toLower();
    QStrGetNext();
}

/******      带参数的构造函数      ******/
Word::Word(std::string word, int freq):word(word), freq(freq)
{
    std::transform(this->word.begin(), this->word.end(), this->word.begin(), ::tolower);
    GetNext();
}

/******      重载运算符<，便于进行 sort 操作      ******/
bool Word::operator<(const Word& w)
{
    if(freq == w.freq) return word < w.word;  // 字典顺序小的排在前面
    return freq > w.freq; // 出现频率大的排在前面
}

/******      KMP算法      ******/
// KMP算法的主函数
void Word::WordInLine(std::string s, int linenumber)
{
    int i = 0, j = 0;  // 初始化两个指针 i 和 j 为 0
    while (j < (int)word.length() && i < (int)s.length())
    {
        if (j == -1 || s[i] == word[j])  // 如果 j 为 -1 或者当前字符匹配成功
            i++, j++;  // 移动 i 和 j
        else
            j = next[j];  // 不匹配时，根据 next 数组更新 j

        if(j == (int)word.length() && ((i<(int)s.length() && std::isalpha(s[i])) ||(i-(int)word.length()-1>=0 && std::isalpha(s[i-(int)word.length()-1]))))
        {
            j = 0;  // 如果找到完全匹配的子串，且其前后字符为字母，则重置 j 为 0
        }
    }

    if (j == (int)word.length())
        lines.emplace_back(linenumber);  // 如果 j 等于子串长度，说明找到了匹配，将行号添加到结果集合中
}

// 获取 next 数组
void Word::GetNext()
{
    int i = 0, j = -1;
    next.emplace_back(-1);  // 初始化 next 数组，第一个元素为 -1
    while (i < (int)word.length())
    {
        if (j == -1 || word[i] == word[j])
        {
            i++, j++;  // 如果字符匹配成功，移动 i 和 j
            if ( i < (int)word.length() && word[i] == word[j])
            {
                next.emplace_back(next[j]);
            }
            else
                next.emplace_back(j);
        }
        else
            j = next[j];  // 如果字符不匹配，根据已知的 next 值更新 j
    }
}


/******      找到单词出现的所有位置      ******/
// 在给定的文本字符串 text 中查找子字符串 qword 出现的所有位置
std::vector<int> Word::FindAllPos(QString& text)
{
    std::vector<int> res;  // 用于存储子字符串 qword 出现的位置的结果集合
    if (text.isEmpty() || qword.isEmpty())  // 如果文本或要查找的单词为空，直接返回空结果
        return res;

    int i = 0, j = 0;  // 初始化两个指针 i 和 j 为 0

    while (i < (int)text.length())  // 循环遍历文本字符串
    {
        if (j == (int)qword.length())  // 如果 j 等于子字符串长度，表示找到一个匹配
        {
            res.emplace_back(i - j);  // 将匹配位置的起始索引添加到结果集合
            j = 0;  // 重置 j 为 0，准备继续查找下一个匹配
            continue;
        }

        if (j == -1 || text[i] == qword[j])  // 如果 j 为 -1 或当前字符匹配成功
        {
            i++, j++;  // 移动 i 和 j
        }
        else
        {
            j = next[j];  // 不匹配时，根据 next 数组更新 j，以提高匹配效率
        }
    }

    // 判断文章末尾的情况
    if (j == (int)qword.length())  // 如果 j 等于子字符串长度，表示找到一个匹配
    {
        res.emplace_back(i - j);  // 将匹配位置的起始索引添加到结果集合
    }

    return res;  // 返回包含所有匹配位置的结果集合
}

// 获取 next 数组
void Word::QStrGetNext()
{
    int i = 0, j = -1;
    next.emplace_back(-1);  // 初始化 next 数组，第一个元素为 -1
    while (i < (int)qword.length())
    {
        if (j == -1 || qword[i] == qword[j])
        {
            i++, j++;  // 如果字符匹配成功，移动 i 和 j
            if (i < (int)qword.length() && qword[i] == qword[j])
            {
                next.emplace_back(next[j]);  // 如果下一个字符仍匹配，更新 next 数组
            }
            else
                next.emplace_back(j);
        }
        else
            j = next[j];  // 如果字符不匹配，根据已知的 next 值更新 j
    }
}
