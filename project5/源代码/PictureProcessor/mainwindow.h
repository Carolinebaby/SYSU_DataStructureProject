#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>
#include <QMainWindow>
#include <ppmimage.h>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <changesizewidget.h>
#include <imagecompress.h>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_openFileAction_triggered();
    void on_blackImageAction_triggered();
    void on_ZoomInAction_triggered();
    void on_ZoomOutAction_triggered();
    void on_originalImageAction_triggered();
    void on_ChangeSizeAction_triggered();

    void on_saveFileAction_triggered();

    void on_warmColorAction_triggered();

    void on_coolColorAction_triggered();

    void on_oldColorAction_triggered();

    void on_inverseColorAction_triggered();

    void on_CompressImageAction_triggered();

    void on_DecompressImageAction_triggered();

    void on_clear_Action_triggered();

    void on_ClockwiseRotate_Action_triggered();

    void on_AnticlockwiseRotate_Action_triggered();

    void on_exit_Action_triggered();

private:
    void ShowImage(const QImage& img);
    void ShowImage(PPMimage& img);
    void ChangeImageSize();
    void UpdateStatusBar();
    void JudgetEmpty();
    void ClearStatusBar();

    bool isImageExist;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage image;
    QImage presentImage;
    PPMimage presentPpmImage;
    PPMimage ppmImage;
    ChangeSizeWidget changeSizeWidget;
    QString filePath;
    void closeEvent(QCloseEvent *event) override;

    QLabel* photoInfoLabel;
    QLabel* photoFilePathLabel;
    PPMimage originalImage;
};
#endif // MAINWINDOW_H
