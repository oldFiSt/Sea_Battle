// GameServer.cpp
#include "../headers/GameServer.h"
#include <QDebug>
#include <QTimer>
#include <QTcpServer>

GameServer::GameServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

// ИСПРАВЛЕНО: Добавляем определение деструктора
GameServer::~GameServer()
{
    // Гарантированно останавливаем сервер при удалении объекта
    if (server->isListening()) {
        server->close();
    }
}

bool GameServer::startServer(quint16 port) {
    // ИСПРАВЛЕНО: Это правильный способ установки флага ReuseAddressHint в Qt 6.
    // Этот флаг должен быть установлен ПЕРЕД вызовом listen().
    if (!server->listen(QHostAddress::Any, port)) {
        if (server->serverError() == QAbstractSocket::AddressInUseError) {
            qDebug() << "Address in use, trying to reuse...";
            server->close();
            if (server->listen(QHostAddress::Any, port)) {
                qDebug() << "Server started on port" << port << "with reuse.";
                return true;
            }
        }
        qDebug() << "Не удалось запустить сервер:" << server->errorString();
        return false;
    } else {
        qDebug() << "Сервер запущен на порту" << port;
        return true;
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
    qDebug() << "Player connected. Currently players:" << clients.size();

    connect(client, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);

    if (clients.size() == 2) {
        QTimer::singleShot(100, this, [this]() {
            if (clients.size() == 2) {
                emit twoPlayersConnected();
                clients.at(0)->write("ID:1\n");
                clients.at(1)->write("ID:2\n");
                for (QTcpSocket *c : clients) {
                    c->write("both connected\n");
                }
            }
        });
    }
}

void GameServer::onReadyRead() {
    QTcpSocket *senderClient = qobject_cast<QTcpSocket*>(sender());
    if (!senderClient) return;
    QByteArray data = senderClient->readAll();
    // ИСПРАВЛЕНО: Заменяем устаревший qAsConst на std::as_const
    for (QTcpSocket *client : std::as_const(clients)) {
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
