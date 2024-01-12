#include "ppmimage.h"

PPMimage::PPMimage(): type(""), width(0), height(0), max_val(255)
{

}
PPMimage::PPMimage(const PPMimage& pimg): type(pimg.type), width(pimg.width), height(pimg.height), max_val(pimg.max_val)
{
    for(Pixel p: pimg.pmatrix)
    {
        pmatrix.emplace_back(p);
    }
}
PPMimage::PPMimage(std::string type, int width, int height, int max_val, std::vector<Pixel>& matrix):type(type),
    width(width), height(height), max_val(max_val), pmatrix(matrix)
{

}
void PPMimage::operator=(const PPMimage& pimg)
{
    type = pimg.type;
    width = pimg.width;
    height = pimg.height;
    max_val = pimg.max_val;
    pmatrix.clear();
    for(Pixel p: pimg.pmatrix)
    {
        pmatrix.emplace_back(p);
    }
}
PPMimage::PPMimage(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    file >> type >> width >> height >> max_val;
    int r, g, b;
    for(int i = 0; i<width*height; i++)
    {
        if(type == "P2")
        {
            file >> r;
            g = b = r;
        }
        else if(type == "P3")
        {
            file >> r >> g >> b;
        }
        pmatrix.emplace_back(r, g, b);
    }

    file.close();
}

int PPMimage::GetWidth()
{
    return width;
}
int PPMimage::GetHeight()
{
    return height;
}
std::vector<Pixel>& PPMimage::GetPMatrix()
{
    return pmatrix;
}

std::string PPMimage::GetType()
{
    return type;
}
int PPMimage::GetMaxVal()
{
    return max_val;
}

void PPMimage::SavePpmImage(std::string filePath)
{
    std::ofstream file;
    file.open(filePath, std::ios::out | std::ios::trunc);
    file << type << "\n" << width << " " << height << "\n" << max_val << "\n";
    for(Pixel p: pmatrix)
    {
        if(type == "P3")
            file << p.R << " " << p.G << " " << p.B << " ";
        else if(type == "P2")
            file << p.R << " ";
    }
    file.close();
}

PPMimage PPMimage::ChangeSmallSize(int coef)
{
    int w = width/coef, h = height/coef;
    std::vector<Pixel> matrix(w*h);
    for(int i = 0; i<height/coef; i++)
    {
        for(int j = 0; j<width/coef; j++)
        {
            Pixel P = pmatrix[(i*coef+1)*width+j*coef+1];
            matrix[i*w+j].SetRGB(P.R,P.G,P.B);
        }
    }
    return PPMimage(type, w, h, max_val, matrix);
}

PPMimage PPMimage::ChangeBigSize(int coef)
{
    int w = width*coef, h = height*coef;
    std::vector<Pixel> matrix(w*h);
    for(int i = 0; i<height; i++)
    {
        for(int j = 0; j <width; j++)
        {
            CalcItem(j,j+1, i, i+1, coef, matrix);
        }
    }

    return PPMimage(type, w, h, max_val, matrix);
}

void PPMimage::CalcItem(int l, int r, int b, int t, int coef, std::vector<Pixel>& matrix)
{
    int w = width*coef, h = height*coef;
    int divisor = coef*coef, pl = l*coef, pr = r*coef, pt = t*coef, pb = b*coef;
    Pixel P1,P2,P3,P4;
    if(pr == w && pt == h)
    {
        P4 = P3 = P2 = P1 = pmatrix[l+b*width];
    }
    else if(pr == w)
    {
        P2 = P1 = pmatrix[l+b*width], P4 = P3 = pmatrix[l+t*width];
    }
    else if(pt == h)
    {
        P3 = P1 = pmatrix[l+b*width], P4 = P2 = pmatrix[r+b*width];
    }
    else
    {
        P1 = pmatrix[l+b*width], P2 = pmatrix[r+b*width], P3 = pmatrix[l+t*width], P4 = pmatrix[r+t*width];
    }

    for(int i = pl; i<=pr; i++)
    {
        if(i >= w) continue;
        for(int j = pb; j<=pt; j++)
        {
            if(j >= h) continue;
            if((i == pl || i == pr) && (j == pb && j == pt))
            {
                Pixel P = pmatrix[i/coef+j/coef*width];
                matrix[i+j*w].SetRGB(P.R, P.G, P.B);
                continue;
            }
            int c1 = (pr-i)*(pt-j), c2 = (i-pl)*(pt-j), c3 = (pr-i)*(j-pb), c4 = (i-pl)*(j-pb);
            matrix[i+j*w].R =  ((double)P1.R/divisor)*c1 + ((double)P2.R/divisor)*c2 + ((double)P3.R/divisor)*c3 + ((double)P4.R/divisor)*c4;
            matrix[i+j*w].G =  ((double)P1.G/divisor)*c1 + ((double)P2.G/divisor)*c2 + ((double)P3.G/divisor)*c3 + ((double)P4.G/divisor)*c4;
            matrix[i+j*w].B =  ((double)P1.B/divisor)*c1 + ((double)P2.B/divisor)*c2 + ((double)P3.B/divisor)*c3 + ((double)P4.B/divisor)*c4;
        }
    }
}

PPMimage PPMimage::BlackImage()
{
    PPMimage res(*this);
    for(Pixel& p: res.pmatrix)
    {
        int grey = (p.R + p.G + p.B)/3;
        p.SetRGB(grey, grey, grey);
    }
    return res;
}
PPMimage PPMimage::WarmColor()
{
    PPMimage res(*this);
    for(Pixel& p: res.pmatrix)
    {
        p.SetRGB(std::min(p.R+30, 255), p.G, std::min(p.B+30, 255));
    }
    return res;
}
PPMimage PPMimage::CoolColor()
{
    PPMimage res(*this);
    for(Pixel& p: res.pmatrix)
    {
        p.SetRGB(p.R, std::min(p.G+30, 255), p.B);
    }
    return res;
}
PPMimage PPMimage::OldColor()
{
    PPMimage res(*this);
    for(Pixel& p: res.pmatrix)
    {
        int R = std::min((int)(0.7*p.R+0.2*p.G+0.1*p.B), 255);
        int G = std::min((int)(0.1*p.R+0.7*p.G+0.2*p.B), 255);
        int B = std::min((int)(0.2*p.R+0.1*p.G+0.7*p.B), 255);
        p.SetRGB((R),(G),(B));
    }
    return res;
}
PPMimage PPMimage::ReverseColor()
{
    PPMimage res(*this);
    for(Pixel& p: res.pmatrix)
    {
        p.SetRGB(255-p.R, 255-p.G, 255-p.B);
    }
    return res;
}

PPMimage PPMimage::ClockwiseRotate()
{
    std::vector<Pixel> matrix(width*height);
    int newwid = height, newheight = width;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int new_j = i;
            int new_i = height-j-1;
            matrix[new_i+new_j*newwid] = pmatrix[i+j*width];
        }
    }
    return PPMimage(type,newwid, newheight, max_val, matrix);
}

PPMimage PPMimage::AntiClockwiseRotate()
{
    std::vector<Pixel> matrix(width*height);
    int newwid = height, newheight = width;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int new_j = width-i-1;
            int new_i = j;
            matrix[new_i+new_j*newwid] = pmatrix[i+j*width];
        }
    }
    return PPMimage(type,newwid, newheight, max_val, matrix);
}
