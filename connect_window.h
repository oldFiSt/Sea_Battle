#ifndef CONNECT_WINDOW_H
#define CONNECT_WINDOW_H

#include <QDialog>
#include <QLabel>


namespace Ui {
    class ConnectWindow;
}

class GameClient;

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

signals:
    void backtoMenu();
    void connectedToServer();

private slots:
    void on_btnConnect_clicked();
    void on_btnBack_clicked();
    void onTwoPlayersConnected();
    void onConnected();
    void onDisconnected();
    void onDataReceived(const QByteArray &data); // если нужно

private:
    Ui::ConnectWindow *ui;
    GameClient *client;
    QLabel *labelStatus;
};

#endif
