#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <expression.h>
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
    void GetInfix();
    void GetExprValue();
    void MergeConst();
    void CompoundExpr();
    void ShowTreeGraph();
    void CloseClicked();
    void ClearClicked();

private:
    std::string SimplifyNumString(std::string s);
    bool isInputValid(std::string s);
    bool isNum(std::string s);

    Ui::Widget *ui;
    TreeGraph* treeGraph;
};
#endif // WIDGET_H
