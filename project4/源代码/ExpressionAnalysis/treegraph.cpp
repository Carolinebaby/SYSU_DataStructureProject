#include "treegraph.h"
#include "ui_treegraph.h"

TreeGraph::TreeGraph(ExpNode* newRoot,QWidget *parent) :
    QWidget(parent),painter(this),
    ui(new Ui::TreeGraph),buffer(size())
{
    ui->setupUi(this);
    this->resize(800,600);
    buffer = QPixmap(size());
    setMouseTracking(false);
    setAutoFillBackground(false);

    root = newRoot;
    InitGraph();
}


void TreeGraph::InitGraph()
{
    if(!root) return;

    std::queue<ExpNode*> que;
    que.emplace(root);
    d = 0;

    while(!que.empty())
    {
        d++;
        int size = que.size();
        for(int i = 0; i<size; i++)
        {
            ExpNode* node = que.front(); que.pop();
            vec.emplace_back(node);
            if(node->left) que.emplace(node->left);
            if(node->right) que.emplace(node->right);
        }
    }

    w = width()/(d+1);
    h = height()/(d+1);
    r = h/4;

    root->x = width()/2, root->y = height()/(d+1);


    buffer.fill(QColor(214, 230, 242));
    QPainter painter(&buffer);

    int penWidth = 5/(d/3+1)+1;
    painter.setPen(QPen(QColor(118, 159, 205), penWidth));
    painter.setBrush(QColor(247, 251, 252));
    int fontSize = 60/(d/3+1);
    QFont font("Cascadia Code" , fontSize);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);

    que.emplace(root);

    while(!que.empty())
    {
        int sz = que.size();
        for(int i = 0; i<sz ; i++)
        {
            ExpNode* node = que.front(); que.pop();
            vec.emplace_back(node);
            if(node->left)
            {
                node->left->x = node->x - w;
                node->left->y = node->y + h;
                painter.drawLine(node->x, node->y, node->left->x, node->left->y);
                que.emplace(node->left);
            }

            if(node->right)
            {
                node->right->x = node->x + w;
                node->right->y = node->y + h;
                painter.drawLine(node->x, node->y, node->right->x, node->right->y);
                que.emplace(node->right);
            }
        }

        w = w*2/3> r*1.5? w*2/3 : r*1.5;
    }

    for(ExpNode* node: vec)
    {
        painter.drawEllipse(node->x-r, node->y-r, 2*r, 2*r);
        QString text = QString::fromStdString(node->c);
        int textWidth = painter.fontMetrics().maxWidth() * text.length();
        int textHeight = painter.fontMetrics().xHeight();
        int textX = node->x - textWidth / 2;
        int textY = node->y + textHeight / 2;
        painter.drawText(textX, textY, text);
    }

}

void TreeGraph::paintEvent(QPaintEvent* event)
{
    painter.setRenderHint(QPainter::Antialiasing);
    QPainter painter(this);
    painter.drawPixmap(0, 0, buffer);
}

TreeGraph::~TreeGraph()
{
    delete ui;
}



