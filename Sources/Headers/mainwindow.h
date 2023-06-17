//
// Created by mrpour on 2023/6/16.
//

#ifndef MYQTDEMO_MAINWINDOW_H
#define MYQTDEMO_MAINWINDOW_H

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};


#endif //MYQTDEMO_MAINWINDOW_H
