#ifndef CPPANALYSIS_H
#define CPPANALYSIS_H

#include <analysis.h>
#include <regex>

class CppAnalysis: public Analysis
{
public:
    CppAnalysis(std::string& s);
    std::vector<Word> GetWords();
    int GetFunctionCount();
    int GetCommentCount();
    int GetBlankLineCount();
    int GetHeaderFileCount();


private:
    void GetReservedWord();
    bool isReservedWords(const std::string& word);
    void CalcFunctionCount();
    void CalcCommentCount();
    void CalcBlankLineCount();
    void CalcHeaderFileCount();

    // 详细的统计信息
    int functionCount;
    int commentCount;
    int blankLineCount;
    int headerFileCount;
};

#endif // CPPANALYSIS_H
