#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H
#include <QString>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextDocument>

class CppHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CppHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QTextCharFormat keywordFormat; // 关键词
    QTextCharFormat classFormat; // 类名
    QTextCharFormat singleLineCommentFormat; // 单行注释
    QTextCharFormat multiLineCommentFormat; // 多行注释
    QTextCharFormat quotationFormat; // 头文件引用
    QTextCharFormat functionFormat; // 函数
};

#endif // CPPHIGHLIGHTER_H
