#include "cppanalysiswidget.h"
#include "ui_cppanalysiswidget.h"

CppAnalysisWidget::CppAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CppAnalysisWidget)
{
    ui->setupUi(this);
    setWindowTitle("Cpp Analysis");
    connect(ui->showchartbutton, SIGNAL(clicked()), this, SLOT(ShowChart()));
}

void CppAnalysisWidget::SetTitle(std::string title)
{
    ui->titlelabel->setText(QString::fromStdString(title));
}
void CppAnalysisWidget::SetResult(std::string& text)
{
    ui->outputtext->setPlainText(QString::fromStdString(text));
}

void CppAnalysisWidget::SetWords(std::vector<Word>& words)
{
    this->words = words;
}

void CppAnalysisWidget::ShowChart()
{
    cw.show();
    cw.SetWords(words);
    cw.ShowChart();
}

CppAnalysisWidget::~CppAnalysisWidget()
{
    delete ui;
}
