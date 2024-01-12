#include "hash.h"

Hash::Hash()
{

}


// 哈希表初始化
Hash::Hash(std::string message, int mapMode, int collisionTackleMode, int datasize)
{
    sz = datasize;
    hashTable.resize(HTSIZE);
    searchLen.resize(HTSIZE);
    hashTable.assign(HTSIZE,"");
    searchLen.assign(HTSIZE, 0);
    sum_sl = 0;

    std::istringstream iss(message);
    std::string name;
    while(std::getline(iss, name))
    {
        Insert(name, mapMode, collisionTackleMode);
    }

}

// 插入哈希表
void Hash::Insert(std::string text, int mapMode, int collisionTackleMode)
{
    int idx;
    int num = 0;
    for(char& c: text) num += (int)c;
    idx = num % HTSIZE;     // 除留余数法

    if(mapMode == 2)        // 平方取中法
    {
        idx = (midSquare(num))% HTSIZE;
    }

    int sl = 1;
    while(hashTable[idx].length() != 0)
    {
        if(collisionTackleMode == 1)        // 伪随机探测再散列法
        {
            idx = ((idx * 1103515245%600 + 12345) & 0xFFFFFFFF) % HTSIZE;
        }
        else if(collisionTackleMode == 2)   // 线性探测再散列法
        {
            idx = (idx+1) % HTSIZE;
        }
        else if(collisionTackleMode == 3)   // 平方探测再散列法
        {
            idx = ((int)(idx+pow(-1, sl)*pow(sl/2, 2))) % HTSIZE;
        }
        sl++;
    }

    hashTable[idx] = text;
    searchLen[idx] = sl;
    sum_sl+=sl;
}

// 平方取中法
int Hash::midSquare(int num)
{
    long long squared = static_cast<long long>(num) * static_cast<long long>(num);
    std::string squaredStr = std::to_string(squared);

    int midIndex = (squaredStr.length() / 2) - 2;
    std::string hashStr = squaredStr.substr(midIndex, 3);

    return std::stoi(hashStr);
}

// 输出哈希查找表信息
std::string Hash::MapMessage()
{
    std::string message;

    message += std::string("idx") + '\t' + "name" + "\t\t" + "searchLen" + "\n";

    for(int i = 0; i<HTSIZE; i++)
    {
        message += std::to_string(i);
        message += '\t';
        message += (hashTable[i].length())? (hashTable[i].length() <= 10 ? hashTable[i] + '\t' :  hashTable[i] ): "NULL\t";
        message += '\t';
        message += std::to_string(searchLen[i]);
        message += '\n';
    }

    return message;
}

// 输出平均查找长度
double Hash::averageSearchLen()
{
    return (double)sum_sl/sz;
}

// 获取 索要查找姓名的信息
std::pair<int, int> Hash::GetMessage(std::string text, int mapMode, int collisionTackleMode)
{
    int idx;
    int num = 0;
    for(char& c: text) num += (int)c;


    idx = num % HTSIZE;     // 除留余数法

    if(mapMode == 2)        // 平方取中法
    {
        idx = (midSquare(num))% HTSIZE;
    }

    int temp = 1;
    while(hashTable[idx].length() && hashTable[idx] != text)
    {
        if(collisionTackleMode == 1)
        {
            idx = ((idx * 1103515245%600 + 12345) & 0xFFFFFFFF) % HTSIZE;
        }
        else if(collisionTackleMode == 2)
        {
            idx = (idx+1) % HTSIZE;
        }
        else if(collisionTackleMode == 3)
        {
            idx = (int)(idx+pow(-1, temp)*pow(temp/2, 2)) % HTSIZE;
            temp++;
        }
    }
    if(hashTable[idx].length())
        return std::make_pair(idx, searchLen[idx]);
    return std::make_pair(-1,0);
}
