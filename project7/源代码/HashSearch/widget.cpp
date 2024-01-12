#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->modTypeHash_RadioButton->setChecked(true);
    ui->randomProbing_RadioButton->setChecked(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_hashTableOutput_clicked()
{
    if(ui->dataInput->toPlainText().isEmpty())
    {
        ui->output->setPlainText("输入无效");
        return;
    }

    int mapMode = 1, collisionTackleMode = 1;

    if(ui->midSquare_RadioButton->isChecked())
    {
        mapMode = 2;
    }

    if(ui->linearProbing_RadioButton->isChecked())
    {
        collisionTackleMode = 2;
    }

    if(ui->quadraticProbing_RadioButton->isChecked())
    {
        collisionTackleMode = 3;
    }

    Hash hash(ui->dataInput->toPlainText().toStdString(), mapMode, collisionTackleMode, ui->dataInput->toPlainText().count('\n')+1);
    ui->output->setPlainText(QString::fromStdString(hash.MapMessage()));
    ui->searchLenOutput->setText(QString::number(hash.averageSearchLen()));
}


void Widget::on_SearchIdx_clicked()
{
    if(ui->dataInput->toPlainText().isEmpty() || ui->searchNameInput->text().isEmpty())
    {
        ui->output->setPlainText("输入无效");
        return;
    }

    int mapMode = 1, collisionTackleMode = 1;

    if(ui->midSquare_RadioButton->isChecked())
    {
        mapMode = 2;
    }

    if(ui->linearProbing_RadioButton->isChecked())
    {
        collisionTackleMode = 2;
    }

    if(ui->quadraticProbing_RadioButton->isChecked())
    {
        collisionTackleMode = 3;
    }

    Hash hash(ui->dataInput->toPlainText().toStdString(), mapMode, collisionTackleMode, ui->dataInput->toPlainText().count('\n')+1);

    std::pair<int, int> p = hash.GetMessage(ui->searchNameInput->text().toStdString(), mapMode, collisionTackleMode);
    if(p.first == -1)
        ui->output->setPlainText("此姓名未在表中，请重新输入姓名");
    else
        ui->output->setPlainText(QString("搜索得到的处于哈希映射表的下标为：") + QString::number(p.first) + "，搜索长度为：" + QString::number(p.second));
    ui->searchLenOutput->setText(QString::number(hash.averageSearchLen()));
}

