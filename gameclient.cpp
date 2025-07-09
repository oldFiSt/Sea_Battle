#include "GameClient.h"
#include <QDataStream>
#include <QDebug>

GameClient::GameClient(QObject *parent)
    : QObject(parent),
    socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &GameClient::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &GameClient::onSocketError);
}

void GameClient::connectToServer(const QString &host, quint16 port)
{
    socket->abort(); // На случай, если подключение уже активно
    socket->connectToHost(host, port);
}

void GameClient::sendData(const QByteArray &data)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
    } else {
        qDebug() << "Не удалось отправить данные: нет соединения.";
    }
}

void GameClient::onConnected()
{
    qDebug() << "Успешное подключение к серверу.";
    emit connected();
}

void GameClient::onDisconnected()
{
    qDebug() << "Отключено от сервера.";
    emit disconnected();
}

void GameClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    qDebug() << "Ошибка сокета:" << socket->errorString();
    emit errorOccurred(socket->errorString());
}

void GameClient::onReadyRead() {
    QByteArray data = socket->readAll();
    emit dataReceived(data);

    QString msg = QString::fromUtf8(data);
    if (msg == "both connected") {
        emit twoPlayersConnected();
    }
}
