#include "calculator.h"

// 静态变量初始化
std::map<char, int> Calculator::opidx = {
    {'+', 0},
    {'-', 1},
    {'*', 2},
    {'/', 3},
    {'(', 4},
    {')', 5},
    {'#', 6},
    {'^', 7}
};
int Calculator::isHigher[8][8] = {
    {-1, -1, 1, 1, 1, -1, -1, 1},
    {-1, -1, 1, 1, 1, -1, -1, 1},
    {-1, -1, -1, -1, 1, -1, -1, 1},
    {-1, -1, -1, -1, 1, -1, -1, 1},
    {1, 1, 1, 1, 1, 0, 2, 1},
    {-1, -1, -1, -1, 2, -1, -1, -1},
    {1, 1, 1, 1, 1, 2, 0, 1},
    {-1, -1, -1, -1, 1, -1, 1, 1}
};

/******      构造函数     ******/
Calculator::Calculator(std::string& exp, int x):exp(exp)
{
    isvalid = isValid(x); // 在判断的时候，可以先判断多项式的合理性，再把多项式转化为普通的整数算术表达式。如果是普通的整数表达式，可以直接判断有效性
    if(!isvalid) return;
    isvalid = ExpCalc() && isvalid;
}
/******      获取计算结果的函数     ******/
std::string Calculator::GetResult()
{
    return NumSimplify(std::to_string(expresult));
}

bool Calculator::Getisvalid()
{
    return isvalid;
}

/******      计算功能的主要函数     ******/
bool Calculator::ExpCalc()
{
    std::stack<char> opstk;  // 存放 运算符的 栈
    std::stack<double> numstk;  // 存放 数字 的栈

    opstk.emplace('#');  // 栈底标识

    // 遍历计算
    for(int i = 0; i<(int)exp.length(); i++)
    {
        if(std::isdigit(exp[i])) // 如果是数字
        {
            int start = i; // 数字的起始位置
            int cnt = 0;
            while(std::isdigit(exp[i]))
            {
                i++;
                cnt++;
            } // 获取数字区域的范围

            auto numstr = exp.substr(start,cnt);  // 得到数字字符串
            int num = StringToNum(numstr); // 转换数字字符串
            numstk.emplace(num); //把得到的结果压入栈中
            i--; // 防止 i 多加了一次
            continue; // 如果是数字，跳过后面操作
        }
        else if((exp[i] == '-' || exp[i] == '+') && (i == 0 || exp[i-1] == '(')) // 如果是 单目减或加，把 0 压入栈中，可以避免特殊单独处理
        {
            numstk.emplace(0);
        }

        // 运算符，开始栈顶运算符 和 当前遍历到的运算符 进行比较
        while(true)
        {
            int comp = isHigher[opidx[opstk.top()]][opidx[exp[i]]];
            if(comp == 1)  // 当前运算符优先级更高，则将当前运算符压入栈中
            {
                opstk.emplace((char)exp[i]);
                break;
            }
            else if(comp == 0)  // 优先级相等，弹栈
            {
                opstk.pop();
                break;
            }
            else if(comp == -1)  // 当前运算符优先级低，对运算符栈和数字栈进行弹栈，对弹出的运算符进行判断，然后对数字进行相应的操作
            {
                if(!CalcNum(numstk, opstk))  //如果运算失败
                    return false;
            }
        }
    }

    // 将运算符栈中所有运算符弹出，进行相应计算
    while(opstk.top() != '#')
    {
        if(!CalcNum(numstk, opstk))
            return false;
    }

    // 栈顶元素即为结果
    expresult = numstk.top();

    return true;
}


bool Calculator::CalcNum(std::stack<double>& numstk, std::stack<char>& opstk)
{
    // 弹栈操作
    double num1 = numstk.top();
    numstk.pop();
    double num2 = numstk.top();
    numstk.pop();
    char op = opstk.top();
    opstk.pop();

    double num = 0; // 结果
    // 对运算符进行判断，并对弹出两个数字进行相应的运算操作
    if(op == '+')
    {
        num = num1+num2;
    }
    else if(op == '-')
    {
        num = num2-num1;
    }
    else if(op == '*')
    {
        num = num1*num2;
    }
    else if(op == '/')
    {
        if(num1 == 0) return false;
        num = num2/num1;
    }
    else if(op == '^')
    {
        if(num2 == 0 && num1 < 0) return false;
        num = pow(num2, num1);
    }
    else
    {
        return false;
    }

    numstk.emplace(num); // 把得到的结果压入栈中

    return true;
}


/******      多项式赋值后，转化为表达式     ******/
void Calculator::PolyToExp(int xvalue)
{
    std::string numstr = std::to_string(xvalue);
    std::string result;
    for(int i = 0; i<(int)exp.length(); i++)
    {
        if(exp[i] == 'x')
        {
            result += numstr;
        }
        else
        {
            result += exp[i];
        }
    }
    exp = result;
}




/******      获取计算是否有效     ******/
bool Calculator::isValid(int xvalue)
{
    if(isPoly()) // 判断是多项式，则进行特殊的输入有效判断，然后代入 x 的值后，
    {
        for(int i = 0; i<(int)exp.length(); i++)
        {
            if(exp[i] == 'x')
            {
                if(i>0 && exp[i-1] != '-' && exp[i-1] != '*' && exp[i-1] != '+' && exp[i-1] != '^' && exp[i-1] != '(' && exp[i-1] != '/') return false;
                else if(i<(int)exp.length()-1 && exp[i+1] != '-' && exp[i+1] != '+' && exp[i+1] != '*' && exp[i+1] != '^' && exp[i+1] != ')' && exp[i+1] != '/') return false;
            }
        }
        PolyToExp(xvalue);  // 把多项式转化为普通的整数表达式
    }

    // 判断 括号是否匹配
    std::stack<char> stk;
    int cnt = 0;  //包围多项式括号的数目
    for(int i = 0; i<(int)(exp.length()); i++)
    {
        if(exp[i] == '(') // 左括号，入栈
        {
            stk.push(i);  // 用 i 代表左括号
        }
        else if(exp[i] == ')') //右括号
        {
            if(stk.empty()) //若为空，即不存在匹配的左括号
                return false;  // 输入无效
            else
            {
                int idx = stk.top();
                stk.pop();
                if(stk.empty())  // 说明是外围的括号
                {
                    if(idx == 0 || exp[idx-1] != '^') // 如果左括号在首位，或者其他位置且不是指数的括号
                    {
                        cnt++;
                    }
                }
            }

        }
        if(cnt == 2 && i != (int)(exp.length()-1)) return false;
    }
    if(!stk.empty()) return false;  // 若不为空，说明有左括号不匹配


    for(int i = 0; i < (int)(exp.length()); i++)
    {
        if(std::isdigit(exp[i])) // 数字
            continue;
        else if(exp[i] == '*' || exp[i] == '/')
        {
            if(i == 0 || i == (int)(exp.length()-1)) return false;
            else if(exp[i-1] != ')' && !std::isdigit(exp[i-1]) )
                return false;
            else if(exp[i+1] != '(' && !std::isdigit(exp[i+1]) )
                return false;
        }
        else if(exp[i] == '+' || exp[i] == '-')
        {
            if(i == (int)(exp.length()-1)) //放在最后
                return false;
            else if(i>0 && exp[i-1] != ')' && exp[i-1] != '(' && !std::isdigit(exp[i-1])) // 前一个字符可以是：x,数字,）,(
                return false;
            else if(exp[i+1] != '('&& !std::isdigit(exp[i+1]) ) // 后一个字符可以是：数字,(,
                return false;
        }
        else if(exp[i] == '^')
        {
            if(i == 0 || i == (int)(exp.length()-1))
            {
                return false;
            }
            else if(!std::isdigit(exp[i-1]) || (!std::isdigit(exp[i+1]) && exp[i+1] != '('))
                return false;
            else if( i<=(int)(exp.length()-3) && (std::isdigit(exp[i+1]) && std::isdigit(exp[i+2])))
                return false;
        }
        else if(exp[i] == '(')
        {
            if(i == (int)(exp.length()-1)) return false;
            else if(exp[i+1] !='(' && !std::isdigit(exp[i+1]) && exp[i+1] != '+' && exp[i+1] !='-')
            {
                return false;
            }
        }
        else if(exp[i] == ')')
        {
            if(i == 0) return false;
            else if(exp[i-1] != ')' && !std::isdigit(exp[i-1]))
            {
                return false;
            }
        }
        else
            return false;
    }



    return true;
}


/******      其他函数     ******/
// 把 string 类转为数字
int Calculator::StringToNum(std::string& s)
{
    return stoi(s);
}

// 将 char 类转为数字
int Calculator::StringToNum(char& s)
{
    return s-'0';
}

// 数字简化：6.0000=>6, 6.50000=>6.5
std::string Calculator::NumSimplify(std::string s)
{
    for(int i = s.length()-1; i>=0; i--)
    {
        if(s[i] == '0' || s[i] == '.')
        {
            auto temp = s[i];
            s.erase(i);

            if(temp == '.')
            {
                return s;
            }
        }
        else
        {
            return s;
        }
    }
    return std::string("");
}

// 判断是否含有未知数，即是否为多项式
bool Calculator::isPoly()
{
    for(int i = 0; i<(int)exp.length(); i++)
    {
        if(exp[i] == 'x') return true;
    }
    return false;
}
