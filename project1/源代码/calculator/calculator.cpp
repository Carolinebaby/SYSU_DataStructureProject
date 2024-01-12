#include "calculator.h"

Calculator::Calculator(std::string& s): fxpolynomial(s)
{
    SimplifyPolynomial();
}


/***********************      与外界接口函数      ***********************/
// 返回化简后的多项式
std::string Calculator::GetSimplifiedFx()
{
    std::pair<std::vector<int>, std::map<int, double>> p = std::make_pair(fxexponent, fxexpcoef);
    return FormatToString(p);
}

//返回多项式导函数
std::string Calculator::GetDF()
{
    std::pair<std::vector<int>, std::map<int, double>> p = GetDerivative();
    return FormatToString(p);
}

//返回给定x下的多项式的值
std::string Calculator::GetFxValue(double x)
{
    std::string result = std::to_string(CalValue(x));
    return NumSimplify(result);
}

// 返回多项式积分函数
std::string Calculator::GetIntFx()
{
    std::pair<std::vector<int>, std::map<int, double>> p = GetIntegral();
    return FormatToString(p,1);
}


/***********************      功能 1 化简多项式      ***********************/

//提取两个多项式以及之间的符号，调用相应加减乘化简
void Calculator::SimplifyPolynomial()
{
    std::string p1, p2;  // 提取出来的两个多项式
    int flag = 0;  // 符号

    std::stack<int> stk;
    if(fxpolynomial[0] == '(')
    {
        int pos = 0;
        stk.push(0);
        for(int i = 1; i<(int)(fxpolynomial.length()); i++)
        {
            if(fxpolynomial[i] == '(')
            {
                stk.push(i);
            }
            else if(fxpolynomial[i] == ')')
            {
                stk.pop();
                if(stk.empty()) // 大括号结束的位置
                {
                    pos = i;
                    break;
                }
            }
        }

        p1 = fxpolynomial.substr(1, pos-1);
        if(pos == (int)(fxpolynomial.length()-1)) // 后面没有多项式
        {
            p2 = std::string("0");
        }
        else
        {
            if(fxpolynomial[pos+1] == '*')
                flag = 1;
            else if(fxpolynomial[pos+1] == '+')
                flag = 2;
            else if(fxpolynomial[pos+1] == '-')
                flag = 3;

            if(fxpolynomial[pos+2] == '(')
            {
                p2 = fxpolynomial.substr(pos+3, fxpolynomial.length()-pos-4);
            }
            else
            {
                p2 = fxpolynomial.substr(pos+2, fxpolynomial.length()-pos-2);
            }
        }

    }
    else  // 第一个多项式没有大括号
    {
        int pos = -1;
        for(int i = 1; i<(int)(fxpolynomial.length()); i++)
        {
            if(fxpolynomial[i] == '(' && fxpolynomial[i-1] != '^')  // 超大括号
            {
                pos = i;
                break;
            }
        }

        if(pos == -1) // 只有一个多项式
        {
            p1 = fxpolynomial;
            p2 = std::string("0");
        }
        else
        {
            if(fxpolynomial[pos-1] == '*')
                flag = 1;
            else if(fxpolynomial[pos-1] == '+')
                flag = 2;
            else if(fxpolynomial[pos-1] == '-')
                flag = 3;

            p1 = fxpolynomial.substr(0, pos-1);
            p2 = fxpolynomial.substr(pos+1,fxpolynomial.length()-pos-2);
        }
    }

    std::pair<std::vector<int>, std::map<int, double>> p;
    if(p2 == "0") p = PolyFormat(p1);
    if(flag == 1)
    {
        p = PolyProduct(p1,p2);
    }
    else if(flag == 2)
    {
        p = PolyAdd(p1,p2);
    }
    else if(flag == 3)
    {
        p = PolySubstract(p1,p2);
    }
    fxexponent = p.first;
    fxexpcoef = p.second;
}

 // 获得单项的指数-系数
std::pair<int, double> Calculator::GetExpCoef(std::string monomial, int sign)
{
    double coef = 0;
    int exp = 0;
    int j = 0;
    if(monomial[0] == 'x')
    {
        coef = 1;
        j = 1;
        if(j == (int)(monomial.length()))
        {
            exp = 1;
        }
        else if(monomial[j] == '^')
        {
            j--;
            if(monomial[j+2] == '(')
            {
                std::string expstr = monomial.substr(j+3,monomial.length()-j-3);
                exp = StringToNum(expstr);
            }
            else if(monomial[j+2] >='0' && monomial[j+2] <= '0'+9)
            {
                std::string expstr = monomial.substr(j+2, 1);
                exp = StringToNum(expstr);
            }
        }
    }
    else if(monomial[0]>='0' && monomial[0] <='0'+9)
    {
        while((monomial[j]>='0' && monomial[j] <='0'+9) || monomial[j] == '.') j++;
        coef = std::stod(monomial.substr(0, j));
        if(j == (int)(monomial.length()))
        {
            exp = 0;
        }
        else if(monomial[j] == '*' && monomial[j+1] == 'x')
        {
            j++;
            if(j == (int)(monomial.length()-1))
            {
                exp = 1;
            }
            else if(monomial[j+1] == '^')
            {
                if(monomial[j+2] == '(')
                {
                    std::string expstr = monomial.substr(j+3,monomial.length()-j-3);
                    exp = StringToNum(expstr);
                }
                else if(monomial[j+2] >='0' && monomial[j+2] <= '0'+9)
                {
                    std::string expstr = monomial.substr(j+2, 1);
                    exp = StringToNum(expstr);
                }
            }
        }

    }
    coef*=sign;
    return std::make_pair(exp, coef);
}

// 将多项式 格式化 成 map(指数-系数) 和 vector（指数）
std::pair<std::vector<int>, std::map<int, double>> Calculator::PolyFormat(std::string& polynomial)
{
    std::vector<int> exponent;  // 存储 指数
    std::map<int, double> expcoef;  // 存储 指数-系数
    int sign = 1;  // 当前项的符号

    int i = 0; // 遍历的位置
    if(polynomial[0] == '+') // 特殊情况，首位有符号
    {
        i = 1;
    }
    else if(polynomial[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    // 遍历
    int start = i;  // 当前项 开始的位置
    for(; i<(int)(polynomial.length()); i++)
    {
        if(polynomial[i] == '+' || polynomial[i] == '-' || i == (int)(polynomial.length() - 1))
        { 
            if(i>=2 && polynomial[i-1] == '(' && polynomial[i-2] == '^') continue;
            // 判定 -、+ 号是否是 指数括号里面的，如果不是，则继续下面的操作
            int end = (i == (int)(polynomial.length() - 1)) ? i: i-1; // 单项的结尾位置
            std::pair<int, double> p = GetExpCoef(polynomial.substr(start,end+1-start), sign);
            if(expcoef.count(p.first))
            {
                expcoef[p.first] += p.second;
            }
            else
            {
                expcoef[p.first] = p.second;
                exponent.emplace_back(p.first);
            }
			// 为下一个 单项的符号数 赋值
            if(polynomial[i] == '+')
            {
                sign = 1;
            }
            else if(polynomial[i] == '-')
            {
                sign = -1;
            }
            start = end+2;
        }
    }

    std::sort(exponent.begin(), exponent.end(), std::greater<int>());

    return std::make_pair(exponent, expcoef);
}

 // 多项式相乘
std::pair<std::vector<int>, std::map<int, double>> Calculator::PolyProduct(std::string& polynomial1, std::string& polynomial2)
{
    std::pair<std::vector<int>, std::map<int, double>> p1 = PolyFormat(polynomial1);
    std::pair<std::vector<int>, std::map<int, double>> p2 = PolyFormat(polynomial2);

    std::vector<int> exponent;  // 存储 指数
    std::map<int, double> expcoef;  // 存储 指数-系数

    for(auto exp1: p1.first)
    {
        for(auto exp2: p2.first)
        {
            int exp = exp1+exp2;
            double coef = p1.second[exp1] * p2.second[exp2];
            if(expcoef.count(exp))
            {
                expcoef[exp] += coef;
            }
            else
            {
                expcoef[exp] = coef;
                exponent.emplace_back(exp);
            }
        }
    }

    std::sort(exponent.begin(), exponent.end(), std::greater<int>());

    return std::make_pair(exponent, expcoef);
}

 // 多项式相加
std::pair<std::vector<int>, std::map<int, double>> Calculator::PolyAdd(std::string& polynomial1, std::string& polynomial2)
{
    std::pair<std::vector<int>, std::map<int, double>> p1 = PolyFormat(polynomial1);
    std::pair<std::vector<int>, std::map<int, double>> p2 = PolyFormat(polynomial2);

    std::vector<int> exponent;  // 存储 指数
    std::map<int, double> expcoef;  // 存储 指数-系数

    for(auto exp1: p1.first)  // 遍历 第一个多项式的指数
    {
        for(auto exp2: p2.first)   // 遍历 第二个多项式的指数
        {
            if(exp1 == exp2)  // 存在相等的指数，进行系数合并
            {
                int exp = exp1;
                double coef = p1.second[exp1] + p2.second[exp2];  // 系数相加
                if(expcoef.count(exp))  // 原先存在
                {
                    expcoef[exp] += coef;  // 和原先的系数进行合并
                }
                else  //原先不存在，直接存入
                {
                    expcoef[exp] = coef;
                    exponent.emplace_back(exp);
                }
            }
        }
    }

    // 获取 在另一个多项式中不存在相同指数的 指数对应的系数，直接存入 expcoef 和 exponent 中
    for(auto exp: p1.first)
    {
        if(!expcoef.count(exp))
        {
            expcoef[exp] = p1.second[exp];
            exponent.emplace_back(exp);
        }
    }
    for(auto exp: p2.first)
    {
        if(!expcoef.count(exp))
        {
            expcoef[exp] = p2.second[exp];
            exponent.emplace_back(exp);
        }
    }

    // 从大到小对指数进行排序
    std::sort(exponent.begin(), exponent.end(), std::greater<int>());

    return std::make_pair(exponent, expcoef);
}

 //多项式相减
std::pair<std::vector<int>, std::map<int, double>> Calculator::PolySubstract(std::string& polynomial1, std::string& polynomial2)
{
    std::pair<std::vector<int>, std::map<int, double>> p1 = PolyFormat(polynomial1);
    std::pair<std::vector<int>, std::map<int, double>> p2 = PolyFormat(polynomial2);

    std::vector<int> exponent;  // 存储 指数
    std::map<int, double> expcoef;  // 存储 指数-系数

    for(auto exp1: p1.first)
    {
        for(auto exp2: p2.first)
        {
            if(exp1 == exp2)
            {
                int exp = exp1;
                double coef = p1.second[exp1] - p2.second[exp2];
                if(expcoef.count(exp))
                {
                    expcoef[exp] += coef;
                }
                else
                {
                    expcoef[exp] = coef;
                    exponent.emplace_back(exp);
                }
            }
        }
    }

    for(auto exp: p1.first)
    {
        if(!expcoef.count(exp))
        {
            expcoef[exp] = p1.second[exp];
            exponent.emplace_back(exp);
        }
    }
    for(auto exp: p2.first)
    {
        if(!expcoef.count(exp))
        {
            expcoef[exp] = -1*p2.second[exp];
            exponent.emplace_back(exp);
        }
    }



    std::sort(exponent.begin(), exponent.end(), std::greater<int>());

    return std::make_pair(exponent, expcoef);
}


/***********************      功能 2 求导函数     ***********************/
std::pair<std::vector<int>, std::map<int, double>> Calculator::GetDerivative() // 获取导函数的标准格式
{
    std::vector<int> exponent; // 导数的指数容器
    std::map<int, double> expcoef;  // 导数的 指数-系数 键值对容器


    for(int i = 0; i<(int)(fxexponent.size()); i++)
    {
        int oldexp = fxexponent[i];  // 原来的指数
        double oldcoef = fxexpcoef[oldexp];  //原来的参数
        if(oldexp == 0 || oldcoef == 0)  // 如何系数为0 或 指数为0
            continue;
		// 根据多项式求导的基本定理，对
        int exp = oldexp -1; // 导数的指数 = 原来的指数-1
        double coef = oldcoef*oldexp;  // 导数的系数 = 原来的指数*原来的系数
        // 新的指数-系数 键值对 存入导数的 指数-系数 键值对容器
        // 因为原来的指数是唯一的，所以求导后的指数也是唯一的，可以直接存入导数的 指数-系数 键值对容器，不需要进行重复判断
        expcoef[exp] = coef;  
        exponent.emplace_back(exp);
    }

    return std::make_pair(exponent, expcoef);
}


/***********************      功能 3 求给定 x 的值      ***********************/
double Calculator::CalValue(double x)  // 根据给定的 x 值计算 f(x)的数值大小
{
    double result = 0;

    for(int i = 0; i < (int)(fxexponent.size()); i++)
    {
        result += fxexpcoef[fxexponent[i]] * pow(x, fxexponent[i]);
    }
    return result;
}

/***********************      功能 4 求积分函数      ***********************/
std::pair<std::vector<int>, std::map<int, double>> Calculator::GetIntegral() // 获取积分函数的标准格式，其中 exp = 0代表为 lnx
{
    std::vector<int> exponent; // 积分的指数容器
    std::map<int, double> expcoef;  // 积分的 指数-系数 键值对容器


    for(int i = 0; i<(int)(fxexponent.size()); i++)
    {
        int oldexp = fxexponent[i];  // 原来的指数
        double oldcoef = fxexpcoef[oldexp];  //原来的参数
        if(oldcoef == 0)  // 如果系数为0
            continue;
        // 根据多项式求积分的基本定理
        int exp = oldexp + 1;
        double coef = 0;
        if(exp == 0)
        {
            coef = oldcoef;
        }
        else
        {
            coef = oldcoef/exp;
        }
        // 新的指数-系数 键值对 存入导数的 指数-系数 键值对容器
        // 因为原来的指数是唯一的，所以求导后的指数也是唯一的，可以直接存入积分的 指数-系数 键值对容器，不需要进行重复判断
        expcoef[exp] = coef;
        exponent.emplace_back(exp);
    }

    return std::make_pair(exponent, expcoef);
}



/***********************      其他函数      ***********************/

 // 把 string 类转为数字
int Calculator::StringToNum(std::string& s)
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
    return sign * std::stoi(s.substr(start, s.length()-start));
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

// 将 标准格式转换为字符串
std::string Calculator::FormatToString(std::pair<std::vector<int>, std::map<int, double>>& p, int isIntegral)
{
    std::string result;  // 保存输出的多项式字符串
    bool isBegin = true; // 判断是否为字符串的开端
    for(int i = 0; i<(int)(p.first.size()); i++)
    {
        int exp = p.first[i];
        double coef = p.second[exp];

        if(coef == 0) continue;

        if(exp == 0)
        {
            if(!isIntegral)  // 不是积分函数，正常输出处理
            {
                if(i == 0 || coef < 0)
                {
                    result += NumSimplify(std::to_string(coef));
                }
                else
                {
                    result += "+";
                    result += NumSimplify(std::to_string(coef));
                }
            }
            else  // 如果是积分函数，要对 exp == 0 的情况作特殊输出处理：lnx
            {
                if(coef == 1)
                {
                    if(isBegin) result += "lnx";
                    else result += "+lnx";
                }
                else if(coef == -1)
                {
                    result += "-lnx";
                }
                else if(isBegin || coef < 0)
                {
                    result += NumSimplify(std::to_string(coef));
                    result += "*lnx";
                }
                else
                {
                    result += "+";
                    result += NumSimplify(std::to_string(coef));
                    result += "*lnx";
                }
            }
            isBegin = false;
            continue;
        }

        if(coef == -1)
        {
            result += "-x";
        }
        else if(coef == 1)
        {
            if(!isBegin) result += "+";
            result += "x";
        }
        else if(coef >0 && !isBegin)
        {
            result += "+";
            result += NumSimplify(std::to_string(coef));
            result += "*x";
        }
        else
        {
            result += NumSimplify(std::to_string(coef));
            result += "*x";
        }

        if(exp != 1)
        {
            if(exp < 0 || exp >9)
            {
                result += "^(";
                result += std::to_string(exp);
                result += ")";
            }
            else
            {
                result += "^";
                result += std::to_string(exp);
            }
        }
        isBegin = false;
    }

    if(result.empty())  // 没有字符输出，说明多项式为 0
    {
        result = "0";
    }

    if(result[0] == '+')  // 字符串开端的 + 可以省略
    {
        result = result.substr(1,result.length()-1);
    }


    if(isIntegral) result += "+C"; // 如果是积分函数，在末尾要加上常量 C

    return result;
}
