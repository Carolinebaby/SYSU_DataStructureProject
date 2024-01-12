#ifndef CPPANALYSISWIDGET_H
#define CPPANALYSISWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts>
#include <word.h>
#include<chartwidget.h>

namespace Ui {
class CppAnalysisWidget;
}

class CppAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CppAnalysisWidget(QWidget *parent = nullptr);
    ~CppAnalysisWidget();
    void SetResult(std::string& text);
    void SetTitle(std::string title);
    void SetWords(std::vector<Word>& words);

private slots:
    void ShowChart();

private:
    Ui::CppAnalysisWidget *ui;
    ChartWidget cw;
    std::vector<Word> words;
};

#endif // CPPANALYSISWIDGET_H
