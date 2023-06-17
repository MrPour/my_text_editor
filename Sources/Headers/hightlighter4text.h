//
// Created by mrpour on 2023/6/17.
//

#ifndef MYQTDEMO_HIGHTLIGHTER4TEXT_H
#define MYQTDEMO_HIGHTLIGHTER4TEXT_H

#include <QObject>
#include <QSyntaxHighlighter>
//qt6移除了QRegExp
#include <QRegularExpression>
class Highlighter4Text : public QSyntaxHighlighter
{
public:
    //定义构造函数，赋予默认值
    explicit Highlighter4Text(QTextDocument *parent= nullptr,QString fontFamily="Consolas",int fontsize=14);
    void setFont(QFont font);

protected:
    //实现父类的纯虚方法
    void highlightBlock(const QString &text) override;

private:
    QString mFontFamily;
    int mFontSize;
    //规则内部类
    struct HighlightRule{
        //正则决定处理哪些string
        QRegularExpression pattern;
        //format则决定如何处理
        QTextCharFormat format;
    };
    //规则集合
    QVector<HighlightRule> highlightRules;
    //创建普通文本高亮规则
    void addNormalTextFormat();
    //数组高亮规则
    void addNumberFormat();
    //字符串高亮规则
    void addStringFormat();
    //普通注释高亮规则
    void addCommentFormat();
    //多行注释高亮规则
    void addMultiLineCommentFormat(const QString &text);
    //关键词高亮规则
    void addKeywordsFormat();
    //类名高亮规则
    void addClassNameFormat();
    //函数名高规则
    void addFunctionFormat();
};

#endif //MYQTDEMO_HIGHTLIGHTER4TEXT_H
