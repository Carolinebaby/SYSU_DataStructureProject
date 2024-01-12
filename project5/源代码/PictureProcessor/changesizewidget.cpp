#include "changesizewidget.h"
#include "ui_changesizewidget.h"

ChangeSizeWidget::ChangeSizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeSizeWidget)
{
    ui->setupUi(this);
    ui->choice2->setChecked(true);
    connect(ui->saveChoiceButton, SIGNAL(clicked()), this, SIGNAL(SaveChoiceButtonClicked()));
}

ChangeSizeWidget::~ChangeSizeWidget()
{
    delete ui;
}

void ChangeSizeWidget::SetChoiceLabel(int w, int h)
{
    ui->choice1->setText(QString::number(w/4)+"×"+QString::number(h/4)+" 像素");
    ui->choice2->setText(QString::number(w/2)+"×"+QString::number(h/2)+" 像素");
    ui->choice3->setText(QString::number(w*2)+"×"+QString::number(h*2)+" 像素");
    ui->choice4->setText(QString::number(w*4)+"×"+QString::number(h*4)+" 像素");
}


void ChangeSizeWidget::on_cancelButton_clicked()
{
    this->close();
}

void ChangeSizeWidget::on_saveChoiceButton_clicked()
{
    this->close();
}

int ChangeSizeWidget::GetChoice()
{
    if(ui->choice1->isChecked())
    {
        return 1;
    }
    else if(ui->choice2->isChecked())
    {
        return 2;
    }
    else if(ui->choice3->isChecked())
    {
        return 3;
    }
    else if(ui->choice4->isChecked())
    {
        return 4;
    }
    return 0;
}
