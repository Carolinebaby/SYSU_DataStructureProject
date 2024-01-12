#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), treeGraph(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/favcion.png"));
    ui->inputBox->setFocus();

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(CloseClicked()));
    connect(ui->infixexprButton, SIGNAL(clicked()), this, SLOT(GetInfix()));
    connect(ui->getvalueButton, SIGNAL(clicked()), this, SLOT(GetExprValue()));
    connect(ui->mergeconstButton, SIGNAL(clicked()), this, SLOT(MergeConst()));
    connect(ui->compoundButton, SIGNAL(clicked()), this, SLOT(CompoundExpr()));
    connect(ui->treegraphButton, SIGNAL(clicked()), this, SLOT(ShowTreeGraph()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(ClearClicked()));
}


/******      获取中缀表达式      ******/
void Widget::GetInfix()
{
    std::string inputText = ui->inputBox->toPlainText().toStdString();
    if(!isInputValid(inputText))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    Expression E(inputText);
    std::string outputText;
    if(E.isValid())
        outputText = E.GetInfix();
    else
        outputText = "invalid input";

    ui->outputBox->setPlainText(QString::fromStdString(outputText));
    ui->inputBox->setFocus();
}

/******      获取赋值后表达式的值      ******/
void Widget::GetExprValue()
{
    std::string inputText = ui->inputBox->toPlainText().toStdString();
    if(!isInputValid(inputText))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    Expression E(inputText);
    if(!E.isValid())
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    std::string varValInput = ui->varValueInputBox->text().toStdString();
    double result = E.GetExpValue(varValInput);
    QString outputText;
    if(E.isCalValid() && E.isValid())
        outputText = QString::fromStdString(E.GetInfix()) + "\nthe value of the expression is:" + QString::number(result);
    else
        outputText = "invalid input";

    ui->outputBox->setPlainText(outputText);
    ui->inputBox->setFocus();
}

/******      合并常数运算      ******/
void Widget::MergeConst()
{
    std::string inputText = ui->inputBox->toPlainText().toStdString();
    if(!isInputValid(inputText))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }

    Expression E(inputText);
    std::string outputText;
    if(E.isValid())
    {
        outputText = E.GetInfix() + "\n" + "merge constant operation: ";
        E.MergeConst();
        if(!E.isValid())
        {
            outputText = "invalid input";
        }
        else
            outputText += E.GetInfix();
    }
    else
        outputText = "invalid input";

    ui->outputBox->setPlainText(QString::fromStdString(outputText));
    ui->inputBox->setFocus();
}

/******      合并两个表达式      ******/
void Widget::CompoundExpr()
{
    QStringList inputText = ui->inputBox->toPlainText().split("\n");
    if(inputText.size() <= 2)
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }

    std::string expr1 = inputText[0].toStdString();
    if(!isInputValid(expr1))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    std::string op = inputText[1].toStdString();
    if(op.size() != 1 ||(op[0] != '+' && op[0] != '*' && op[0] !='/' && op[0] != '^' && op[0] != '-'))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    std::string expr2 = inputText[2].toStdString();
    if(!isInputValid(expr2))
    {
        ui->outputBox->setPlainText("invalid input");
        return;
    }
    Expression E1(expr1), E2(expr2);
    Expression cE;
    E1.CompoundExpr(cE, op, E2);

    std::string outputText;
    if(E1.isValid() && E2.isValid() && cE.isValid())
    {
        outputText = "1: " + E1.GetInfix() + "\n2: "
                     + E2.GetInfix() + "\n1"+op+"2: " + cE.GetInfix();
    }
    else
        outputText = "Invalid input expression";

    ui->outputBox->setPlainText(QString::fromStdString(outputText));
    ui->inputBox->setFocus();
}

/******      显示树状图      ******/
void Widget::ShowTreeGraph()
{
    std::string inputText = ui->inputBox->toPlainText().toStdString();
    if(!isInputValid(inputText))
    {
        QMessageBox::warning(this, "Warning", "invalid input");
        return;
    }
    Expression E(inputText);
    std::string outputText;
    ExpNode* root = nullptr;
    if(E.isValid())
        root = E.GetNodes();
    else
    {
        QMessageBox::warning(this, "Warning", "invalid input");
        return;
    }

    if(!root)
    {
        QMessageBox::warning(this, "Warning", "invalid input");
        return;
    }

    //窗口显示
    if(treeGraph) delete treeGraph;
    treeGraph = new TreeGraph(root);
    treeGraph->show();
}

/******      判断是否是数字      ******/
bool Widget::isNum(std::string s)
{
    for (char c : s)
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

/******      判断输入是否有效，只进行一些基础的字符判断，对前缀表达式格式的判断在 expression 类中进行      ******/
bool Widget::isInputValid(std::string s)
{
    if(s.length() == 0) return false;
    std::istringstream iss(s);

    std::string word;
    while (iss >> word)
    {
        if(word.length() == 1)
        {
            if(word[0] >= '0' && word[0] <= '9') continue;
            else if(word[0] >= 'a' && word[0] <= 'z') continue;
            else if(word[0] == '-' || word[0] == '+' || word[0] == '*' || word[0] == '/' || word[0] == '^') continue;
            else return false;
        }
        else if(word[0] == 's' || word[0] == 'c')
        {
            if(word.length() <6) return false;
            if((word[0] == 's' && word[1] == 'i' && word[2] == 'n' && word[3] == '(' && word[word.length()-1] == ')') ||
                (word[0] == 'c' && word[1] == 'o' && word[2] == 's' && word[3] == '(' && word[word.length()-1] == ')'))
            {
                std::string subs = word.substr(4,word.length()-5);
                if(subs.length() == 1 && word[4]>='a' && word[4] <='z')	continue;
                else if(!isNum(subs)) return false;
            }
            else return false;
        }
        else if(!isNum(word))
        {
            return false;
        }

    }
    return true;
}

/******      化简数字字符串      ******/
std::string Widget::SimplifyNumString(std::string s)
{
    for(int i = s.length()-1; i>=0; i--)
    {
        if(s[i] == '0' || s[i] == '.')
        {
            auto temp = s[i];
            s.erase(i);

            if(temp == '.')
                return s;
        }
        else
            return s;
    }
    return std::string("");
}

/******      关闭按钮点击      ******/
void Widget::CloseClicked()
{
    this->close();
}

/******      清空输入输出框按钮点击      ******/
void Widget::ClearClicked()
{
    ui->inputBox->clear();
    ui->outputBox->clear();
    ui->inputBox->setFocus();
}

/******      析构函数      ******/
Widget::~Widget()
{
    delete ui;
}
