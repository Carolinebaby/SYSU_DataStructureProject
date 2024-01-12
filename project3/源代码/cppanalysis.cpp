#include "cppanalysis.h"
/******      构造函数      ******/
CppAnalysis::CppAnalysis(std::string& s): Analysis(s)
{
    CalcFunctionCount();
    CalcCommentCount();
    CalcBlankLineCount();
    CalcHeaderFileCount();
}

/******      与外界的接口      ******/
int CppAnalysis::GetFunctionCount()
{
    return functionCount;
}
int CppAnalysis::GetCommentCount()
{
    return commentCount;
}
int CppAnalysis::GetBlankLineCount()
{
    return blankLineCount;
}
int CppAnalysis::GetHeaderFileCount()
{
    return headerFileCount;
}

/******      利用 正则表达式计算各个 参数的数量      ******/
// 函数的数量
void CppAnalysis::CalcFunctionCount()
{
    // 使用正则表达式来匹配函数定义
    std::regex functionPattern(R"((\w+)\s+(\w+)\s*\([^)]*\)\s*\{)");
    std::sregex_iterator iter(text.begin(), text.end(), functionPattern);
    std::sregex_iterator end;

    functionCount = 0;
    while (iter != end)
    {
        functionCount++;
        ++iter;
    }

}
// 注释的数量
void CppAnalysis::CalcCommentCount()
{
    commentCount = 0;

    // 正则表达式匹配注释，包括单行注释和多行注释
    std::regex commentRegex(R"((\/\/[^\n]*|\/\*[\s\S]*?\*\/))");

    std::sregex_iterator it(text.begin(), text.end(), commentRegex);
    std::sregex_iterator end;

    while (it != end)
    {
        commentCount++;
        ++it;
    }

}
// 空白行的数量
void CppAnalysis::CalcBlankLineCount()
{
    blankLineCount = 0;
    std::istringstream textStream(text);
    std::string line;

    while (std::getline(textStream, line))
    {
        if (line.find_first_not_of(" \t") == std::string::npos)
        {
            // 行只包含空格和制表符，被视为空白行
            blankLineCount++;
        }
    }
}
// 头文件的数量
void CppAnalysis::CalcHeaderFileCount()
{
    headerFileCount = 0;
    std::istringstream codeStream(text);
    std::string line;

    while (std::getline(codeStream, line))
    {
        // 移除行首的空白字符
        size_t pos = line.find_first_not_of(" \t");
        if (pos != std::string::npos)
        {
            line = line.substr(pos);
        }

        if (line.find("#include") == 0)
        {
            headerFileCount++;
        }
    }
}

/******      得到保留字的出现频率和出现的行数      ******/
// 接口函数
std::vector<Word> CppAnalysis::GetWords()
{
    GetReservedWord();
    return words;
}

// 功能函数
void CppAnalysis::GetReservedWord()
{
    for(int i = 0; i<(int)text.length(); i++)
    {
        while(!std::isalpha(text[i])) i++;
        std::string word;
        while(std::isalpha(text[i]))
        {
            word+=text[i];
            i++;
        }
        if(isReservedWords(word)) // 判断单词是否为保留字，如果是，则放入 map 中
            wordFrequency[word]++;
    }

    for(auto it = wordFrequency.begin(); it != wordFrequency.end(); it++)
    {
        words.emplace_back(it->first,it->second);
    }

    sort(words.begin(), words.end());

    GetWordLines(words);
}

/******      判断是否为保留字      ******/
bool CppAnalysis::isReservedWords(const std::string& word)
{
    static const std::string reservedword[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
        "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
        "void", "volatile", "while"
    };

    // 在关键字数组中查找
    return std::find(std::begin(reservedword), std::end(reservedword), word) != std::end(reservedword);
}
