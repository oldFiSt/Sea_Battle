#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "friendwindow.h"
#include "connect_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondWindow(nullptr)
    , friendWindow(nullptr)
    , connectWindow(nullptr)
{
    setFixedSize(800, 588);
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
    if (connectWindow) delete connectWindow;
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
    if (!connectWindow) {
        connectWindow = new ConnectWindow(this);

        connect(connectWindow, &ConnectWindow::backtoMenu, this, [this]() {
            connectWindow->close();
            this->show();
        });

        connect(connectWindow, &ConnectWindow::connectedToServer, this, [this]() {
            connectWindow->close();

            if (!friendWindow){
                friendWindow = new FriendWindow(this);
            }
            friendWindow->show();
        });
    }
    connectWindow->show();
}

void MainWindow::on_btnExitToDesktop_clicked()
{
    QApplication::quit();
}
