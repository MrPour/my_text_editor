//
// Created by mrpour on 2023/6/16.
//

#ifndef MYQTDEMO_MAINWINDOW_H
#define MYQTDEMO_MAINWINDOW_H

#include "texteditor.h"
#include <QMainWindow>
#include <QSettings>

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
    void closeEvent(QCloseEvent *event) override;
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
    Ui::MainWindow *ui;
    QString mFontFamily;
    int mFontSize;
    QSettings * mSettings;

    //初始化父容器字体
    void initFont();
    //根据是否有tab页签刷新功能
    void refreshFunctionByTab();
    //创建新的标签页
    void createNewTab(QString string);
};


#endif //MYQTDEMO_MAINWINDOW_H
