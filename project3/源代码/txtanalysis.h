#ifndef TXTANALYSIS_H
#define TXTANALYSIS_H

#include <analysis.h>


class TxtAnalysis: public Analysis
{
public:
    TxtAnalysis(std::string& text);
    std::vector<Word> GetKthWords(int k = 10);

private:
    void GetAllWordFreq();
};

#endif // TXTANALYSIS_H
