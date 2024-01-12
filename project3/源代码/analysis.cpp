#include "analysis.h"

Analysis::Analysis(std::string& s): text(s)
{
    // 所有字母转化为小写，方便统计单词数目
    for (char &c : text)
    {
        if (std::isalpha(c))
        {
            c = std::tolower(c);
        }
    }

    // 将文本一行一行拆分
    SplitTextToLines();
}

void Analysis::SplitTextToLines()
{
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line))
    {
        textLines.emplace_back(line);
    }
}

// 获得每个单词对应的行数
void Analysis::GetWordLines(std::vector<Word>& wordvec)
{
    for(int i = 0; i<(int)wordvec.size(); i++)
    {
        for(int j = 0; j<(int)textLines.size(); j++)
        {
            wordvec[i].WordInLine(textLines[j], j+1);
        }
    }
}
