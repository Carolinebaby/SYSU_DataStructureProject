#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <word.h>
#include <sstream>

class Analysis
{
public:
    Analysis(std::string& text);

protected:
    void SplitTextToLines();
    void GetWordLines(std::vector<Word>& wordvec);


    std::string text;
    std::map<std::string, int> wordFrequency;
    std::vector<std::string> textLines;
    std::vector<Word> words;
};

#endif // ANALYSIS_H
