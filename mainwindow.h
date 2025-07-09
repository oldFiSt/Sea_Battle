#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SecondWindow;
class FriendWindow;
class ConnectWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_btnExitToDesktop_clicked();

private:
    Ui::MainWindow *ui;
    SecondWindow *secondWindow;
    FriendWindow *friendWindow;
    ConnectWindow *connectWindow;
};
#endif // MAINWINDOW_H
