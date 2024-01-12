#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<graph.h>
#include <treegraph.h>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_dfsSequence_clicked();

    void on_bfsSequence_clicked();

    void on_dfsTree_clicked();

    void on_bfsTree_clicked();

    void InitGraph();

private:
    Graph graph;
    Ui::Widget *ui;
    TreeGraph* treeGraph;
    std::string lastgraphmessage;
};
#endif // WIDGET_H
