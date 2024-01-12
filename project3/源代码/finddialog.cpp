#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    //接按钮的点击信号与自定义信号
    connect(ui->findButton, SIGNAL(clicked()), this, SIGNAL(findButtonClicked()));
    // 窗口关闭
    connect(this, &QDialog::finished, this, &FindDialog::findDialogClosed);
}

QString FindDialog::GetInput()
{
    return ui->inputtext->text();
}

void FindDialog::SetFocus()
{
    ui->inputtext->setFocus();
}

bool FindDialog::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->inputtext)		//可替换
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);

            if(k->key() == Qt::Key_Return)
            {
                emit findButtonClicked();		//替换为需要响应的函数事件，以这里的按钮为例
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

FindDialog::~FindDialog()
{
    delete ui;
}
