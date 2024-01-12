#include "txtanalysis.h"
// 构造函数
TxtAnalysis::TxtAnalysis(std::string& s): Analysis(s)
{
    GetAllWordFreq();
}


// 获得频数最大的前 k 个单词，默认为10
std::vector<Word> TxtAnalysis::GetKthWords(int k)
{
    std::vector<Word> kthwords;
    for(int i = 0; i<k && i<(int)words.size(); i++)
    {
        kthwords.emplace_back(words[i]);
    }

    GetWordLines(kthwords);

    return kthwords;
}

// 获得所有单词的频数
void TxtAnalysis::GetAllWordFreq()
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
        wordFrequency[word]++;
    }

    for(auto it = wordFrequency.begin(); it != wordFrequency.end(); it++)
    {
        words.emplace_back(it->first,it->second);
    }

    sort(words.begin(), words.end());
}


