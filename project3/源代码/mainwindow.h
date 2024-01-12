#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <finddialog.h>
#include <word.h>
#include <cpphighlighter.h>
#include <cppanalysis.h>
#include <cppanalysiswidget.h>
#include <txtanalysis.h>
#include <txtanalysiswidget.h>

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
    void OpenTxtFile();
    void OpenCppFile();
    void SaveTxtFile();
    void SaveCppFile();
    void ActionOpen();
    void ActionSave();
    void ActionSearch();
    void ActionAbout();
    void FindWordInText();
    void InitialShow();
    void ShowTextLength();
    void ShowLineCount();
    void ShowLetterCount();
    void ShowPunctuationCount();

    void AnalyseKthWords();
    void AnalyseReservedWords();
    void AnalyseCppStatistics();
    void WindowReset();


private:
    void OpenFile(int choice);
    void SaveFile(int choice);
    std::string VecWordToStr(std::vector<Word> words);


    bool isCreate = true;
    QString filePath;
    bool isCppFile = false;
    QLabel* textlengthlabel;
    QLabel* linecountlabel;
    QLabel* lettercountlabel;
    QLabel* punctuationcountlabel;
    QFont textfont;
    Ui::MainWindow *ui;
    FindDialog fd;
    CppHighlighter* highlighter;
    CppAnalysisWidget cppWidget;
    TxtAnalysisWidget txtWidget;
};
#endif // MAINWINDOW_H
