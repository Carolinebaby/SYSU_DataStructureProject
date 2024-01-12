#include "imagecompress.h"

ImageCompress::ImageCompress(): treeRoot(nullptr){}

// 保存压缩后的图像文件
void ImageCompress::SaveCompressedImage(PPMimage& pimg, std::string filename)
{
    Init();
    CalcWeight(pimg.GetPMatrix());
    BuildTree();
    CodeTreeNode(treeRoot);
    SaveInFile(pimg, filename);
}
PPMimage ImageCompress::DecompressedImage(std::string cmpimgfile)// 解压 压缩文件
{
    Init();
    return GetDecodeImage(cmpimgfile);
}

ImageCompress::~ImageCompress()
{
    ClearTree(treeRoot);
}

// 初始化参数
void ImageCompress::Init()
{
    ClearTree(treeRoot);
    freqMap.clear();
    huffmanCodeMap.clear();
}

// 清空树
void ImageCompress::ClearTree(TreeNode*& root)
{
    if(root == nullptr) return;
    if(!root->left && !root->right)
    {
        delete root;
        root = nullptr;
    }
    else
    {
        ClearTree(root->left);
        ClearTree(root->right);
        delete root;
        root = nullptr;
    }
}

/******      编码时使用的函数      ******/

// 计算每个结点的权值（频数）， 构建键值对容器
void ImageCompress::CalcWeight(std::vector<Pixel>& pmatrix)
{
    for(Pixel& p: pmatrix)
    {
        int value = p.R*1000000+p.G*1000+p.B;
        if(freqMap.count(value))
        {
            freqMap[value]++;
        }
        else
        {
            freqMap[value] = 1;
        }
    }
}

//构建哈夫曼编码树
void ImageCompress::BuildTree()
{
    std::priority_queue<TreeNode*, std::vector<TreeNode*>, nodePtrCmp> nodeQueue;
    for(auto it = freqMap.begin(); it!=freqMap.end(); it++)
    {
        TreeNode* node = new TreeNode(it->first, it->second);
        nodeQueue.emplace(node);
    }

    while(nodeQueue.size() > 1)
    {
        TreeNode* right = nodeQueue.top(); nodeQueue.pop();
        TreeNode* left = nodeQueue.top(); nodeQueue.pop();
        TreeNode* parent = new TreeNode(0,right->weight+left->weight,left, right);
        nodeQueue.emplace(parent);
    }
    treeRoot = nodeQueue.top();
}

// 遍历树，对叶子结点进行编码
void ImageCompress::CodeTreeNode(TreeNode* root)
{
    if(!root) return;
    if(!root->left && !root->right)
    {
        huffmanCodeMap.emplace(root->value, root->code);
        return;
    }
    if(root->left)
    {
        root->left->code = root->code+'0';
        CodeTreeNode(root->left);
    }
    if(root->right)
    {
        root->right->code = root->code+'1';
        CodeTreeNode(root->right);
    }
}

// 把压缩文件的二进制内容写入文件
void ImageCompress::SaveInFile(PPMimage& pimg, std::string filename)
{
    std::fstream file;
    file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    // 写入图像基本信息，便于恢复图像
    file << pimg.GetType() << '\n' << pimg.GetWidth() << ' ' << pimg.GetHeight() << '\n' << pimg.GetMaxVal() << '\n' << huffmanCodeMap.size() << '\n';

    // 把哈夫曼编码表写入文件，用来重构图像的哈夫曼树
    for(auto it = huffmanCodeMap.begin(); it!=huffmanCodeMap.end();it++)
    {
        file << it->first << ' ';
        file << it->second.length() << ' ';
        WirteBinaryString(it->second, file);
        file << ' ';
    }

    // 构建 二进制文件内容
    std::string encodeText;
    for(Pixel&p: pimg.GetPMatrix())
    {
        int RGB = p.R*1000000+p.G*1000+p.B;
        std::string code = huffmanCodeMap[RGB];
        encodeText+=code;
    }

    // 写入 根据哈夫曼编码得到新的图像的二进制信息
    file << encodeText.length() << '\n';
    WirteBinaryString(encodeText, file);
    file.close();
}

// 把表示string类文本表示的二进制内容写入文件中
void ImageCompress::WirteBinaryString(std::string& encodeText, std::fstream& out)
{
    int Tbits = 0;
    char buf = 0;
    for (char c : encodeText)
    {
        if (Tbits == 8)
        {
            out.put(buf);
            out.flush();
            buf = Tbits = 0;
        }
        unsigned char mask = 1;
        if (c == '1')
        {
            mask = mask << (7 - Tbits);
            buf = buf | mask;
        }
        if (c == '0')
        {
            mask = mask << (7 - Tbits);
            mask = ~mask;
            buf = buf & mask;
        }
        Tbits++;
    }

    if (Tbits)
    {
        out.put(buf);
        out.flush();
    }
}



/************            解码时使用的函数            ************/
// 解码的总函数，返回图像的 PPMimage 格式
PPMimage ImageCompress::GetDecodeImage(std::string filename)
{
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    std::string type;
    int width, height, max_val, mapsize;  // 读入基本信息
    file >> type >> width >> height >> max_val >> mapsize;

    for(int i = 0; i<mapsize; i++)
    {
        int RGB, codelen;
        file >> RGB >> codelen;

        file.get(); // 读取 空格，避免空格读入code
        std::string code = DecodeBinaryFile(codelen, file);

        huffmanCodeMap.emplace(RGB, code.substr(0,codelen));
    }

    int codeTextLen;
    file >> codeTextLen;

    std::string line;
    std::getline(file, line);//读入换行，避免换行符读入 code
    std::string encodeText = DecodeBinaryFile(codeTextLen, file);

    // 重构哈夫曼编码树
    RebuildTree();
    // 获取图像的 像素点矩阵
    std::vector<Pixel> pmatrix;
    ConstructMatrix(pmatrix, encodeText);

    file.close();

    return PPMimage(type, width, height, max_val, pmatrix);
}

// 根据二进制编码构建 像素点矩阵
void ImageCompress::ConstructMatrix(std::vector<Pixel>& pmatrix, std::string& codeText)
{
    int pos = 0;
    TreeNode* node = treeRoot;
    while(pos < (int)codeText.length())
    {
        if(codeText[pos] == '0')
        {
            node = node->left;
        }
        else if(codeText[pos] == '1')
        {
            node = node->right;
        }

        if(!node->right && !node->left)
        {
            int RGB = node->value;
            int R = RGB/1000000, G = (RGB-R*1000000)/1000, B = RGB-R*1000000-G*1000;
            if(RGB) pmatrix.emplace_back(R, G, B);
            node = treeRoot;
        }

        pos++;
    }
}

// 根据哈夫曼编码表，重构哈夫曼树
void ImageCompress::RebuildTree()
{
    treeRoot = new TreeNode;
    for(auto it = huffmanCodeMap.begin(); it != huffmanCodeMap.end(); it++)
    {
        ConstructNode(it->first, 0, it->second, treeRoot);
    }
}

// 构建结点(RebuildTree函数要用)
void ImageCompress::ConstructNode(const int& value, int pos, std::string& code, TreeNode*& root)
{
    if(pos == (int)code.length())
    {
        root->value = value;
        root->code = code;
        return;
    }

    if(code[pos] == '0')
    {
        if(!root->left) root->left = new TreeNode;
        ConstructNode(value, pos+1, code, root->left);
    }
    else
    {
        if(!root->right) root->right = new TreeNode;
        ConstructNode(value, pos+1, code, root->right);
    }
}

// 读出二进制内容
std::string ImageCompress::DecodeBinaryFile(int len, std::fstream& file)
{
    std::string code;
    char buf = 0;
    int nbits = 8;
    int t = len;
    while(t>0)
    {
        if(nbits == 8)
        {
            buf = file.get();
            nbits = 0;
        }

        unsigned char mask = 1;
        mask = mask << (7-nbits);
        mask = mask & buf;
        nbits++;
        if(mask == 0)
        {
            code += '0';
        }
        else
        {
            code += '1';
        }
        t--;
    }
    return code;
}

