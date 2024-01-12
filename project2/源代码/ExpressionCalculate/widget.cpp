#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    /******    连接信号和槽（对应的操作）    ******/
    connect(ui->clearbutton, SIGNAL(clicked()), this, SLOT(ClearClicked()));
    connect(ui->backbutton,  SIGNAL(clicked()), this, SLOT(BackClicked()));
    connect(ui->zerobutton,  SIGNAL(clicked()), this, SLOT(ZeroClicked()));
    connect(ui->onebutton,  SIGNAL(clicked()), this, SLOT(OneClicked()));
    connect(ui->twobutton,  SIGNAL(clicked()), this, SLOT(TwoClicked()));
    connect(ui->threebutton,  SIGNAL(clicked()), this, SLOT(ThreeClicked()));
    connect(ui->fourbutton,  SIGNAL(clicked()), this, SLOT(FourClicked()));
    connect(ui->fivebutton,  SIGNAL(clicked()), this, SLOT(FiveClicked()));
    connect(ui->sixbutton,  SIGNAL(clicked()), this, SLOT(SixClicked()));
    connect(ui->sevenbutton,  SIGNAL(clicked()), this, SLOT(SevenClicked()));
    connect(ui->eightbutton,  SIGNAL(clicked()), this, SLOT(EightClicked()));
    connect(ui->ninebutton,  SIGNAL(clicked()), this, SLOT(NineClicked()));
    connect(ui->addbutton,  SIGNAL(clicked()), this, SLOT(AddClicked()));
    connect(ui->subtractbutton,  SIGNAL(clicked()), this, SLOT(SubtractClicked()));
    connect(ui->productbutton,  SIGNAL(clicked()), this, SLOT(ProductClicked()));
    connect(ui->powerbutton,  SIGNAL(clicked()), this, SLOT(PowerClicked()));
    connect(ui->divisionbutton,SIGNAL(clicked()), this, SLOT(DivisionClicked()));
    connect(ui->parenthesis,SIGNAL(clicked()), this, SLOT(ParenthesisClicked()));
    connect(ui->nightmodelbutton,SIGNAL(clicked()), this, SLOT(NightClicked()));
    connect(ui->xbutton,SIGNAL(clicked()), this, SLOT(XButtonClicked()));
    connect(ui->equalbutton,SIGNAL(clicked()), this, SLOT(SubmitClicked()));
    //connect(ui->equalbutton,SIGNAL(clicked()), this, SLOT(EqualCliked()));
    ui->inputbox->installEventFilter(this);
    ui->xvalueinputbox->installEventFilter(this);
    connect(ui->introductionbutton,  SIGNAL(clicked()), this, SLOT(IntroductionClicked()));
    connect(ui->closebutton, SIGNAL(clicked()), this, SLOT(CloseClicked()));
}


/************     提交      ************/
void Widget::SubmitClicked()
{
    ui->outputbox->clear();
    // 保持光标一直存在
    //ui->inputbox->setFocus();
    // 读取内容
    std::string exptext = ui->inputbox->toPlainText().toStdString();

    //剔除空字符
    exptext.erase(std::remove_if(exptext.begin(), exptext.end(), ::isspace), exptext.end());
    // 判定输入内容是否有效
    if(exptext.empty())
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }

    int xvalue = 0; // 对于多项式的 x 的赋值，默认为 0
    std::string numtext = ui->xvalueinputbox->toPlainText().toStdString();
    //剔除空字符
    numtext.erase(std::remove_if(numtext.begin(), numtext.end(), ::isspace), numtext.end());
    if(!numtext.empty() && !isInteger(numtext))  // 如果输入了文字，且输入数字无效
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }
    if(!numtext.empty()) xvalue = std::stoi(numtext);

    Calculator calc(exptext, xvalue);
    if(!calc.Getisvalid())
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }

    ui->outputbox->setPlainText(QString::fromStdString(calc.GetResult()));
}


/************     Enter响应 submitbutton     ************/
bool Widget::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->inputbox)		//可替换
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

            if(k->key() == Qt::Key_Return)
            {
                SubmitClicked();		//替换为需要响应的函数事件，以这里的按钮为例
                return true;
            }
        }
    }

    if(target == ui->xvalueinputbox)		//可替换
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

            if(k->key() == Qt::Key_Return)
            {
                SubmitClicked();		//替换为需要响应的函数事件，以这里的按钮为例
                return true;
            }
        }
    }

    if (event->type() == QEvent::MouseButtonPress) {
        if (target == ui->inputbox)
        {
            // 鼠标点击了文本编辑框
            ui->inputbox->setFocus();
        }
        else if (target == ui->xvalueinputbox)
        {
            // 鼠标点击了单行输入框
            ui->xvalueinputbox->setFocus();
        }
    }


    return QWidget::eventFilter(target,event);
}



/******      判断输入是否有效      ******/
bool Widget::isInteger(std::string& s)
{
    for(int i = 0; i<(int)s.length(); i++)
    {
        if(!std::isdigit(s[i])) return false;
    }
    return true;
}



/******   普通按钮：每个按钮对应一个 输入或操作      ******/

void Widget::ClearClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->clear();
}
void Widget::BackClicked()
{
        ui->inputbox->setFocus();
        ui->inputbox->textCursor ().deletePreviousChar();
}

void Widget::ZeroClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("0");
}
void Widget::OneClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("1");
}
void Widget::TwoClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("2");
}
void Widget::ThreeClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("3");
}
void Widget::FourClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("4");
}
void Widget::FiveClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("5");
}
void Widget::SixClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("6");
}
void Widget::SevenClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("7");
}
void Widget::EightClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("8");
}
void Widget::NineClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("9");
}
void Widget::XButtonClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("x");
}
void Widget::AddClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("+");
}
void Widget::SubtractClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("-");
}
void Widget::PowerClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("^()");
    QTextCursor cursor = ui->inputbox->textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    ui->inputbox->setTextCursor(cursor);
}
void Widget::ProductClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("*");
}
void Widget::DivisionClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("/");
}
void Widget::ParenthesisClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("()");
    QTextCursor cursor = ui->inputbox->textCursor();
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    ui->inputbox->setTextCursor(cursor);
}

void Widget::CloseClicked()
{
    this->close();
}


void Widget::IntroductionClicked()
{
    QMessageBox::information(this, "说明","输入说明：可以输入普通的数字表达式，数字只能是整数范围内的，可以输入多项式，默认参数为字母x，x的值默认为0，系数只能是整数。x的值只能键盘输入\n"
                                             "在 xvalue框中只能输入整数\n"
                                             "表达式输入示例：3*(7-2)、88-1*5^(2)+2、1024/(4*8)、1024/4*8、(-20+2)*(6/2)、3-3-3、8/(9-9)、2*(6+2*(3+6*(6+6)))、(((6+6)*6+3)*2+6)*2\n"
                                           "多项式输入示例：2*(x+2*(3+x*(x+x)))");
}

void Widget::NightClicked()
{
    if(!isNightModel)
    {
        ui->titleText->setStyleSheet("color:rgb(255, 254, 246)");
        this->setStyleSheet("background-color:rgb(73, 104, 186); color:rgb(255, 254, 246);");
        ui->inputbox->setStyleSheet("background-color:rgb(54, 79, 107);");
        ui->outputbox->setStyleSheet("background-color:rgb(54, 79, 107);");
        ui->xvalueinputbox->setStyleSheet("background-color:rgb(54, 79, 107);");
        ui->inputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->outputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->xvaluelabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->nightmodelbutton->setText(QString::fromStdString("☀️"));
        isNightModel = true;
    }
    else
    {
        ui->titleText->setStyleSheet("color:rgb(73, 104, 186)");
        this->setStyleSheet("background-color:rgb(255, 254, 246); color:rgb(0, 0, 0)");
        ui->inputbox->setStyleSheet("background-color:rgb(255, 255, 255);");
        ui->xvalueinputbox->setStyleSheet("background-color:rgb(255, 255, 255)");
        ui->outputbox->setStyleSheet("background-color:rgb(255, 255, 255)");
        ui->outputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->inputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->xvaluelabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->nightmodelbutton->setText(QString::fromStdString("🌙"));
        isNightModel = false;
    }
}





Widget::~Widget()
{
    delete ui;
}

