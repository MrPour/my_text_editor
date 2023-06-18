//
// Created by mrpour on 2023/6/16.
//

#ifndef MYQTDEMO_MAINWINDOW_H
#define MYQTDEMO_MAINWINDOW_H

#include "texteditor.h"
#include <QMainWindow>
#include <QSettings>

//声明Ui命名空间，此命名空间也可以通过#include "Forms/ui_MainWindow.h"引入
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void saveSuccessAction(MyEditor* myEditor);
protected:
    //重写关闭整个容器的函数
    void closeEvent(QCloseEvent *event) override;

    //1、对ui对象中的actions绑定对应的slots
    //用ui设计不必再写connect()函数,在ui里编写QAction会在此处自动生成相应的槽函数
private slots:
    void on_new_file_triggered();

    void on_save_file_triggered();

    void on_open_file_triggered();

    void on_save_as_triggered();

    void on_paste_triggered();

    void on_copy_triggered();

    void on_about_triggered();

    void on_cut_triggered();

    void on_font_triggered();

    void on_undo_triggered();

    void on_redo_triggered();

    void on_exit_triggered();

    void on_print_triggered();

    void on_tabWidget_tabCloseRequested(int index);
private:

    //对象指针 这里的指针指向的是来自于ui_MainWindow.h中Ui_MainWindow的子类Ui::MainWindow
    Ui::MainWindow *ui;
    //两个字体相关的成员变量
    QString mFontFamily;
    int mFontSize;
    //初始配置信息
    QSettings * mSettings;

    //2、初始化容器字体
    void initFont();
    //3、根据是否有tab页签对Ui按钮进行管理
    void refreshFunctionByTab();
    //4、创建新的标签页MyEditor
    void createNewTab(QString string);
};


#endif //MYQTDEMO_MAINWINDOW_H
