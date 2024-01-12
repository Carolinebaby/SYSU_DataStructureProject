#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QVBoxLayout>
#include <word.h>

namespace Ui {
class ChartWidget;
}

class ChartWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();
    void SetWords(std::vector<Word>& words);
    void ShowChart();

private:
    Ui::ChartWidget *ui;
    QChart* chart;
    QChartView* chartView;
    std::vector<Word> words;
    QStringList categories;
};

#endif // CHARTWIDGET_H
