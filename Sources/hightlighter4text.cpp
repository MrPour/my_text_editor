//
// Created by mrpour on 2023/6/17.
//

#include "hightlighter4text.h"
#include <QTextStream>
#include <QFile>

//定义构造函数
Highlighter4Text::Highlighter4Text(QTextDocument *parent,QString fontFamily,int fontsize): QSyntaxHighlighter(parent)
{
    mFontFamily = fontFamily;
    mFontSize = fontsize;

    //初始化规则vector
    //初始化普通文本高亮规则
    addNormalTextFormat();
    //初始化数组高亮规则
    addNumberFormat();
    //初始化字符串高亮规则
    addStringFormat();
    //初始化普通注释高亮规则
    addCommentFormat();

    //关键词高亮规则
    addKeywordsFormat();
    //类名高亮规则
    addClassNameFormat();
    //函数名高规则
    addFunctionFormat();
}

void Highlighter4Text::setFont(QFont font)
{
    mFontFamily = font.family();
    mFontSize = font.pointSize();
}

void Highlighter4Text::highlightBlock(const QString &text) {
    QRegularExpression reg;
    //使用c++的范围for循环
    for(HighlightRule &rule:highlightRules)
    {
        auto pExpression = rule.pattern;
        auto resultMatch = pExpression.match(text);
        //如果匹配上了,一次性把所有的全部修改
        for (int i = 0; i <= resultMatch.lastCapturedIndex(); ++i) {
            int index = resultMatch.capturedStart(i);
            int length = resultMatch.capturedLength(i);
            setFormat(index,length,rule.format);
        }
    }
    //最后处理多行文本注释
    addMultiLineCommentFormat(text);
}

void Highlighter4Text::addNormalTextFormat() {
    HighlightRule rule;
    rule.pattern =  QRegularExpression("[a-z0-9A-Z]+");
    QTextCharFormat normalFormat;
    normalFormat.setFont(QFont(mFontFamily, mFontSize));
    normalFormat.setForeground(QColor(0, 0, 0));
    rule.format = normalFormat;
    highlightRules.append(rule);
}

void Highlighter4Text::addNumberFormat()
{
    HighlightRule rule;
    rule.pattern = QRegularExpression("\\b\\d+|\\d+\\.\\d+\\b");
    QTextCharFormat numberFormat;
    numberFormat.setFont(QFont(mFontFamily,mFontSize));
    numberFormat.setForeground(QColor(250,80,50));
    rule.format = numberFormat;
    highlightRules.append(rule);
}

void Highlighter4Text::addStringFormat()
{
    HighlightRule rule;
    QTextCharFormat stringFormat;
    stringFormat.setFont(QFont(mFontFamily,mFontSize));
    stringFormat.setForeground(QColor(0,200,180));
    rule.format = stringFormat;
    //string有3种不同的格式，所以需要存入3种不同的规则
    //匹配单引号
    rule.pattern = QRegularExpression("'[^']*'");
    highlightRules.append(rule);
    //匹配双引号
    rule.pattern = QRegularExpression("\"[^\"]*\"");
    highlightRules.append(rule);
    //匹配``
    rule.pattern = QRegularExpression("`[^`]*`");
    highlightRules.append(rule);
}

void Highlighter4Text::addCommentFormat()
{
    HighlightRule rule;
    //双斜杠匹配
    rule.pattern = QRegularExpression("\\/\\/.*$");
    QTextCharFormat commentFormat;
    commentFormat.setFont(QFont(mFontFamily, mFontSize));
    //颜色的枚举值
    commentFormat.setForeground(Qt::darkGreen);
    //设置斜体
    commentFormat.setFontItalic(true);
    rule.format = commentFormat;
    highlightRules.append(rule);
}

void Highlighter4Text::addKeywordsFormat() {
    HighlightRule rule;
    //keyWordsStream,注意路径
    QFile file(":/config/keywords.txt");
    QTextStream keyWordsStream(&file);
    QTextCharFormat keywordsFormat;
    keywordsFormat.setFont(QFont(mFontFamily,mFontSize));
    keywordsFormat.setForeground(Qt::darkMagenta);
    rule.format = keywordsFormat;

    //以只读和text方式打开txt文件
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        keyWordsStream.seek(0);
        QString word;
        //如果没有读完
        while(!keyWordsStream.atEnd())
        {
            //每一行都是一个关键字
            word =  keyWordsStream.readLine();
            if(word!="")
            {
                //匹配一个单词边界，也就是指单词和空格间的位置。
                // 例如， 'er\\b' 可以匹配"never" 中的 'er'，但不能匹配 "verb" 中的 'er'。
                rule.pattern = QRegularExpression("\\b"+word+"\\b");
                highlightRules.append(rule);
            }
        }
        file.close();
    }
}


void Highlighter4Text::addClassNameFormat()
{
    HighlightRule rule;
    rule.pattern = QRegularExpression("\\b[A-Z]+\\w*");
    QTextCharFormat ClassNameFormat;
    ClassNameFormat.setFont(QFont(mFontFamily,mFontSize));
    ClassNameFormat.setForeground(QColor(150,20,100));
    ClassNameFormat.setFontWeight(99);
    rule.format = ClassNameFormat;
    highlightRules.append(rule);
}

void Highlighter4Text::addFunctionFormat()
{
    HighlightRule rule;
    QTextCharFormat functionFormat;
    functionFormat.setFont(QFont(mFontFamily,mFontSize));
    functionFormat.setForeground(QColor(200,0,150));
    functionFormat.setFontWeight(99);
    rule.format = functionFormat;
    //匹配前括号部分
    rule.pattern = QRegularExpression("\\w+\\(");
    highlightRules.append(rule);
    //匹配后括号部分
    rule.pattern = QRegularExpression("\\)");
    highlightRules.append(rule);
}

void Highlighter4Text::addMultiLineCommentFormat(const QString &text){

    setCurrentBlockState(0);
    // /*
    QRegularExpression commentBeginReg("/\\*");
    // */
    QRegularExpression commentEndReg("\\*/");
    QTextCharFormat multiLineCommentFormat;
    multiLineCommentFormat.setFont(QFont(mFontFamily,mFontSize));
    multiLineCommentFormat.setForeground(Qt::darkGreen);
    multiLineCommentFormat.setFontItalic(true);

    int startIndex = 0;
    //状态不为1，则处理开头
    if(previousBlockState()!=1)
    {
        auto matchBegin= commentBeginReg.match(text);
        startIndex = matchBegin.capturedStart();
    }
    //确实存在开头符，则寻找结尾，并执行高亮
    while(startIndex>=0)
    {
        auto endMatch = commentEndReg.match(text);
        int endIndex = endMatch.capturedEnd();
        int commentLength = 0;
        if(endIndex == -1)
        {
            //当前text没结束，即comment还未完，则把当前处理的text状态改为1
            setCurrentBlockState(1);
            //这一块text中comment的长度
            commentLength = text.length() - startIndex;
        }
        else
        {
            //当前text找到了结尾符
            //内容长度+结束符的长度
//            commentLength = endIndex - startIndex + endMatch.capturedLength();
            commentLength = endIndex - startIndex;
        }
        //执行高亮
        setFormat(startIndex,commentLength,multiLineCommentFormat);

        //寻找后面的开头符
//        auto matchNextBegin= commentBeginReg.match(text, startIndex + commentLength);
        auto matchNextBegin= commentBeginReg.match(text, endIndex);

        //如果在后面能找到开头符，则开启下一轮循环，找不到则退出
        startIndex = matchNextBegin.capturedStart();
    }
}