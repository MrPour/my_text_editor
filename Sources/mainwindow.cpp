//
// Created by mrpour on 2023/6/16.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <QTimer>
#include <QFontMetrics>
#include <QPainter>

#include "Headers/mainwindow.h"
#include "Forms/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}
