#include "chartwidget.h"
#include "ui_chartwidget.h"

ChartWidget::ChartWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartWidget)
{
    ui->setupUi(this);
    setWindowTitle("Chart");

    chart = new QChart;
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}
void ChartWidget::SetWords(std::vector<Word>& words)
{
    this->words.clear();
    this->words.resize(words.size());
    this->words = words;
}

void ChartWidget::ShowChart() {
    chart->removeAllSeries();
    // 创建柱状统计图
    QBarSeries* series = new QBarSeries;
    QBarSet* barSet = new QBarSet("Frequency");
    categories.clear();
    for (Word& word : words) {
        *barSet << word.freq;
        categories << QString::fromStdString(word.word);
    }

    series->append(barSet);

    // 创建横坐标轴
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(45);
    axisX->setLabelsFont(QFont("Arial", 5));

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(axisX, series);


    // 显示柱状统计图窗口
    chartView->show();
}

ChartWidget::~ChartWidget()
{
    delete ui;
}
