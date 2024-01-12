#ifndef TREEGRAPH_H
#define TREEGRAPH_H

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <queue>
#include <expnode.h>

namespace Ui {
class TreeGraph;
}

class TreeGraph : public QWidget
{
    Q_OBJECT

public:
    explicit TreeGraph(ExpNode* newRoot, QWidget *parent = nullptr);
    ~TreeGraph();
    void paintEvent(QPaintEvent* event) override;

private:
    void InitGraph();

    QPainter painter;
    Ui::TreeGraph *ui;
    ExpNode* root;
    int d, w, h, r;
    std::vector<ExpNode*> vec;
    QPixmap buffer;
};

#endif // TREEGRAPH_H
