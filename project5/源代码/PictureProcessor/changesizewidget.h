#ifndef CHANGESIZEWIDGET_H
#define CHANGESIZEWIDGET_H

#include <QWidget>

namespace Ui {
class ChangeSizeWidget;
}

class ChangeSizeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeSizeWidget(QWidget *parent = nullptr);
    ~ChangeSizeWidget();

    void SetChoiceLabel(int w, int h);
    int GetChoice();
signals:
    void SaveChoiceButtonClicked(); // 自定义信号

private slots:
    void on_cancelButton_clicked();

    void on_saveChoiceButton_clicked();

private:
    Ui::ChangeSizeWidget *ui;
};

#endif // CHANGESIZEWIDGET_H
