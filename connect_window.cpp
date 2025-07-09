#include "connect_window.h"
#include "ui_connect_window.h"
#include "GameClient.h"
#include <QMessageBox>

ConnectWindow::ConnectWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectWindow)
{
    setFixedSize(800, 588);
    ui->setupUi(this);

    labelStatus = ui->labelStatus;

    client = new GameClient(this);

    connect(client, &GameClient::connected, this, &ConnectWindow::onConnected);
    connect(client, &GameClient::disconnected, this, &ConnectWindow::onDisconnected);
    connect(client, &GameClient::dataReceived, this, &ConnectWindow::onDataReceived);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_btnConnect_clicked()
{
    QString ip = ui->lineEditIP->text().trimmed();
    quint16 port = ui->lineEditPort->text().toUShort();

    if (ip.isEmpty() || port == 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные IP и порт.");
        return;
    }

    client->connectToServer(ip, port);
}

void ConnectWindow::on_btnBack_clicked()
{
    emit backtoMenu();
    this->close(); // Закрываем это окно
}

void ConnectWindow::onConnected()
{
    QMessageBox::information(this, "Успешно", "Соединение установленоd[sd[sd[sd[ds[ds[ds!");
    emit connectedToServer();
}

void ConnectWindow::onDisconnected()
{
    QMessageBox::warning(this, "Отключено", "Соединение с сервером потеряно.");
}

void ConnectWindow::onDataReceived(const QByteArray &data)
{
    if (QString(data) == "twoPlayersConnected") {
        onTwoPlayersConnected();
    }
}

void ConnectWindow::onTwoPlayersConnected()
{
    labelStatus->setText("Оба игрока подключены. Можно начинать игру!");
}
