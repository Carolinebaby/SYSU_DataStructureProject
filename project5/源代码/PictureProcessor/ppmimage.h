#ifndef PPMIMAGE_H
#define PPMIMAGE_H
#include<string>
#include<vector>
#include<algorithm>
#include <fstream>

struct Pixel{
    int R, G, B;
    Pixel(int R = 0, int G = 0, int B = 0): R(R), G(G), B(B){}
    void SetRGB(int r = 0, int g = 0, int b = 0)
    {
        R = r, G = g, B = b;
    }
};

class PPMimage
{
public:
    // 构造函数
    PPMimage();
    PPMimage(const PPMimage& pimg);
    PPMimage(std::string filePath);
    PPMimage(std::string type, int width, int height, int max_val, std::vector<Pixel>& matrix);
    // 重载运算符
    void operator=(const PPMimage& pimg);

    // 保存图片
    void SavePpmImage(std::string filePath);

    // 获取图像信息的接口
    std::string GetType();
    int GetMaxVal();
    int GetWidth();
    int GetHeight();
    std::vector<Pixel>& GetPMatrix();

    // 滤镜
    PPMimage BlackImage();
    PPMimage WarmColor();
    PPMimage CoolColor();
    PPMimage OldColor();
    PPMimage ReverseColor();
    // 改变图片大小
    PPMimage ChangeBigSize(int coef);
    PPMimage ChangeSmallSize(int coef);
    // 图像旋转
    PPMimage ClockwiseRotate();
    PPMimage AntiClockwiseRotate();

private:
    // 插值运算
    void CalcItem(int l, int r, int b, int t, int coef, std::vector<Pixel>& matrix);

    std::string type;
    int width, height, max_val;
    std::vector<Pixel> pmatrix;
};

#endif // PPMIMAGE_H
