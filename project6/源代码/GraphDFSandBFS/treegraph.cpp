#include "treegraph.h"
#include "ui_treegraph.h"

TreeGraph::TreeGraph(Node* newroot, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeGraph)
{
    ui->setupUi(this);
    this->resize(1000,800);
    buffer = QPixmap(size());
    setMouseTracking(false);
    setAutoFillBackground(false);

    root = newroot;
    InitGraph();
}

void TreeGraph::InitGraph()
{
    if(!root) return;

    std::queue<Node*> que;
    que.emplace(root);
    d = 0;
    int maxwid = 0;
    std::vector<int> wids;
    while(!que.empty())
    {
        int sz = que.size();
        d++;
        maxwid = fmax(maxwid, sz);
        wids.emplace_back(maxwid);
        for(int i = 0; i<sz; i++)
        {
            Node* root = que.front();
            que.pop();
            root->childnum = 0;
            Node* child = root->firstchild;
            while(child)
            {
                root->childnum++;
                que.emplace(child);
                child = child->nextsibling;
            }
        }
    }

    w = width()/(maxwid+1);
    h = height()/(d+1);
    r = h/6;

    root->x = width()/2, root->y = height()/(d+1);
    buffer.fill(QColor("#D8D9DA"));
    QPainter painter(&buffer);

    int penWidth = 5/(d/3+1)+1;
    painter.setPen(QPen(QColor(0, 0, 0), penWidth));
    painter.setBrush(QColor(247, 251, 252));
    int fontSize = 20/(d/6+1);
    QFont font("Cascadia Code" , fontSize);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);

    que.emplace(root);

    while(!que.empty())
    {
        int sz = que.size();
        Node* lastsibling = nullptr;
        for(int i = 0; i<sz; i++)
        {
            Node* node = que.front(); que.pop();
            vec.emplace_back(node);
            int childnum = node->childnum;
            Node* child = node->firstchild;

            if(childnum == 1)
            {
                child->x = node->x;
                if(lastsibling && lastsibling->childnum>=2)
                {
                    child->x = fmax(lastsibling->firstchild->x + (lastsibling->childnum-0.5)*2*w/(lastsibling->childnum-1), node->x);
                }
                else if(lastsibling && lastsibling->childnum == 1)
                {
                    child->x = fmax(lastsibling->firstchild->x + w/2, node->x);
                }

                if(child->x>width()-w/2) child->x = width()-w/3+r;
                if(child->x<=2*r) child->x = w/3+r;
                child->y = node->y+h;
                painter.drawLine(node->x, node->y, child->x, child->y);
                que.emplace(child);
            }
            else if(childnum > 1)
            {
                int offset = 2*w/(childnum-1);
                int childx;

                int childy = node->y+h;

                if(lastsibling && lastsibling->childnum == 1)
                {
                    childx = fmax(lastsibling->firstchild->x + w/2, node->x+w-offset*(childnum-1));
                    if(i<sz-1)
                    {
                        w = w*3/4;
                        offset = 2*w/(childnum-1);
                    }
                }
                else if(lastsibling && lastsibling->childnum>=2)
                {
                    childx = fmax(lastsibling->firstchild->x + 2*w/(lastsibling->childnum-1)*(lastsibling->childnum-0.5),node->x+w-offset*(childnum-1));
                    if(i<sz-1)
                    {
                        w = w*3/4;
                        offset = 2*w/(childnum-1);
                    }
                }
                else if((i<sz-1 && que.front()->childnum) || (node->nextsibling && node->nextsibling->childnum))
                {
                    w = w*3/4;
                    offset = 2*w/(childnum-1);
                    childx = node->x + w - offset*(childnum-0.6);
                    if(i<sz-1)
                    {
                        w = w*6/7;
                        offset = 2*w/(childnum-1);
                    }
                }
                else
                {
                    childx = node->x + w - offset*(childnum-1);
                }

                while(child)
                {
                    if(childx > width() - w/2) childx = 1000- (w/3+r);
                    if(childx <= 2*r) childx = w/3+r;
                    child->x = childx;
                    child->y = childy;
                    childx += offset;
                    painter.drawLine(node->x, node->y, child->x, child->y);
                    que.emplace(child);
                    child = child->nextsibling;
                }

            }
            lastsibling = node;
        }

        for(Node* node: vec)
        {
            painter.drawEllipse(node->x-r, node->y-r, 2*r, 2*r);
            QString text = QString::fromStdString(node->data);
            int textWidth = painter.fontMetrics().maxWidth() * text.length();
            int textHeight = painter.fontMetrics().xHeight();
            int textY;
            if(node->firstchild == nullptr)
                textY =node->y+ r+textHeight*3/2;
            else
                textY = node->y + textHeight / 2;

            int textX = node->x - textWidth / 2;

            painter.drawText(textX, textY, text);
        }

        /*int sz = que.size();
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

        w = w*2/3 > r*1.5? w*2/3 : r*1.5;*/
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
