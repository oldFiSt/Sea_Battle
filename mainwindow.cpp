#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "connect_window.h"
#include "shipplacementwindow.h"
#include "battlewindow.h"
#include "GameClient.h"
#include "GameServer.h"
#include "multiplayermodewindow.h"
#include "hostlobbywindow.h"
#include "hostsetupwindow.h"
#include "guidewindow.h"
#include <QNetworkInterface>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondWindow(nullptr) // Инициализируем все указатели нулем
    , connectWindow(nullptr)
    , shipPlacementWindow(nullptr)
    , battleWindow(nullptr)
    , gameServer(nullptr)
    , gameClient(nullptr)
    , multiplayerModeWindow(nullptr)
    , hostLobbyWindow(nullptr)
    , hostSetupWindow(nullptr)
    , guideWindow(nullptr)
    , playerID(0)
    , m_iAmReady(false)
    , m_opponentIsReady(false)
{
    setFixedSize(800, 575);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    cleanupNetwork(true);
    cleanupGameWindows();
    delete ui;
}

// Новый метод для очистки игровых окон
void MainWindow::cleanupGameWindows()
{
    if (secondWindow) {
        secondWindow->deleteLater();
        secondWindow = nullptr;
    }
    if (battleWindow) {
        battleWindow->deleteLater();
        battleWindow = nullptr;
    }
}

void MainWindow::cleanupNetwork(bool force) {
    if (gameClient) {
        if(gameClient->property("connected").toBool()) {
            gameClient->disconnectFromServer();
        }
        gameClient->deleteLater();
        gameClient = nullptr;
    }
    if (gameServer) {
        gameServer->deleteLater();
        gameServer = nullptr;
    }
    if (connectWindow && force) {
        connectWindow->deleteLater();
        connectWindow = nullptr;
    }
    if (guideWindow) {
        guideWindow->close();
    }
    m_iAmReady = false;
    m_opponentIsReady = false;
    m_myShips.clear();
}

// Этот метод теперь вызывает наш новый централизованный метод
void MainWindow::on_pushButton_clicked() {
    startNewSinglePlayerGame();
}

// Новый централизованный метод для начала/рестарта одиночной игры
void MainWindow::startNewSinglePlayerGame()
{
    this->hide();
    cleanupGameWindows(); // Очищаем старые окна перед созданием новых

    secondWindow = new SecondWindow(this);
    secondWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(secondWindow, &SecondWindow::shipsPlacedAndReady, this, &MainWindow::startSinglePlayerBattle);
    // Если окно расстановки закроют, покажем главное меню
    connect(secondWindow, &QObject::destroyed, this, [this]() {
        // Убедимся, что главное окно показывается, только если не начался бой
        if (!battleWindow) {
            this->show();
        }
    });

    secondWindow->show();
}

void MainWindow::startSinglePlayerBattle(const QList<QList<QPoint>>& ships)
{
    cleanupGameWindows(); // Очищаем окно расстановки

    battleWindow = new BattleWindow(ships, this, this);
    battleWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(battleWindow, &QObject::destroyed, this, [this]() {
        this->battleWindow = nullptr;
    });

    battleWindow->show();
}


// --- Остальной код без существенных изменений ---

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    if (!multiplayerModeWindow) {
        multiplayerModeWindow = new MultiplayerModeWindow(this);
        connect(multiplayerModeWindow, &MultiplayerModeWindow::hostGameClicked, this, &MainWindow::onHostGameClicked);
        connect(multiplayerModeWindow, &MultiplayerModeWindow::joinGameClicked, this, &MainWindow::onJoinGameClicked);
        connect(multiplayerModeWindow, &MultiplayerModeWindow::backClicked, this, [this](){ onMultiplayerBackClicked(multiplayerModeWindow); });
    }
    multiplayerModeWindow->show();
}

void MainWindow::onHostGameClicked()
{
    multiplayerModeWindow->hide();
    cleanupNetwork(true);

    if (!hostSetupWindow) {
        hostSetupWindow = new HostSetupWindow(this);
        connect(hostSetupWindow, &HostSetupWindow::hostGame, this, &MainWindow::startServerAndLobby);
        connect(hostSetupWindow, &HostSetupWindow::backToMenu, this, [this](){
            hostSetupWindow->hide();
            multiplayerModeWindow->show();
            cleanupNetwork(true);
        });
    }
    hostSetupWindow->show();
}

void MainWindow::startServerAndLobby(quint16 port)
{
    hostSetupWindow->hide();

    gameServer = new GameServer(this);
    gameClient = new GameClient(this);
    gameClient->setProperty("connected", false);

    connect(gameClient, &GameClient::errorOccurred, this, [this](const QString& error) {
        QMessageBox::critical(this, "Ошибка подключения", "Не удалось подключиться к серверу: " + error);
        if (hostLobbyWindow) hostLobbyWindow->hide();
        cleanupNetwork(true);
        hostSetupWindow->show();
    });
    connect(gameClient, &GameClient::connected, this, [this](){ gameClient->setProperty("connected", true); });


    if (!gameServer->startServer(port)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось запустить сервер на порту " + QString::number(port) + ". Возможно, порт уже занят.");
        cleanupNetwork(true);
        hostSetupWindow->show();
        return;
    }

    connect(gameClient, &GameClient::dataReceived, this, &MainWindow::handleNetworkData);
    gameClient->connectToServer("127.0.0.1", port);

    if (!hostLobbyWindow) {
        hostLobbyWindow = new HostLobbyWindow(this);
        connect(hostLobbyWindow, &HostLobbyWindow::cancelled, this, [this](){
            hostLobbyWindow->hide();
            cleanupNetwork(true);
            this->show();
        });
    }

    QString ipAddress;
    const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : ipAddressesList) {
        if (address != QHostAddress::LocalHost && address.toIPv4Address()) {
            ipAddress = address.toString();
            break;
        }
    }
    if (ipAddress.isEmpty()) ipAddress = "127.0.0.1";
    hostLobbyWindow->setIpAndPort(ipAddress, port);
    hostLobbyWindow->show();
}

void MainWindow::onJoinGameClicked()
{
    multiplayerModeWindow->hide();
    cleanupNetwork(true);

    gameClient = new GameClient(this);
    connect(gameClient, &GameClient::dataReceived, this, &MainWindow::handleNetworkData);

    if (connectWindow) {
        delete connectWindow;
        connectWindow = nullptr;
    }
    connectWindow = new ConnectWindow(gameClient, this);

    connect(connectWindow, &ConnectWindow::backtoMenu, this, [this]() {
        cleanupNetwork(true);
        multiplayerModeWindow->show();
    });
    connect(connectWindow, &ConnectWindow::bothPlayersReady, this, &MainWindow::proceedToShipPlacement);

    connectWindow->show();
}

void MainWindow::onMultiplayerBackClicked(QWidget* senderWindow) {
    if (senderWindow) senderWindow->hide();
    this->show();
    cleanupNetwork(true);
}

void MainWindow::handleNetworkData(const QByteArray &data)
{
    QString msg = QString::fromUtf8(data);
    qDebug() << "Network data received:" << msg;

    if (msg.startsWith("ID:")) {
        this->playerID = msg.split(':').last().toInt();
    } else if (msg == "both connected") {
        if (hostLobbyWindow) hostLobbyWindow->hide();
        if (playerID == 1) {
            proceedToShipPlacement(this->playerID);
        }
    } else if (msg == "SHIPS_READY") {
        m_opponentIsReady = true;
        tryStartMultiplayerGame();
    }
}

void MainWindow::proceedToShipPlacement(int id)
{
    this->playerID = id;

    if (connectWindow) {
        connectWindow->disconnectFromClient();
        connectWindow->hide();
    }
    if (hostLobbyWindow) {
        hostLobbyWindow->hide();
    }

    if (shipPlacementWindow) {
        shipPlacementWindow->deleteLater();
    }
    shipPlacementWindow = new ShipPlacementWindow(this);
    connect(shipPlacementWindow, &ShipPlacementWindow::startGame, this, &MainWindow::onPlayerShipsPlaced);
    connect(shipPlacementWindow, &ShipPlacementWindow::backToConnectWindow, this, [this]() {
        shipPlacementWindow->hide();
        cleanupNetwork(true);
        multiplayerModeWindow->show();
    });

    shipPlacementWindow->show();
}

void MainWindow::onPlayerShipsPlaced(const QList<QList<QPoint>>& ships)
{
    m_myShips = ships;
    m_iAmReady = true;

    if(gameClient) {
        gameClient->sendData("SHIPS_READY");
    }

    tryStartMultiplayerGame();
}

void MainWindow::tryStartMultiplayerGame()
{
    if (m_iAmReady && m_opponentIsReady) {
        if (shipPlacementWindow) {
            shipPlacementWindow->hide();
        }

        if (battleWindow) {
            battleWindow->deleteLater();
            battleWindow = nullptr;
        }

        if (connectWindow) {
            connectWindow->deleteLater();
            connectWindow = nullptr;
        }

        battleWindow = new BattleWindow(m_myShips, gameClient, this->playerID, this, this);
        battleWindow->show();
    }
}

void MainWindow::on_btnUserGuide_clicked()
{
    if (!guideWindow) {
        guideWindow = new GuideWindow(this);
        guideWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(guideWindow, &QObject::destroyed, this, [this](){ guideWindow = nullptr; });
    }
    guideWindow->show();
    guideWindow->raise();
    guideWindow->activateWindow();
}

void MainWindow::on_btnExitToDesktop_clicked()
{
    QApplication::quit();
}
