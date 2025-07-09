// GameServer.cpp
#include "GameServer.h"
#include <QDebug>

GameServer::GameServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

void GameServer::startServer(quint16 port) {
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Не удалось запустить сервер:" << server->errorString();
    } else {
        qDebug() << "Сервер запущен на порту" << port;
    }
}

void GameServer::onNewConnection() {
    if (clients.size() >= 2) {
        QTcpSocket *extra = server->nextPendingConnection();
        extra->disconnectFromHost();
        qDebug() << "Отклонено подключение: максимум 2 игрока";
        return;
    }

    QTcpSocket *client = server->nextPendingConnection();
    clients.append(client);
    qDebug() << "Игрок подключился. Сейчас игроков:" << clients.size();

    connect(client, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);

    // 🔽 Добавь вот эту часть:
    if (clients.size() == 2) {
        emit twoPlayersConnected();
        for (QTcpSocket *client : clients) {
            client->write("both connected");
        }
    }
}


void GameServer::onReadyRead() {
    QTcpSocket *senderClient = qobject_cast<QTcpSocket*>(sender());
    if (!senderClient) return;

    QByteArray data = senderClient->readAll();


    for (QTcpSocket *client : clients) {
        if (client != senderClient && client->state() == QAbstractSocket::ConnectedState) {
            client->write(data);
        }
    }
}

void GameServer::onDisconnected() {
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    clients.removeAll(client);
    client->deleteLater();
    qDebug() << "Клиент отключился. Осталось:" << clients.size();
}


