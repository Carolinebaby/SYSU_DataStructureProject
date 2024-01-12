#include "expnode.h"

ExpNode::ExpNode():left(nullptr), right(nullptr){}

ExpNode::ExpNode(std::string c):c(c), left(nullptr), right(nullptr){}

bool ExpNode::isLetter()
{
    if(c[0] >= 'a' && c[0] <= 'z') return true;
    return false;
}
bool ExpNode::isNum()
{
    int dotcnt = 0;
    int i = 0;
    if(c[0] == '-' && c.length() > 1) i++;
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
bool ExpNode::isOperator()
{
    if(c.length() > 1) return false;
    if(c[0] == '+' || c[0] == '-' || c[0] == '*' || c[0] == '/' || c[0] == '^') return true;
    return false;
}

bool ExpNode::isTriFunc()
{
    if(c.length()<6) return false;
    std::string str = c.substr(0, 3);
    if((str == "sin" || str == "cos") && c[3] == '(' && c[c.length()-1] == ')')
    {
        return true;
    }
    return false;
}
