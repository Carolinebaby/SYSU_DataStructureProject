#include "mainwindow.h"
#include "imagecompress.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), scene(nullptr)
{
    ui->setupUi(this);
    photoInfoLabel = new QLabel(" ");
    photoFilePathLabel = new QLabel(" ");
    ClearStatusBar();

    connect(&changeSizeWidget, &ChangeSizeWidget::SaveChoiceButtonClicked, this, &MainWindow::ChangeImageSize);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(scene) delete scene;
    if(photoInfoLabel) delete photoInfoLabel;
    if(photoFilePathLabel) delete photoFilePathLabel;
}

void MainWindow::on_openFileAction_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, tr("打开图像文件"), "", tr("图像文件 (*.ppm)"));

    if(image.isNull() && filePath.isEmpty())
    {
        isImageExist = false;
        return;
    }
    image.load(filePath);
    if(image.isNull())
    {
        isImageExist = false;
        return;
    }
    isImageExist = true;
    ppmImage = PPMimage(filePath.toStdString());
    ShowImage(image);
    presentImage = image;
    originalImage = ppmImage;
    presentPpmImage = ppmImage;
    UpdateStatusBar();
}

void MainWindow::UpdateStatusBar()
{
    delete photoInfoLabel;
    photoInfoLabel = new QLabel("  "+ QString::number(presentImage.width())+"×"+QString::number(presentImage.height())+" 像素  ", this);
    photoInfoLabel->setMinimumWidth(120);
    ui->statusbar->addWidget(photoInfoLabel);
    delete photoFilePathLabel;
    photoFilePathLabel = new QLabel("  文件路径: " + filePath + "  ");
    photoFilePathLabel->setMinimumWidth(200);
    ui->statusbar->addWidget(photoFilePathLabel);
}

void MainWindow::ClearStatusBar()
{
    delete photoInfoLabel;
    photoInfoLabel = new QLabel("  "+ QString::number(0)+"×"+QString::number(0)+" 像素  ", this);
    photoInfoLabel->setMinimumWidth(120);
    ui->statusbar->addWidget(photoInfoLabel);
    delete photoFilePathLabel;
    photoFilePathLabel = new QLabel("  请导入图片文件  ");
    photoFilePathLabel->setMinimumWidth(200);
    ui->statusbar->addWidget(photoFilePathLabel);
}

void MainWindow::ShowImage(const QImage& img)
{
    if(scene != nullptr)
    {
        delete ui->graphicsView->scene();
    }
    presentImage = img;
    scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(img));
    ui->graphicsView->setScene(scene);
    UpdateStatusBar();
}

void MainWindow::ShowImage(PPMimage& img)
{
    QImage qimg(img.GetWidth(), img.GetHeight(), QImage::Format_RGB32);
    std::vector<Pixel> matrix = img.GetPMatrix();
    int w = img.GetWidth();
    for(int i = 0; i<(int)matrix.size(); i++)
    {
        qimg.setPixelColor(i%w, i/w, qRgb(matrix[i].R, matrix[i].G, matrix[i].B));
    }

    ShowImage(qimg);
}

void MainWindow::on_ZoomInAction_triggered()
{
    if(!isImageExist) return;
    QImage img = image.scaled(presentImage.width()+10, presentImage.height()+10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ShowImage(img);
}

void MainWindow::on_ZoomOutAction_triggered()
{
    if(!isImageExist) return;
    QImage img = image.scaled(std::max(presentImage.width()-10,0), std::max(presentImage.height()-10,0), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ShowImage(img);
}

void MainWindow::on_originalImageAction_triggered()
{
    if(!isImageExist) return;
    ppmImage = originalImage;
    presentPpmImage = originalImage;
    ShowImage(originalImage);
}


void MainWindow::on_ChangeSizeAction_triggered()
{
    if(!isImageExist) return;
    int w = image.width(), h = image.height();
    changeSizeWidget.SetChoiceLabel(w,h);
    changeSizeWidget.show();
}

void MainWindow::ChangeImageSize()
{
    if(!isImageExist) return;
    int choice = changeSizeWidget.GetChoice();
    if(choice == 1)
    {
        presentPpmImage = ppmImage.ChangeSmallSize(4);
        ShowImage(presentPpmImage);
    }
    else if(choice == 2)
    {
        presentPpmImage = ppmImage.ChangeSmallSize(2);
        ShowImage(presentPpmImage);
    }
    else if(choice == 3)
    {
        presentPpmImage = ppmImage.ChangeBigSize(2);
        ShowImage(presentPpmImage);
    }
    else if(choice == 4)
    {
        presentPpmImage = ppmImage.ChangeBigSize(4);
        ShowImage(presentPpmImage);
    }
}

void MainWindow::on_saveFileAction_triggered()
{
    if(!isImageExist) return;
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存图像文件"), "", tr("图像文件 (*.ppm)"));

    if (!filePath.isEmpty())
    {
        filePath = fileName;
        ppmImage = presentPpmImage;
        UpdateStatusBar();
        presentPpmImage.SavePpmImage(filePath.toStdString());
    }
}

void MainWindow::on_blackImageAction_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = ppmImage.BlackImage();
    ShowImage(presentPpmImage);
}

void MainWindow::on_warmColorAction_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = ppmImage.WarmColor();
    ShowImage(presentPpmImage);
}


void MainWindow::on_coolColorAction_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = ppmImage.CoolColor();
    ShowImage(presentPpmImage);
}


void MainWindow::on_oldColorAction_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = ppmImage.OldColor();
    ShowImage(presentPpmImage);
}


void MainWindow::on_inverseColorAction_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = ppmImage.ReverseColor();
    ShowImage(presentPpmImage);
}


void MainWindow::on_CompressImageAction_triggered()
{
    if(!isImageExist)
    {
        QMessageBox::warning(this, "警告", "未导入图片，请导入图片后再进行压缩");
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("保存压缩文件");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(windowIcon());
    msgBox.setText("文件压缩成功，点击OK保存压缩好的文件");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        ImageCompress imgCmpr;
        QString filePath = QFileDialog::getSaveFileName(this, "Save File", QString(), "CompressedImage Files(*.cimg)");
        imgCmpr.SaveCompressedImage(ppmImage, filePath.toStdString());
    }
}


void MainWindow::on_DecompressImageAction_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("导入压缩文件");
    msgBox.setText("请导入压缩的文件");
    msgBox.setWindowIcon(windowIcon());
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        ImageCompress imgCmpr;
        filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "CompressedImage Files(*.cimg)");
        if(!filePath.isEmpty())
        {
            isImageExist = true;
            ppmImage = imgCmpr.DecompressedImage(filePath.toStdString());
            presentPpmImage = ppmImage;
            ShowImage(ppmImage);
            image = presentImage;
        }
    }
}


void MainWindow::on_clear_Action_triggered()
{
    ui->graphicsView->scene()->clear();
    isImageExist = false;
    ClearStatusBar();
}


void MainWindow::on_ClockwiseRotate_Action_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = presentPpmImage.ClockwiseRotate();
    ShowImage(presentPpmImage);
}


void MainWindow::on_AnticlockwiseRotate_Action_triggered()
{
    if(!isImageExist) return;
    presentPpmImage = presentPpmImage.AntiClockwiseRotate();
    ShowImage(presentPpmImage);
}


void MainWindow::on_exit_Action_triggered()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "关闭窗口", "你确定要关闭窗口吗？",
                                                               QMessageBox::Cancel | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}
