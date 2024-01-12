#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    treeGraph = nullptr;
}



Widget::~Widget()
{
    delete ui;
}


void Widget::on_dfsSequence_clicked()
{
    InitGraph();
    bool valid = graph.DFSSequence(ui->startInput->text().toStdString());
    if(!valid)
    {
        ui->output->setPlainText("Invalid input, please enter again.");
    }
    else
    {
        std::string out = graph.GetDFSSequence();
        ui->output->setPlainText(QString::fromStdString(out));
    }
}


void Widget::on_bfsSequence_clicked()
{
    InitGraph();
    bool valid = graph.BFSSequence(ui->startInput->text().toStdString());
    if(!valid)
    {
        ui->output->setPlainText("Invalid input, please enter again.");
    }
    else
    {
        std::string out = graph.GetBFSSequence();
        ui->output->setPlainText(QString::fromStdString(out));
    }
}


void Widget::on_dfsTree_clicked()
{
    InitGraph();
    bool valid = graph.BuildDFSTree(ui->startInput->text().toStdString());
    if(!valid)
    {
        QMessageBox::warning(this, "Warning", "invalid input");
        return;
    }

    if(treeGraph) delete treeGraph;
    treeGraph = new TreeGraph(graph.GetDFSRoot());
    treeGraph->show();
}


void Widget::on_bfsTree_clicked()
{
    InitGraph();
    bool valid = graph.BuildBFSTree(ui->startInput->text().toStdString());

    if(!valid)
    {
        QMessageBox::warning(this, "Warning", "invalid input");
        return;
    }

    if(treeGraph) delete treeGraph;
    treeGraph = new TreeGraph(graph.GetBFSRoot());
    treeGraph->show();
}


void Widget::InitGraph()
{
    std::string graphmessage = ui->graphInput->toPlainText().toStdString();
    if(graphmessage != lastgraphmessage)
    {
        graph.InitGraph(graphmessage);
        lastgraphmessage = graphmessage;
    }
}
