#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendData(const QByteArray &data);
    void disconnectFromServer(); // <-- НОВАЯ ФУНКЦИЯ

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);
    void twoPlayersConnected();

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
    QByteArray buffer;
};

#endif // GAMECLIENT_H
