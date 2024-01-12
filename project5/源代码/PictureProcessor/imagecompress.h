#ifndef IMAGECOMPRESS_H
#define IMAGECOMPRESS_H
#include<ppmimage.h>
#include <unordered_map>
#include <queue>
#include <bitset>

class ImageCompress
{
public:
    ImageCompress();
    void SaveCompressedImage(PPMimage& pimg, std::string filename);  // 保存压缩后的图像文件
    PPMimage DecompressedImage(std::string cmpimgfile);              // 解压 压缩文件
    ~ImageCompress();

private:
    // 结点结构
    struct TreeNode{
        int value;
        long long weight;
        TreeNode* left, *right;
        std::string code;
        TreeNode(int value = 0, long long weight = 0, TreeNode*left = nullptr, TreeNode* right = nullptr):value(value), weight(weight), left(left), right(right){}
    };

    // 初始化参数
    void Init();

    // 编码时使用的函数
    void CalcWeight(std::vector<Pixel>& pmatrix);                               // 计算每个结点的权值（频数）， 构建键值对容器
    void BuildTree();                                                           //构建哈夫曼编码树
    void CodeTreeNode(TreeNode* root);                                          // 遍历树，对叶子结点进行编码
    void SaveInFile(PPMimage& pimg, std::string filename);                      // 把压缩文件的二进制内容写入文件
    void WirteBinaryString(std::string& encodeText, std::fstream& out);         // 把表示string类文本表示的二进制内容写入文件中

    // 解码时使用的函数
    PPMimage GetDecodeImage(std::string filename);                              // 解码的总函数，返回图像的 PPMimage 格式
    void ConstructMatrix(std::vector<Pixel>& pmatrix, std::string& codeText);   // 根据二进制编码构建 像素点矩阵
    void RebuildTree();                                                         // 根据哈夫曼编码表，重构哈夫曼树
    void ConstructNode(const int& value, int pos, std::string& code, TreeNode*& root);  // 构建结点(RebuildTree函数要用)
    std::string DecodeBinaryFile(int len, std::fstream& file);                  // 读出二进制内容

    // 清空树
    void ClearTree(TreeNode*& root);



    struct nodePtrCmp{
        bool operator()(TreeNode* a, TreeNode* b)
        {
            return (a->weight)>=(b->weight);
        }
    };
    TreeNode* treeRoot;
    std::unordered_map<int, int> freqMap;
    std::unordered_map<int, std::string> huffmanCodeMap;
};

#endif // IMAGECOMPRESS_H
