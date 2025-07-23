#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QPoint>

class MainWindow;
class GameClient;

namespace Ui {
class BattleWindow;
}

class BattleWindow : public QDialog
{
    Q_OBJECT

public:
    // Конструктор для игры с ботом
    explicit BattleWindow(const QList<QList<QPoint>>& playerShipsGroups, MainWindow* mainWindow, QWidget *parent = nullptr);
    // Конструктор для сетевой игры
    explicit BattleWindow(const QList<QList<QPoint>>& playerShipsGroups, GameClient* client, int playerID, MainWindow* mainWindow, QWidget *parent = nullptr);
    ~BattleWindow();

private slots:
    void on_btnExitToMainMenu_clicked();
    void on_btnReplay_clicked(); // <-- ДОБАВЛЕН ЯВНЫЙ СЛОТ
    void botShoot();
    void handlePlayerShot();
    void handleNetworkData(const QByteArray &data);

private:
    Ui::BattleWindow *ui;
    MainWindow* mainWindowPtr;
    GameClient* gameClient;

    QGridLayout* playerGridLayout;
    QGridLayout* enemyGridLayout;
    QPushButton* playerField[10][10];
    QPushButton* enemyField[10][10];

    QList<QList<QPoint>> playerShipsGrouped;
    QList<QList<QPoint>> playerShipsGroupedFOREVER;
    QList<QList<QPoint>> enemyShipsGrouped;
    QList<QList<QPoint>> enemyShipsGroupedFOREVER;

    QTimer* botTimer;

    int playerShipsAlive;
    int enemyShipsAlive;
    bool playerTurn;
    bool isNetworkGame;
    int myPlayerID;

    // --- Переменные для умного бота ---
    QPoint lastHitPoint;
    QPoint firstHitPoint;

    QVector<QPoint> targetQueue;
    bool botHuntingMode;

    void setupFields();
    void placePlayerShips();
    void placeEnemyShipsRandomly();
    bool isValidEnemyShipPlacement(const QList<QPoint>& ship);
    void checkAndHandleDestroyedShip(QPoint hitPoint, bool isPlayerShot);
    void markSurroundingArea(const QList<QPoint>& ship, QPushButton* field[10][10]);
    void updateTurnStatus();
    void setEnemyFieldEnabled(bool enabled);
};

#endif // BATTLEWINDOW_H
