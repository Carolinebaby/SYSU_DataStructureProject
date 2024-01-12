#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    std::string outputtext = "没有任何提交，请输入内容进行提交，结果将在这里显示。";
    ui->outputbox->setPlainText(QString::fromStdString(outputtext));

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
    connect(ui->dotbutton,SIGNAL(clicked()), this, SLOT(DotClicked()));
    connect(ui->leftparenthesis,SIGNAL(clicked()), this, SLOT(LeftClicked()));
    connect(ui->rightparenthesis,SIGNAL(clicked()), this, SLOT(RightClicked()));
    connect(ui->nightmodelbutton,SIGNAL(clicked()), this, SLOT(NightClicked()));
    connect(ui->parameterbutton,SIGNAL(clicked()), this, SLOT(ParameterClicked()));
    connect(ui->derivationbutton,SIGNAL(clicked()), this, SLOT(DerivationClicked()));
    connect(ui->integralbutton,SIGNAL(clicked()), this, SLOT(IntegralClicked()));
    connect(ui->getvaluebutton, SIGNAL(clicked()), this, SLOT(GetValueClicked()));
    connect(ui->equalbutton,SIGNAL(clicked()), this, SLOT(SubmitClicked()));
    ui->inputbox->installEventFilter(this);
    connect(ui->introductionbutton,  SIGNAL(clicked()), this, SLOT(IntroductionClicked()));
    connect(ui->closebutton, SIGNAL(clicked()), this, SLOT(CloseClicked()));
}


/************     提交      ************/
void Widget::SubmitClicked()
{
    ui->xvalueinputbox->clear();
    ui->fxoutputbox->clear();
    ui->outputbox->clear();
    // 保持光标一直存在
    ui->inputbox->setFocus();
    // 读取内容
    std::string polytext = ui->inputbox->toPlainText().toStdString();

    //剔除空字符
    polytext.erase(std::remove_if(polytext.begin(), polytext.end(), ::isspace), polytext.end());
    // 判定输入内容是否有效
    if(!isValid(polytext) || polytext.empty())
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }
    // 把字符导入计算器类创建对象
    Calculator poly(polytext);
    // 输出结果
    ui->outputbox->setPlainText(QString::fromStdString(poly.GetSimplifiedFx()));
    ui->outputbox->insertPlainText("f(x) = ");

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
    return QWidget::eventFilter(target,event);
}

/************     得到导函数      ************/
void Widget::DerivationClicked()
{
    ui->outputbox->clear();
    // 保持光标一直存在
    ui->inputbox->setFocus();
    // 读取内容
    std::string polytext = ui->inputbox->toPlainText().toStdString();

    //剔除空字符
    polytext.erase(std::remove_if(polytext.begin(), polytext.end(), ::isspace), polytext.end());
    // 判定输入内容是否有效
    if(!isValid(polytext)||polytext.empty())
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }
    // 把字符导入计算器类创建对象
    Calculator poly(polytext);
    // 输出结果
    ui->outputbox->setPlainText(QString::fromStdString(poly.GetDF()));
    ui->outputbox->insertPlainText("f(x)' = ");
}


/************     给定x=value，点击获取f(x)值      ************/
void Widget::GetValueClicked()
{
    ui->fxoutputbox->clear();
    ui->outputbox->clear();

    std::string numtext = ui->xvalueinputbox->toPlainText().toStdString();
    std::string polytext = ui->inputbox->toPlainText().toStdString();
    //剔除空字符
    polytext.erase(std::remove_if(polytext.begin(), polytext.end(), ::isspace), polytext.end());

    if(!isValid(polytext) || polytext.empty())
    {
        ui->outputbox->setPlainText(QString("多项式输入无效，请重新输入"));
        ui->fxoutputbox->setPlainText(QString("输入无效"));
        return;
    }

    ui->outputbox->clear();
    Calculator poly(polytext);
    ui->outputbox->setPlainText(QString::fromStdString(poly.GetSimplifiedFx()));
    ui->outputbox->insertPlainText("f(x) = ");

    if(!isNum(numtext) || numtext.empty())  // 判断输入是否有效
    {

        ui->fxoutputbox->setPlainText(QString("x值输入无效"));
    }
    else
    {
        std::string result = poly.GetFxValue(std::stod(numtext));
        ui->fxoutputbox->setPlainText(QString::fromStdString(result));
    }
}


/************      点击 积分按钮，求积分      ************/
void Widget::IntegralClicked()
{
    ui->outputbox->clear();
    // 保持光标一直存在
    ui->inputbox->setFocus();
    // 读取内容
    std::string polytext = ui->inputbox->toPlainText().toStdString();

    //剔除空字符
    polytext.erase(std::remove_if(polytext.begin(), polytext.end(), ::isspace), polytext.end());
    // 判定输入内容是否有效
    if(!isValid(polytext)||polytext.empty())
    {
        ui->outputbox->setPlainText(QString("输入无效，请重新输入"));
        return;
    }
    // 把字符导入计算器类创建对象
    Calculator poly(polytext);
    // 输出结果
    ui->outputbox->setPlainText(QString::fromStdString(poly.GetIntFx()));
    ui->outputbox->insertPlainText("∫f(x)dx = ");
}


/******      判断是否为数字      ******/
bool Widget::isNum(std::string& s)
{
    int dotcnt = 0;
    for(int i = 0; i<(int)(s.length()); i++)
    {
        if(s[i]>='0' && s[i] <= '0'+9 )
        {
            continue;
        }
        else if(s[i] == '.')
        {
            dotcnt++;
        }
        else
        {
            return false;
        }
    }

    if(dotcnt > 1)
        return false;

    return true;
}



/******      判断输入是否有效      ******/
bool Widget::isValid(std::string& text)
{
    // 判断 括号是否匹配
    std::stack<char> stk;
    int cnt = 0;  //包围多项式括号的数目
    for(int i = 0; i<(int)(text.length()); i++)
    {
        if(text[i] == '(') // 左括号，入栈
        {
            stk.push(i);  // 用 i 代表左括号
        }
        else if(text[i] == ')') //右括号
        {
            if(stk.empty()) //若为空，即不存在匹配的左括号
                return false;  // 输入无效
            else
            {
                int idx = stk.top();
                stk.pop();
                if(stk.empty())  // 说明是外围的括号
                {
                    if(idx == 0 || text[idx-1] != '^') // 如果左括号在首位，或者其他位置且不是指数的括号
                    {
                        cnt++;
                    }
                }
            }

        }
        if(cnt == 2 && i != (int)(text.length()-1)) return false;
    }
    if(!stk.empty()) return false;  // 若不为空，说明有左括号不匹配

    // 判断字符位置，字符类型是否合规
    for(int i = 0; i < (int)(text.length()); i++)
    {
        if(std::isdigit(text[i]))// 数字
            continue;
        else if(text[i] == 'x')  //参数
        {
            if(i>0 && text[i-1] != '(' && text[i-1] != '+' && text[i-1] != '-' && text[i-1] != '*' )
            {
                return false;
            }
            else if(i < (int)(text.length()-1) && text[i+1] != '^' && text[i+1] != '+' && text[i+1] != '-' && text[i+1] != ')' )
            {
                return false;
            }
        }
        else if(text[i] == '+' || text[i] == '-') // 加或减
        {
            if(i == (int)(text.length()-1)) //放在最后
                return false;
            else if(i>0 && text[i-1] != ')' && text[i-1] != '(' && !std::isdigit(text[i-1]) && text[i-1] != 'x') // 前一个字符可以是：x,数字,）,(
                return false;
            else if(text[i+1] != '(' && text[i+1] != 'x' && !std::isdigit(text[i+1]) ) // 后一个字符可以是：x,数字,(,
                return false;
        }
        else if(text[i] == '*')
        {
            if(i == 0 || i == (int)(text.length()-1))
            {
                return false;
            }
            else if(text[i-1] != ')' && !std::isdigit(text[i-1]) && text[i-1] != 'x')
                return false;
            else if(text[i+1] != '(' && text[i+1] != 'x' && !std::isdigit(text[i+1]) )
                return false;
            else if(text[i-1] == ')' && text[i+1] == 'x')
            {
                std::stack<int> stk;
                int j = i-1;
                for(; j>=0; j--)
                {
                    if(text[j] == ')')
                    {
                        stk.push(j);
                    }
                    else if(text[j] == '(')
                    {
                        stk.pop();
                        if(stk.empty())
                        {
                            break;
                        }
                    }
                }
                std::string mid = text.substr(j+1,i-j-2);
                if(isNum(mid))
                {
                    return false;
                }
            }
        }
        else if(text[i] == '(')
        {
            if(i == (int)(text.length()-1)) return false;
            else if(text[i+1] != 'x' && text[i+1] !='(' && !std::isdigit(text[i+1]) && text[i+1] != '+' && text[i+1] !='-')
            {
                return false;
            }
        }
        else if(text[i] == ')')
        {
            if(i == 0) return false;
            else if(text[i-1] != 'x' && text[i-1] != ')' && !std::isdigit(text[i-1]))
            {
                return false;
            }
        }
        else if(text[i] == '.') // 小数点
        {
            if(i == 0 || i == (int)(text.length()-1))
            {
                return false;
            }
            else if((text[i-1] > '0'+9 || text[i-1] < '0') || !std::isdigit(text[i+1]))
            {
                return false;
            }
        }
        else if(text[i] == '^')
        {
            if(i == 0 || i == (int)(text.length()-1))
            {
                return false;
            }
            else if(text[i-1] != 'x' || (!std::isdigit(text[i+1]) && text[i+1] != '('))
                return false;
            else if( i<=(int)(text.length()-3) && (std::isdigit(text[i+1]) && std::isdigit(text[i+2])))
                return false;
        }
        else
            return false;
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
void Widget::DotClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText(".");
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
void Widget::LeftClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("(");
}
void Widget::RightClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText(")");
}
void Widget::ParameterClicked()
{
    ui->inputbox->setFocus();
    ui->inputbox->textCursor().insertText("x");
}
void Widget::CloseClicked()
{
    this->close();
}

void Widget::IntroductionClicked()
{
    QMessageBox::information(this, "说明","请输入一个有效多项式，或两个多项式相加相减相乘，再点击 = 按钮 或 按 Enter 键，程序会根据你的输入在输出框中显示结果。\n"
                                             "示例：多项式相加：(2*x+5*x^8-3.1*x^(11))+(7-5*x^8+11*x^9)\n多项式相减：(6*x^(-3)-x+4.4*x^2-1.2*x^9)-(-6*x^(-3)+5.4*x^3-x^2+7.8*x^(15))\n"
                                             "多项式相乘：(2*x+5*x^8-3.1*x^(11))*(7-5*x^8+11*x^9)\n多项式求导：必须现在输入框中输入有效多项式，再点击导数按钮，求导结果会在输出框中显示。\n多项式求积分：必须现在输入框中输入有效多项式，再点击积分按钮，求积分结果会在输出框中显示。\n"
                                             "给定x值，求多项式的值：在输入框输入多项式，再在\"x=\"输入框中输入x的值(必须是数字)，再点击求值按钮，f(x)的结果会在\"f(x)=\"的输出框。\n"
                                             "【输入有效性说明❗】\n"
                                             "● 本程序只针对一个或两个多项式进行操作，无法对三个及三个以上的多项式进行操作。输入的两个多项式，可以有一个没有括号包围。例如：6*x^(-3)-x+4.4*x^2-1.2*x^9-(-6*x^(-3)+5.4*x^3-x^2+7.8*x^(15)) 或 (x+x^(100)) + x^(100) + x^(200)"
                                             "，但是不能出现包围着多项式的两对括号后面还有其他字符\n"
                                             "● 多项式中的系数不能用括号包围，例如：(-4)*x ，必须写为-4*x 。且必须用 * 与未知数相连，除非指数为0\n"
                                             "● 对于负的指数和大于等于10的指数必须用括号包围，例如：x^(-3)或x^(22)\n"
                                             "● 对于两个多项式相乘，至少要有一个包围多项式的括号，x^(100)*x^(100)无效x^(100)*(x+x^(100))有效\n"
                                             "【其他输入示例】\n"
                                             " (2*x+5*x^8-3.1*x^(11))+(7-5*x^8+11*x^9)\n"
                                             " (6*x^(-3)-x+4.4*x^2-1.2*x^9)-(-6*x^(-3)+5.4*x^3-x^2+7.8*x^(15))\n"
                                             " (1+x+x^2+x^3+x^4+x^5+x^6)+(-x^3-x^4)\n"
                                             " (x+x^3)+(-x-x^3)\n"
                                             " (x+x^(100)) + (x^(100) + x^(200))\n"
                                             " (x+x^2+x^3)+0\n"
                                             " 1+(x+x^2+x^3)");
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
        ui->fxoutputbox->setStyleSheet("background-color:rgb(54, 79, 107);");
        ui->inputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->outputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->xvaluelabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->fxlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid white;");
        ui->nightmodelbutton->setText(QString::fromStdString("☀️"));
        isNightModel = true;
    }
    else
    {
        ui->titleText->setStyleSheet("color:rgb(73, 104, 186)");
        this->setStyleSheet("background-color:rgb(255, 254, 246); color:rgb(0, 0, 0)");
        ui->inputbox->setStyleSheet("background-color:rgb(255, 255, 255);");
        ui->outputbox->setStyleSheet("background-color:rgb(255, 255, 255)");
        ui->xvalueinputbox->setStyleSheet("background-color:rgb(255, 255, 255)");
        ui->fxoutputbox->setStyleSheet("background-color:rgb(255, 255, 255)");
        ui->outputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->xvaluelabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->fxlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->inputlabel->setStyleSheet("color:rgb(0, 0, 0);background-color:rgb(255, 229, 241);border: 1px solid black;");
        ui->nightmodelbutton->setText(QString::fromStdString("🌙"));
        isNightModel = false;
    }
}


Widget::~Widget()
{
    delete ui;
}

