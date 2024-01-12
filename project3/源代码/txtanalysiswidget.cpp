#include "txtanalysiswidget.h"
#include "ui_txtanalysiswidget.h"

TxtAnalysisWidget::TxtAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TxtAnalysisWidget)
{
    ui->setupUi(this);
    setWindowTitle("Txt Analysis");
    connect(ui->kvalueinput, &QLineEdit::textChanged, this, &TxtAnalysisWidget::InputChanged);
    connect(ui->showchartbutton, SIGNAL(clicked()), this, SLOT(ShowChart()));
}
int TxtAnalysisWidget::GetKInput()
{
    std::string kstr = ui->kvalueinput->text().toStdString();
    if(kstr.length() == 0) return 0;
    if(kstr[0] == '-') return -1;
    for(int i = 0; i<(int)kstr.length(); i++)
    {
        if(kstr[i]>'9' || kstr[i] < '0') return -1;
    }
    return stoi(kstr);
}
void TxtAnalysisWidget::SetResult(std::string& text)
{
    ui->analyseoutput->setPlainText(QString::fromStdString(text));
}

void TxtAnalysisWidget::SetWords(std::vector<Word>& words)
{
    this->words = words;
}

void TxtAnalysisWidget::ShowChart()
{
    cw.show();
    cw.SetWords(words);
    cw.ShowChart();
}

TxtAnalysisWidget::~TxtAnalysisWidget()
{
    delete ui;
}
