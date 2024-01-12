#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QWidget>
#include <calculator.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:  // 槽口
    void SubmitClicked();

    void ClearClicked();
    void BackClicked();

    void ZeroClicked();
    void OneClicked();
    void TwoClicked();
    void ThreeClicked();
    void FourClicked();
    void FiveClicked();
    void SixClicked();
    void SevenClicked();
    void EightClicked();
    void NineClicked();
    void XButtonClicked();

    void AddClicked();
    void SubtractClicked();
    void PowerClicked();
    void ProductClicked();
    void DivisionClicked();
    void ParenthesisClicked();

    void CloseClicked();
    void IntroductionClicked();

    bool eventFilter(QObject *target, QEvent *event);// 使得 提交按钮响应 输入框的Enter键入

    void NightClicked();  // 黑暗模式

private:
    bool isInteger(std::string&);


private:
    Ui::Widget *ui;
    bool isNightModel = true;


};
#endif // WIDGET_H
