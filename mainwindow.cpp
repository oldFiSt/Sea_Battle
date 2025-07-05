#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "friendwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondWindow(nullptr)
    , friendWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->btnExitToDesktop, &QPushButton::clicked, this, &MainWindow::on_btnExitToDesktop_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (secondWindow) delete secondWindow;
    if (friendWindow) delete friendWindow;
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    if (!secondWindow) {
        secondWindow = new SecondWindow(this);
    }
    secondWindow->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    if (!friendWindow) {
        friendWindow = new FriendWindow(this);
    }
    friendWindow->show();
}

void MainWindow::on_btnExitToDesktop_clicked()
{
    QApplication::quit();
}
