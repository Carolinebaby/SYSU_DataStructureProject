#ifndef TREEGRAPH_H
#define TREEGRAPH_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <baseClass.h>
#include <QMessageBox>
namespace Ui {
class TreeGraph;
}

class TreeGraph : public QWidget
{
    Q_OBJECT

public:
    explicit TreeGraph(Node* newroot, QWidget *parent = nullptr);
    ~TreeGraph();
    void paintEvent(QPaintEvent* event) override;

private:
    void InitGraph();

    Ui::TreeGraph *ui;
    QPainter painter;
    QPixmap buffer;
    std::vector<Node*> vec;
    Node* root;
    int d, w, h, r;
};

#endif // TREEGRAPH_H
