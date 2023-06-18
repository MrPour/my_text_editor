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


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //将tab窗口置于中心
    this->setCentralWidget(ui->tabWidget);
    mSettings = new QSettings("setting.ini", QSettings::IniFormat);
    initFont();
    initAction();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initFont()
{
    //取出初始化配置
    mFontFamily = mSettings->value("font_family", "Consolas").toString();
    mFontSize = mSettings->value("font_size", 14).toInt();
}
void MainWindow::initAction()
{
    //判断是否存在tab页签
   bool valid =  ui->tabWidget->count()!=0;
   //对mainwidow的基本方法进行开启和关闭
   ui->save_file->setEnabled(valid);
    ui->save_as->setEnabled(valid);
    ui->copy->setEnabled(valid);
    ui->paste->setEnabled(valid);
    ui->cut->setEnabled(valid);
    ui->print->setEnabled(valid);
    ui->undo->setEnabled(valid);
    ui->redo->setEnabled(valid);
}
