#include "expression.h"

/******      基本函数      ******/
Expression::Expression(): isvalid(false),expRoot(nullptr){}

Expression::Expression(std::string prestr):isvalid(true), prefix(prestr), expRoot(nullptr)
{
    // 处理前后空白字符
    CutHeadAndTailSpace(prefix);

    if(!prefix.length())
    {
        isvalid = false;
        return;
    }

    PrefixToNodes();
}

Expression::~Expression()
{
    DeleteNodes(expRoot);
}

/******      获取中缀表达式      ******/
std::string Expression::GetInfix()
{
    if(infix.length())infix.clear();
    NodesToInfix(expRoot);
    return infix;
}

/******      获取结点，在绘图时候用到      ******/
ExpNode* Expression::GetNodes()
{
    return expRoot;
}

/******      获取赋值后表达式的值      ******/
double Expression::GetExpValue(std::string varInput)
{
    std::istringstream iss(varInput);
    std::map<char, double> varVal;
    char c;
    char equalop;
    double value;

    while(iss >> c >> equalop >> value)
    {
        if( !isLetter(c) || equalop != '=')
        {
            isCalvalid = false;
            return 0;
        }
        varVal.emplace(c, value);
        if(!(iss>>c)) break;
    }
    isCalvalid = true;
    return CalExpValue(expRoot, varVal);
}

/******      合并表达式      ******/
void Expression::CompoundExpr(Expression &newE, std::string op, Expression &E)
{
    std::string newPrefix = op+" "+prefix+" " +E.prefix;
    newE.SetPrefix(newPrefix);
}

/******      合并常量运算      ******/
void Expression::MergeConst()
{
    if(!isvalid) return;
    MergeConst(expRoot);
}

/******      判断有效性的函数      ******/
bool Expression::isValid()    // 判断表达式的有效性
{
    return isvalid;
}
bool Expression::isCalValid() // 判断赋值表达式以及赋值后表达式的有效性
{
    return isCalvalid;
}


/****************************实现主要功能的函数********************************/

/******      前缀表达式转为二叉树      ******/
void Expression::PrefixToNodes()
{
    std::istringstream iss(prefix);
    std::string c;

    if(!isOperator(prefix[0]))
    {
        if(isTriFunc(prefix))
        {
            std::string subs = prefix.substr(4,prefix.length()-6);
            if(!isNum(subs) && !isLetter(subs))
            {
                isvalid = false;
                return;
            }
        }
        else if(!isNum(prefix) && !isLetter(prefix))
        {
            isvalid = false;
            return;
        }
        expRoot = new ExpNode(prefix);
        return;
    }

    std::stack<ExpNode*> stk;
    while(iss >> c)
    {
        ExpNode* term = new ExpNode(c);

        if(!term->isOperator())
        {
            if(stk.empty())
            {
                isvalid = false;
                return;
            }

            if(stk.top()->left && stk.top()->right)
            {
                ExpNode* preop = stk.top(); stk.pop();
                if(stk.empty() || stk.top()->left || stk.top()->right)
                {
                    isvalid = false;
                    return;
                }
                stk.top()->left = preop;
            }

            if(stk.top()->left)
            {
                stk.top()->right = term;
                while(stk.size() > 1)
                {
                    ExpNode* op = stk.top(); stk.pop();
                    if(stk.top()->left && stk.top()->right)
                    {
                        ExpNode* preop = stk.top(); stk.pop();
                        if(stk.empty())
                        {
                            isvalid = false;
                            return;
                        }
                        stk.top()->left = preop;
                        stk.top()->right = op;
                    }
                    else if(stk.top()->left)
                    {
                        stk.top()->right = op;
                    }
                    else
                    {
                        stk.emplace(op);
                        break;
                    }
                }
            }
            else stk.top()->left = term;
        }
        else
        {
            stk.emplace(term);
        }
    }

    if(stk.size() != 1 || !stk.top()->left || !stk.top()->right) isvalid = false;

    expRoot = stk.top();
}

/******      表达式二叉树转为中缀表达式      ******/
void Expression::NodesToInfix(ExpNode* root)
{

    if(!root->isOperator())
    {
        infix += root->c;
    }
    else
    {
        if(root != expRoot)
            infix += "(";
        NodesToInfix(root->left);
        infix += root->c;
        NodesToInfix(root->right);
        if(root != expRoot)
            infix += ")";
    }
}

/******      计算赋值后表达式的值      ******/
double Expression::CalExpValue(ExpNode* root, std::map<char, double> varVal)
{
    if(!isCalvalid) return 0;

    if(root->isTriFunc())
    {
        std::string str = root->c.substr(4,root->c.length()-5);
        if(str.size() == 1 && isLetter(str[0]))
        {
            double var;
            if(!varVal.count(str[0])) var = 0;
            else var = varVal[str[0]];
            if(root->c[0] == 's')
                return sin(var);
            else
                return cos(var);
        }
        else if(isNum(str))
        {
            if(root->c[0] == 's')
                return sin(StringToNum(str));
            else
                return cos(StringToNum(str));
        }
        else
        {
            isCalvalid = false;
            return 0;
        }
    }

    if(root->isNum()) return StringToNum(root->c);
    if(root->isLetter())
    {
        if(!varVal.count(root->c[0])) return 0;
        return varVal[root->c[0]];
    }

    double num1, num2;
    num1 = CalExpValue(root->left, varVal);
    num2 = CalExpValue(root->right, varVal);
    if(!isCalvalid) return 0;

    if(root->c == "+") return num1+num2;
    else if(root->c == "-") return num1-num2;
    else if(root->c == "*") return num1*num2;
    else if(root->c == "^")
    {
        if(num1 < 1e-9 && num2 < 0)
        {
            isCalvalid = false;
            return 0;
        }
        return pow(num1, num2);
    }
    else
    {
        if(num2 < 1e-9)
        {
            isCalvalid = false;
            return 0;
        }
        return num1/num2;
    }

}

/******      设置前缀表达式      ******/
void Expression::SetPrefix(std::string prestr)
{
    prefix = prestr;

    CutHeadAndTailSpace(prefix);

    isvalid = true;
    if(expRoot) DeleteNodes(expRoot);
    expRoot = nullptr;
    PrefixToNodes();
}

/******      合并常数运算      ******/
void Expression::MergeConst(ExpNode* &root)
{
    if(!isvalid) return;
    if(!root)return;
    if(root->isNum()) return;
    if(root->isTriFunc())
    {
        std::string subs = root->c.substr(4,root->c.length()-5);
        if(isNum(subs))
        {
            double val;
            if(root->c[0] == 's') val = sin(StringToNum(subs));
            else val = cos(StringToNum(subs));
            root->c = std::to_string(val);
        }

        return;
    }

    MergeConst(root->left);
    MergeConst(root->right);

    if(root->left && root->right)
    {
        if(root->left->isNum() && root->right->isNum())
        {
            // double num1 = stod(root->left->c);
            // double num2 = stod(root->right->c);
            double num1 = StringToNum(root->left->c);
            double num2 = StringToNum(root->right->c);
            double result;
            if(root->c == "+") result = num1+num2;
            else if(root->c == "-") result = num1-num2;
            else if(root->c == "*") result = num1*num2;
            else if(root->c == "^")
            {
                if(num1 < 1e-9 && num2 < 0)
                {
                    isvalid = false;
                    return;
                }
                result = pow(num1, num2);
            }
            else
            {
                if(num2 < 1e-7)
                {
                    isvalid = false;
                    return;
                }

                result = num1/num2;
            }

            root->c = SimplifyNumString(std::to_string(result));
            delete root->right;
            delete root->left;
            root->left = nullptr;
            root->right = nullptr;
        }
    }
}

/******      删除结点      ******/
void Expression::DeleteNodes(ExpNode* &root)
{
    if(!root) return;
    if(root->left)
    {
        DeleteNodes(root->left);
        root->left = nullptr;
    }
    if(root->right)
    {
        DeleteNodes(root->right);
        root->right = nullptr;
    }
    delete root;
}

/******      化简数字字符串      ******/
std::string Expression::SimplifyNumString(std::string s)
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

/******      判断 string 类是否是数字      ******/
bool Expression::isNum(std::string& c)
{
    int dotcnt = 0;
    int i = 0;
    if(c[0] == '-') i++;
    for(; i<(int)(c.length()); i++)
    {
        if(c[i]>='0' && c[i] <= '0'+9 )
        {
            continue;
        }
        else if(c[i] == '.')
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

bool Expression::isNum(char c)
{
    return c>='0' && c<='9';
}

bool Expression::isLetter(std::string &c)
{
    if(c.length() > 1) return false;
    if(c[0] < 'a' || c[0] >'z') return false;
    return true;
}

bool Expression::isLetter(char c)
{
    if(c < 'a' || c >'z') return false;
    return true;
}

bool Expression::isOperator(std::string &c)
{
    if(c.length() > 1) return false;
    if(c[0] != '-' && c[0] != '+' && c[0] != '*' && c[0] != '^' && c[0] != '/') return false;
    return true;
}

bool Expression::isOperator(char c)
{
    if(c != '-' && c != '+' && c != '*' && c != '^' && c != '/') return false;
    return true;
}

void Expression::CutHeadAndTailSpace(std::string& s)
{
    int spaceCount = 0;
    while(std::isspace(s[spaceCount])) spaceCount++;
    s = s.substr(spaceCount, s.length()-spaceCount);
    spaceCount = 0;
    while(std::isspace(s[s.length()-spaceCount-1])) spaceCount++;
    s = s.substr(0, s.length()-spaceCount);
}

bool Expression::isTriFunc(std::string c)
{
    if(c.length()<6) return false;
    std::string str = c.substr(0, 3);
    if((str == "sin" || str == "cos") && c[3] == '(' && c[c.length()-1] == ')')
    {
        return true;
    }
    return false;
}

// 把 string 类转为数字
double Expression::StringToNum(std::string& s)
{
    int start = 0;
    int sign = 1;
    if(s[0] == '-')
    {
        sign = -1;
        start = 1;
    }
    else if(s[0] == '+')
    {
        sign = 1;
        start = 1;
    }
    return sign * std::stod(s.substr(start, s.length()-start));
}

// 将 char 类转为数字
double Expression::StringToNum(char& s)
{
    return s-'0';
}
