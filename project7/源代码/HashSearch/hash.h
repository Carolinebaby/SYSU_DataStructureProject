#ifndef HASH_H
#define HASH_H

#include<vector>
#include<string>
#include<sstream>
#include<cmath>
#define HTSIZE 60

class Hash
{
public:
    Hash();
    Hash(std::string message, int mapMode, int collisionTackleMode, int datasize);
    // 将信息插入哈希表
    void Insert(std::string text, int mapMode, int collisionTackleMode);
    int midSquare(int num);     // 平方取中法 下标计算

    std::string MapMessage();   // 获取哈希查找表信息
    double averageSearchLen();  // 平均查找长度
    // 查找单个信息
    std::pair<int, int> GetMessage(std::string name, int mapMode, int collisionTackleMode);

private:
    std::vector<std::string> hashTable;
    std::vector<int> searchLen;
    int sum_sl;
    int sz;
};

#endif // HASH_H
