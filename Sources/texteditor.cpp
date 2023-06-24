//
// Created by mrpour on 2023/6/17.
//

#include "texteditor.h"
#include "hightlighter4text.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QTextStream>
MyEditor::MyEditor(QWidget *parent, QFont font) : QPlainTextEdit(parent)
{
    //在editor中，随着外部事件/字体变化等的触发得到更新，并绘制出来
    lineNumberWidget = new LineNumberWidget(this);
    //初始化连接
    initConnect();
    //按规则高亮
    initHighLighter();
    //初始化字体
    setAllFont(font);
    //当前行高亮显示
    highlightCurrentLine();
    //设置行统计的宽度
    updateLineNumberWidgetWidth();
    //【继承父类的方法】设置文本的自动换行
    setLineWrapMode(QPlainTextEdit::NoWrap);
}

//析构，释放资源
MyEditor::~MyEditor()
{
    delete lineNumberWidget;
}

void MyEditor::initConnect() {
    //观察者模式：当信号发出时，被连接的槽函数会自动被回调。
    //光标点击到相应行，该行高亮显示
    connect(this, SIGNAL(cursorPositionChanged()),this, SLOT(highlightCurrentLine()));
    //当新增块时统计行也更新
    connect(this, SIGNAL(blockCountChanged(int)),this,SLOT(updateLineNumberWidgetWidth()));
    //窗体被重绘时，使用当前的QRect更新行统计组件
    connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(repaintLineNumberWidget(QRect, int)));
    //文字更新时更新存储状态
    connect(this,SIGNAL(textChanged()),this,SLOT(updateSaveState()));
}
void MyEditor::initHighLighter() {
    //QTextDocument 获取当前文件
    highlighter = new Highlighter4Text(document());
}
void MyEditor::setAllFont(QFont font) {
    //将字体改变
    setFont(font);
    //更新高亮字体
    highlighter->setFont(font);
    //字体变化引发左侧视窗的宽度改变
    updateLineNumberWidgetWidth();
}

void MyEditor::highlightCurrentLine() {
        QList<QTextEdit::ExtraSelection> extraSelections;
        //高亮格式,默认对当前行改背景色
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor(0,100,100,20));
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        //获取光标
        selection.cursor = textCursor();
        extraSelections.append(selection);
        setExtraSelections(extraSelections);
}

int MyEditor::calculateLineNumberWidgetWidth()
{
    //获取统计行号面板的宽度 -- 行的宽度随字体的尺寸正相关变化
    //blockCount -- 文本块个数(文本块/回车) fontMetrics -- 字体尺寸 horizontalAdvance -- 覆盖一个字符串的像素宽度
    return 8+QString::number(blockCount()+1).length()*fontMetrics().horizontalAdvance(QChar('0'));
}

void MyEditor::repaintLineNumberWidget(QRect rect, int dy)
{
    if(dy)
        lineNumberWidget->scroll(0,dy);
    else
        //重新绘制组件
        lineNumberWidget->update(0,rect.y(),calculateLineNumberWidgetWidth(),rect.height());
}

void MyEditor::updateLineNumberWidgetWidth()
{
    //设置编辑框的视口大小，参数1为左侧的多余空白
    setViewportMargins(calculateLineNumberWidgetWidth(),0,0,0);
}

void MyEditor::updateSaveState()
{
    isSaved = false;
}

void MyEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    //行统计控件的宽高及位置坐标，空间高度和当前rect的高度相同
    //contentsRect()返回当前布局的矩形
    lineNumberWidget->setGeometry(0,0,calculateLineNumberWidgetWidth(),contentsRect().height());
}

void MyEditor::LineNumberWidgetPaintEvent(QPaintEvent *event)
{
    //绘制行号统计widget
    QPainter painter(lineNumberWidget);
    //绘制区域行号
    painter.fillRect(event->rect(),QColor(100,100,100,20));
    //拿到block
    QTextBlock block = firstVisibleBlock();
    //拿到行号
    int blockNumber = block.blockNumber();
    //拿到当前的block的top
    int cursorTop = blockBoundingGeometry(textCursor().block()).translated(contentOffset()).top();
    //拿到block的top
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    //拿到block的bottom
    int bottom = top + blockBoundingRect(block).height();
    while (block.isValid()&&top<=event->rect().bottom())
    {
        //设置画笔颜色
        painter.setPen(cursorTop == top?Qt::black:Qt::gray);
        //绘制文字，行号从1开始，所以+1
        painter.drawText(0,top,calculateLineNumberWidgetWidth()-4,bottom-top,Qt::AlignRight,QString::number(blockNumber+1));
        //下一个block
        block = block.next();

        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        blockNumber++;
    }
}

void MyEditor::LineNumberWidgetMousePressEvent(QMouseEvent *event)
{
    setTextCursor(QTextCursor(document()->findBlockByLineNumber(event->y()/fontMetrics().height()+verticalScrollBar()->value())));
}

void MyEditor::LineNumberWidgetWheelEvent(QWheelEvent *event)
{
    //QT6中无orientation方法获取滚轮是水平还是垂直，因此根据值来判断滚动方向
    if(event->angleDelta().x()!=0)
    {
        //鼠标水平方向滚动的距离
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+event->angleDelta().x());
    }
    else
    {
        //鼠标竖直方向滚动的距离
        verticalScrollBar()->setValue(verticalScrollBar()->value()-event->angleDelta().y()/4);
    }
    event->accept();
}

bool MyEditor::saveFile()
{
    QString fileName;

    //保存时弹出保存文件的对话框，
    // 父界面指针是this
    // 标题caption是"保存文件"
    //返回用户选择的文件名
    if(mFilename.isEmpty())
    {
        //这个fileName包含了文件的跟目录
        fileName = QFileDialog::getSaveFileName(this,"保存文件");
        //注意，将当前打开的文件名更新
        mFilename = fileName;
    }
    else
    {
        //当前文本是有文件名的，就不用弹框
        fileName = mFilename;
    }

    //创建文件
    QFile file(fileName);
    //打开并写入
    // 打开失败
    if(!file.open(QIODevice::WriteOnly|QFile::Text))
    {
        //弹出警告框
        QMessageBox::warning(this,"warning","failed to save the file "+file.errorString());
        return false;
    }
    //打开成功，将数据写入到文件
    QTextStream out(&file);
    //QPlainTextEdit 多行简单文本框用 toPlainText() 读取内容
    out<<toPlainText();
    file.close();
    //当前文本是否保存
    isSaved = true;
    return true;
}

bool MyEditor::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,"另存为");
    //创建文件
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QFile::Text))
    {
        //弹出警告框
        QMessageBox::warning(this,"warning","failed to save the file "+file.errorString());
        return false;
    }
    mFilename = fileName;
    //打开成功，将数据写入到文件
    QTextStream out(&file);
    //QPlainTextEdit 多行简单文本框用 toPlainText() 读取内容
    out<<toPlainText();
    file.close();
    //当前文本是否保存
    isSaved = true;
    return true;
}

void MyEditor::setFileName(QString filename)
{
    mFilename = filename;
}

QString MyEditor::getFileName() {
    return mFilename;
}

bool MyEditor::checkSaved()
{
    return isSaved;
}