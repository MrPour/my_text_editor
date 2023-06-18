//
// Created by mrpour on 2023/6/16.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include "Headers/mainwindow.h"
#include "Forms/ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QPainter>


//构造函数使用列表初始化，首先调用父类构造器初始化父类的成员，再对自己的成员变量ui赋值
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //将tab窗口置于中心
    this->setCentralWidget(ui->tabWidget);
    mSettings = new QSettings("setting.ini", QSettings::IniFormat);
    initFont();
    refreshFunctionByTab();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initFont()
{
    //取出初始化参数配置父容器
    mFontFamily = mSettings->value("font_family", "Consolas").toString();
    mFontSize = mSettings->value("font_size", 14).toInt();
}
void MainWindow::refreshFunctionByTab()
{
    //判断是否存在tab页签
   bool valid =  ui->tabWidget->count()!=0;
   //对ui按钮进行开启和关闭
    ui->save_file->setEnabled(valid);
    ui->save_as->setEnabled(valid);
    ui->copy->setEnabled(valid);
    ui->paste->setEnabled(valid);
    ui->cut->setEnabled(valid);
    ui->print->setEnabled(valid);
    ui->undo->setEnabled(valid);
    ui->redo->setEnabled(valid);
}

void MainWindow::on_new_file_triggered()
{
    //新开的标签页是MyEditor，且字体与父容器相同
    ui->tabWidget->addTab(new MyEditor(this,QFont(mFontFamily,mFontSize)),"new tab");
    refreshFunctionByTab();
}

void MainWindow::on_save_file_triggered()
{
        MyEditor* myEditor = (MyEditor*)ui->tabWidget->currentWidget();
        if(myEditor)
        {
            if(myEditor->saveFile())
            {
                //保存成功，更新状态
                saveSuccessAction(myEditor);
            }
            else
            {
                QMessageBox::warning(this, "warning", "failed to save the file");
            }
        }
}

void MainWindow::on_open_file_triggered()
{
    //选择文件的弹出框为"open file"，返回值为所选中的文件名作为新tab的名字
    createNewTab(QFileDialog::getOpenFileName(this,"open file"));
}

void MainWindow::createNewTab(QString fineName)
{
    QFile file(fineName);
    //以text的方式打开文件
    if(!file.open(QIODevice::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this, "warning", "failed to open the file" + file.errorString());
        return;
    }
    QTextStream in(&file);
    //读入text
    QString text = in.readAll();
    //为其创建tab
    MyEditor* myEditor = new MyEditor(this,QFont(mFontFamily,mFontSize));
    //将text输入到tab里
    myEditor->setPlainText(text);
    myEditor->setFileName(fineName);
    ui->tabWidget->addTab(myEditor,fineName);
    refreshFunctionByTab();
    //设置此次新开的tab标签的序号
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    file.close();
}

void MainWindow::on_save_as_triggered()
{
    MyEditor* myEditor = (MyEditor*)ui->tabWidget->currentWidget();
    if(myEditor)
    {
        if(myEditor->saveAsFile())
        {
            //保存成功，更新状态
            saveSuccessAction(myEditor);
        }
        else
        {
            QMessageBox::warning(this, "warning", "failed to save the file");
        }
    }
}

void MainWindow::on_paste_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
        myEditor->paste();
    }
}

void MainWindow::on_copy_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
        myEditor->copy();
    }
}

void MainWindow::on_cut_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
        myEditor->cut();
    }
}

void MainWindow::on_about_triggered()
{
    QMessageBox::about(this, "欢迎使用myEditor编辑器", "这是myEditor第一版, 欢迎使用！");
}

void MainWindow::on_font_triggered()
{
    bool fontSelected;
    //选择字体，并获得所选中的字体
    QFont font = QFontDialog::getFont(&fontSelected,QFont(mFontFamily,mFontSize),this);
    if(fontSelected)
    {
        MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
        if(myEditor)
        {
            myEditor->setAllFont(font);
        }
        mSettings->setValue("font_family", font.family());
        mSettings->setValue("font_size", font.pointSize());
    }
}

void MainWindow::on_undo_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
        myEditor->undo();
    }
}

void MainWindow::on_redo_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
        myEditor->redo();
    }
}

void MainWindow::on_exit_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_print_triggered()
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();
    if (myEditor)
    {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
        QPrinter printDev;
#if QT_CONFIG(printdialog)
        QPrintDialog dialog(&printDev, this);
        if (dialog.exec() == QDialog::Rejected)
            return;
#endif
        codeEditor->print(&printDev);
#endif
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    MyEditor *myEditor = (MyEditor *)ui->tabWidget->currentWidget();

    //检查当前关闭的页签是否保存
    if (!myEditor->checkSaved())
    {
        //弹出框并获得所选结果
        QMessageBox::StandardButton btn = QMessageBox::question(this, "警告", "尚未保存，是否先保存再关闭?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (btn == QMessageBox::Yes)
        {
            if (myEditor->saveFile())
            {
                saveSuccessAction(myEditor);
            }
            return;
        }
        else if (btn == QMessageBox::Cancel)
        {
            return;
        }
    }
    delete myEditor;
    refreshFunctionByTab();
}

void MainWindow::saveSuccessAction(MyEditor *myEditor)
{
    QString filename = myEditor->getFileName();
    //更新tab页签名字和索引
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ui->tabWidget->count() > 0)
    {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "警告", "有未保存的文件,确定要关闭吗？", QMessageBox::Yes | QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}
