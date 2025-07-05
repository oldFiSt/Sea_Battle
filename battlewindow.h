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
class SecondWindow;

namespace Ui {
class BattleWindow;
}

class BattleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BattleWindow(const QList<QList<QPoint>>& playerShipsGroups, MainWindow* mainWindow, QWidget *parent = nullptr);
    ~BattleWindow();

private slots:
    void on_btnExitToMainMenu_clicked();

private:
    Ui::BattleWindow *ui;
    MainWindow* mainWindowPtr;

    QGridLayout* playerGridLayout;
    QGridLayout* enemyGridLayout;
    QPushButton* playerField[10][10];
    QPushButton* enemyField[10][10];

    QList<QList<QPoint>> playerShipsGrouped;
    QList<QList<QPoint>> playerShipsGroupedFOREVER;
    QList<QList<QPoint>> enemyShipsGrouped;
    QList<QList<QPoint>> enemyShipsGroupedFOREVER;
    QList<QPoint> playerShips;

    QTimer* botTimer;
    bool hitStatus[10][10];

    int playerShipsAlive;
    int enemyShipsAlive;
    bool playerTurn;

    QPoint lastHitPoint;
    QVector<QPoint> targetQueue;
    bool botHuntingMode;

    void setupFields();
    void placePlayerShips();
    void placeEnemyShipsRandomly();
    bool isValidEnemyShipPlacement(const QList<QPoint>& ship);
    void checkAndHandleDestroyedShip(QPoint hitPoint);
    void markSurroundingArea(const QList<QPoint>& ship);
    void botShoot();
    void markPlayerDestroyedShip(const QList<QPoint>& ship);
    void checkPlayerShipDestroyed(QPoint hitPoint);
};

#endif // BATTLEWINDOW_H
