#ifndef TXTANALYSISWIDGET_H
#define TXTANALYSISWIDGET_H

#include <QWidget>
#include <string>
#include <chartwidget.h>
#include <word.h>

namespace Ui {
class TxtAnalysisWidget;
}

class TxtAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TxtAnalysisWidget(QWidget *parent = nullptr);
    ~TxtAnalysisWidget();
    int GetKInput();
    void SetResult(std::string& text);
    void SetTitle(std::string title);
    void SetWords(std::vector<Word>& words);

signals:
    void InputChanged();

private slots:
    void ShowChart();

private:
    Ui::TxtAnalysisWidget *ui;
    ChartWidget cw;
    std::vector<Word> words;
};

#endif // TXTANALYSISWIDGET_H
