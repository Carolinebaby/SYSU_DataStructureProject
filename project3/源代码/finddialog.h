#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    QString GetInput();
    void SetFocus();

signals:
    void findButtonClicked(); // 自定义信号
    void findDialogClosed(); // 窗口关闭


private:
    bool eventFilter(QObject* obj, QEvent* event);

    Ui::FindDialog *ui;

};

#endif // FINDDIALOG_H
