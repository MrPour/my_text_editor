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
    //绘制行号widget
    void LineNumberWidgetPaintEvent(QPaintEvent * event);
    //高亮鼠标点击的行
    void LineNumberWidgetMousePressEvent(QMouseEvent *event);
    //随着鼠标滚轮滚动
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
    void initHighLighter();
    //适应字体变化计算行统计组件的宽度
    int calculateLineNumberWidgetWidth();
private slots:
    //高亮当前行
    void highlightCurrentLine();
    //根据当前窗体的QRect更新行统计组件
    void repaintLineNumberWidget(QRect rect, int dy);
    //更新行统计组件的宽度
    void updateLineNumberWidgetWidth();
    //有新文字输入时 - 将保存状态改为否
    void updateSaveState();

protected:
    //窗口变化事件
    void resizeEvent(QResizeEvent *event) override;
signals:
   //父类已经提供了很多signals
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
    //一旦Widget刷新就会重绘
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
