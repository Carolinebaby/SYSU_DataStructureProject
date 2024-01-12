#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    highlighter = new CppHighlighter(ui->inputtext->document());

    // 设置 Tab 字符的宽度（以像素为单位）
    int tabWidthInPixels = 40;  // 40 像素宽度，你可以根据需要进行调整

    QTextOption textOption = ui->inputtext->document()->defaultTextOption();
    textOption.setTabStopDistance(tabWidthInPixels);
    ui->inputtext->document()->setDefaultTextOption(textOption);

    InitialShow();

    connect(ui->actionopen_txt_file, &QAction::triggered, this, &MainWindow::OpenTxtFile);
    connect(ui->actionopen_cpp_file, &QAction::triggered, this, &MainWindow::OpenCppFile);
    connect(ui->actionsave_txt_file, &QAction::triggered, this, &MainWindow::SaveTxtFile);
    connect(ui->actionsave_cpp_file, &QAction::triggered, this, &MainWindow::SaveCppFile);
    connect(ui->actionsave, &QAction::triggered, this, &MainWindow::ActionSave);
    connect(ui->actionopen, &QAction::triggered, this, &MainWindow::ActionOpen);
    connect(ui->actionsearch, &QAction::triggered, this, &MainWindow::ActionSearch);
    connect(ui->actionabout, &QAction::triggered, this, &MainWindow::ActionAbout);

    connect(ui->inputtext, SIGNAL(textChanged()), this, SLOT(ShowTextLength()));
    connect(ui->inputtext, SIGNAL(textChanged()), this, SLOT(ShowLineCount()));
    connect(ui->inputtext, SIGNAL(textChanged()), this, SLOT(ShowLetterCount()));
    connect(ui->inputtext, SIGNAL(textChanged()), this, SLOT(ShowPunctuationCount()));

    connect(&fd, &FindDialog::findButtonClicked, this, &MainWindow::FindWordInText);
    connect(&fd, &FindDialog::findDialogClosed, this, &MainWindow::WindowReset);

    connect(ui->actionThe_kth_words, &QAction::triggered, this, &MainWindow::AnalyseKthWords);
    connect(ui->action_The_frequency_of_the_reserved_words, &QAction::triggered, this, &MainWindow::AnalyseReservedWords);
    connect(ui->actiondetailed_analysis, &QAction::triggered, this, &MainWindow::AnalyseCppStatistics);

    connect(&txtWidget, SIGNAL(InputChanged()), this, SLOT(AnalyseKthWords()));

    connect(ui->inputtext, &QPlainTextEdit::redoAvailable, ui->actionredo, &QAction::setEnabled);
    connect(ui->actionredo, &QAction::triggered, ui->inputtext, &QPlainTextEdit::redo);
    connect(ui->inputtext, &QPlainTextEdit::undoAvailable, ui->actionundo, &QAction::setEnabled);
    connect(ui->actionundo, &QAction::triggered, ui->inputtext, &QPlainTextEdit::undo);
    connect(ui->actionclose, &QAction::triggered, this, &MainWindow::close);
    connect(ui->inputtext, &QPlainTextEdit::copyAvailable, ui->actioncopy, &QAction::setEnabled);
    connect(ui->actioncopy, &QAction::triggered, ui->inputtext, &QPlainTextEdit::copy);
    connect(ui->actioncut, &QAction::triggered, ui->inputtext, &QPlainTextEdit::cut);
    connect(ui->actionpaste, &QAction::triggered, ui->inputtext, &QPlainTextEdit::paste);
}

/******      分析 txt文本文件的 前 k 个 出现频率最大的单词以及单词出现的频率和行数      ******/
void MainWindow::AnalyseKthWords()
{
    if(isCppFile)
    {
        QMessageBox::warning(this, "Warning", "Please conduct a CPP file analysis!");
        return;
    }

    txtWidget.show();
    int k = txtWidget.GetKInput();

    std::string present;
    if(k<=0)
    {
        present = std::string("Please enter an unsigned positive integer");
        txtWidget.SetResult(present);
        return;
    }
    std::string text = ui->inputtext->toPlainText().toStdString();

    TxtAnalysis ta(text);
    std::vector<Word> words = ta.GetKthWords(k);
    present = VecWordToStr(words);

    txtWidget.SetResult(present);
    txtWidget.SetWords(words);
}

/******      分析 cpp文件的 保留字出现频率以及出现的行数      ******/
void MainWindow::AnalyseReservedWords()
{
    if(!isCppFile)
    {
        QMessageBox::warning(this, "Warning", "Please conduct a txt file analysis!");
        return;
    }
    cppWidget.show();
    std::string text = ui->inputtext->toPlainText().toStdString();
    CppAnalysis ca(text);
    std::vector<Word> words = ca.GetWords();
    std::string present = VecWordToStr(words);
    cppWidget.SetTitle("Analyze the frequency of reserved words");
    cppWidget.SetWords(words);
    cppWidget.SetResult(present);
}

/******      分析 cpp文件的 函数、注释、头文件、空白行的个数     ******/
void MainWindow::AnalyseCppStatistics()
{
    if(!isCppFile)
    {
        QMessageBox::warning(this, "Warning", "Please conduct a txt file analysis!");
        return;
    }
    cppWidget.show();
    std::string text = ui->inputtext->toPlainText().toStdString();
    CppAnalysis ca(text);
    std::string present;
    present = "number of functions = " + std::to_string(ca.GetFunctionCount()) +
              "\nnumber of commets = " + std::to_string(ca.GetCommentCount()) +
              "\nnumber of blacklines = " + std::to_string(ca.GetBlankLineCount()) +
              "\nnumber of headerfiles = " + std::to_string(ca.GetHeaderFileCount()) + "\n";
    cppWidget.SetTitle("Cpp File Analysis");
    cppWidget.SetResult(present);
    std::vector<Word> statics{{"functions",ca.GetFunctionCount()},
                              {"commets",ca.GetCommentCount()},
                              {"blacklines",ca.GetBlankLineCount()},
                              {"headerfiles",ca.GetHeaderFileCount()}};
    cppWidget.SetWords(statics);
}


/******      ctrl+F 或 点击放大镜的那个按钮，进行单词匹配搜索     ******/
void MainWindow::ActionSearch()
{
    fd.show();
    QRect screenGeometry = this->geometry();
    fd.setGeometry(screenGeometry.x() + screenGeometry.width()-fd.width(), screenGeometry.y(), fd.width(), fd.height());
    fd.SetFocus();
}


/******      将查找到的单词标红     ******/
void MainWindow::FindWordInText()
{
    QString qword = fd.GetInput();
    Word word(qword);
    QString text = ui->inputtext->toPlainText().toLower();

    std::vector<int> pos = word.FindAllPos(text);

    WindowReset();

    // 设置匹配字符背景颜色为黄色
    QTextCursor cursor(ui->inputtext->document());
    QTextCharFormat format;
    format.setBackground(QColor(255, 255, 0));

    for (int position: pos)
    {
        cursor.setPosition(position);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, word.qword.length());
        cursor.setCharFormat(format);
    }
}



/******      初始化状态栏的显示      ******/
void MainWindow::InitialShow()
{
    textlengthlabel = new QLabel("  Text length: 0");
    textlengthlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(textlengthlabel);

    linecountlabel = new QLabel(" Line count: 0");
    linecountlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(linecountlabel);

    lettercountlabel = new QLabel(" Letter count: 0");
    lettercountlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(lettercountlabel);

    punctuationcountlabel = new QLabel(" Punctuation count: 0");
    punctuationcountlabel->setMinimumWidth(150);
    ui->statusbar->addWidget(punctuationcountlabel);
}

/******      实时显示文本长度      ******/
void MainWindow::ShowTextLength()
{
    QString text = ui->inputtext->toPlainText();
    int length = text.length();
    delete textlengthlabel;
    textlengthlabel = new QLabel(" Text length: " + QString::number(length), this);
    textlengthlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(textlengthlabel);
}

/******      实时显示行数      ******/
void MainWindow::ShowLineCount()
{
    QTextDocument doc;
    doc.setPlainText(ui->inputtext->toPlainText());
    int linecount = doc.blockCount();
    delete linecountlabel;
    linecountlabel = new QLabel("  Line count: " + QString::number(linecount), this);
    linecountlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(linecountlabel);
}

/******      实时显示字数      ******/
void MainWindow::ShowLetterCount()
{
    QString text = ui->inputtext->toPlainText();
    int count = 0;
    for (QChar ch : text) {
        if (!ch.isSpace()) {
            count++;
        }
    }
    delete lettercountlabel;
    lettercountlabel = new QLabel(" Letter count: " + QString::number(count), this);
    lettercountlabel->setMinimumWidth(120);
    ui->statusbar->addWidget(lettercountlabel);
}

/******      实时显示标点数量      ******/
void MainWindow::ShowPunctuationCount()
{
    QString text = ui->inputtext->toPlainText();
    int punctuationCount = 0;

    for (int i = 0; i < text.length(); i++) {
        QChar character = text.at(i);
        if (character.isPunct()) {
            punctuationCount++;
        }
    }

    delete punctuationcountlabel;
    punctuationcountlabel = new QLabel(" Punctuation count: " + QString::number(punctuationCount), this);
    punctuationcountlabel->setMinimumWidth(150);
    ui->statusbar->addWidget(punctuationcountlabel);
}
/******      打开文件      ******/
void MainWindow::ActionOpen()
{
    OpenFile(0);
}
void MainWindow::OpenTxtFile()
{
    OpenFile(1);
}

void MainWindow::OpenCppFile()
{
    OpenFile(2);
}

//打开文件的主函数
void MainWindow::OpenFile(int choice)
{

    if(choice == 0)
        filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "C++ Files or Txt Files (*.cpp;*.txt)");
    else if(choice == 1)
    {
        isCppFile = false;
        filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "Text Files (*.txt)");
    }
    else if(choice == 2)
    {
        isCppFile = true;
        filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "CPP Files (*.cpp)");
    }

    if (!filePath.isEmpty())
    {
        ui->inputtext->clear();
        isCreate = false;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            ui->inputtext->setPlainText(in.readAll());
            file.close();
            if(choice == 0)
            {
                QFileInfo fileInfo(filePath);
                if (fileInfo.suffix() == "cpp")
                {
                    isCppFile = true;
                }
                else
                {
                    isCppFile = false;
                }
            }
        }
    }

    if(isCppFile)
    {
        if(highlighter == nullptr) highlighter = new CppHighlighter(ui->inputtext->document());
    }
    else
    {
        if(highlighter != nullptr)
        {
            delete highlighter;
            highlighter = nullptr;
        }
    }

    if(filePath.isEmpty())
    {
        isCreate = true;
        isCppFile = false;
    }
}



/******      保存文件      ******/
void MainWindow::ActionSave()
{
    SaveFile(0);
}
void MainWindow::SaveTxtFile()
{
    if(!isCppFile)
        SaveFile(1);
    else
    {
        QMessageBox::warning(this, "Warning", "This is a cpp file, not a txt file! Please click the \'save cpp file\' button!");
    }
}

void MainWindow::SaveCppFile()
{
    if(isCppFile)
        SaveFile(2);
    else
    {
        QMessageBox::warning(this, "Warning", "This is a txt file, not a cpp file! Please click the \'save txt file\' button!");
    }
}

// 保存文件-主函数
void MainWindow::SaveFile(int choice)
{
    if(!isCreate)
    {
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << ui->inputtext->toPlainText();
                file.close();
            }
        }
    }
    else
    {
        // 获取用户指定的文件名和路径
        QString filePath;
        if (choice == 0) {
            filePath = QFileDialog::getSaveFileName(this, "Save File", QString(), "Text Files(*.txt);;C++ Files(*.cpp)");
        } else if (choice == 1) {
            filePath = QFileDialog::getSaveFileName(this, "Save File", QString(), "Text Files (*.txt)");
        } else if (choice == 2) {
            filePath = QFileDialog::getSaveFileName(this, "Save File", QString(), "CPP Files (*.cpp)");
        }

        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << ui->inputtext->toPlainText();
                file.close();
            }

            // 打开已创建的文件并读取其内容
            QFile readFile(filePath);
            if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&readFile);
                QString fileContents = in.readAll();
                // 在这里可以使用fileContents来处理文件内容
                readFile.close();
            }

            QFileInfo fileInfo(filePath);
            if (fileInfo.suffix() == "cpp") {
                isCppFile = true;
            } else {
                isCppFile = false;
            }
        }
        isCreate = false;
        if(isCppFile)
        {
            if(highlighter == nullptr) highlighter = new CppHighlighter(ui->inputtext->document());
        }
        else
        {
            if(highlighter != nullptr)
            {
                delete highlighter;
                highlighter = nullptr;
            }
        }

        if(filePath.isEmpty())
        {
            isCreate = true;
            isCppFile = false;
        }
    }
}

/******      将wordvector 转为输出字符串     ******/
std::string MainWindow::VecWordToStr(std::vector<Word> words)
{
    std::string res;
    for(int i = 0; i<(int)words.size(); i++)
    {
        res+=std::to_string(i+1);
        res+="th: ";
        res+=words[i].word;
        res+=": freqency is ";
        res += std::to_string(words[i].freq);
        res+= ", appear lines: ";
        for(auto j: words[i].lines)
        {
            res+=std::to_string(j);
            res+=" ";
        }
        res += '\n';
    }
    return res;
}

/******      界面刷新      ******/
void MainWindow::WindowReset()
{
    QString text = ui->inputtext->toPlainText();
    ui->inputtext->clear();
    ui->inputtext->setPlainText(text);
}

void MainWindow::ActionAbout()
{
    QMessageBox::information(this,"About", "This is a simple text editor and analyzer made by Wu Yingfei.");
}

/******      析构函数     ******/
MainWindow::~MainWindow()
{
    delete ui;
    delete highlighter;
}
