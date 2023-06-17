//
// Created by mrpour on 2023/6/17.
//

#ifndef MYQTDEMO_TEXTEDITOR_H
#define MYQTDEMO_TEXTEDITOR_H
#include "hightlighter4text.h"
#include <QPlainTextEdit>
#include <QWidget>
class LineNumberWidget;
class MyEditor:public QPlainTextEdit{

    //完成QObject初始化的宏
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = nullptr,QFont font = QFont("Consolas",14));
    ~MyEditor();
    //绘制行号
    void LineNumberWidgetPaintEvent(QPaintEvent * event);
    void LineNumberWidgetMousePressEvent(QMouseEvent *event);
    void LineNumberWidgetWheelEvent(QWheelEvent *event);
    //保存内容功能
    bool saveFile();
    //另存为功能
    bool saveAsFile();
    //设置文件名
    void setFileName(QString filename);
    //获取文件名
    QString getFileName();
    //改变字体功能
    void setAllFont(QFont font);
    //检查是否保存
    bool checkSaved();
private:
    //行号的widget
    LineNumberWidget* lineNumberWidget;
    //高亮处理器
    Highlighter4Text* highlighter ;
    //当前文件名
    QString mFilename;
    //当前文件存储状态
    bool isSaved = false;
private:
    //连接功能
    void initConnect();
    //初始化高亮对象
    void initHighLight();
    //适应字体变化
    int getLineNumberWidgetWidth();
private slots:
    //高亮当前行
    void highlightCurrentLine();
    void updateLineNumberWidget(QRect rect,int dy);
    void updateLineNumberWidgetWidth();
    //更新保存状态
    void updateSaveState();

protected:
    //窗口变化事件
    void resizeEvent(QResizeEvent *event) override;
signals:

};

//绘制行号的挂件类
class LineNumberWidget:public QWidget
{
public:
    explicit LineNumberWidget(MyEditor *editor = nullptr):QWidget(editor)
    {
        codeEditor = editor;
    };

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->LineNumberWidgetPaintEvent(event);
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        codeEditor->LineNumberWidgetMousePressEvent(event);
    }
    void wheelEvent(QWheelEvent *event) override
    {
        codeEditor->LineNumberWidgetWheelEvent(event);
    }
private:
    //子类的成员变量
    MyEditor *codeEditor;
};


#endif //MYQTDEMO_TEXTEDITOR_H
