#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include "secondwindow.h" // Убедитесь, что путь правильный
#include <QPushButton>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QPoint>

namespace Ui {
class BattleWindow;
}

class BattleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BattleWindow(const QList<QList<QPoint>>& playerShipsGroups, QWidget *parent = nullptr);
    ~BattleWindow();

private:
    Ui::BattleWindow *ui;

    int playerShipsAlive;
    int enemyShipsAlive;


    QGridLayout* playerGridLayout;
    QGridLayout* enemyGridLayout;

    friend class TestBattleWindow;

    QPushButton* playerField[10][10];
    QPushButton* enemyField[10][10];

    void markPlayerDestroyedShip(const QList<QPoint>& ship);
    void checkPlayerShipDestroyed(QPoint hitPoint);
    QList<QList<QPoint>> playerShipsGrouped;
    QList<QList<QPoint>> playerShipsGroupedFOREVER;
    QList<QList<QPoint>> enemyShipsGrouped;
    QList<QList<QPoint>> enemyShipsGroupedFOREVER;
    QList<QPoint> playerShips;

    bool playerTurn;
    QTimer* botTimer;
    bool hitStatus[10][10];

    void setupFields();
    void placePlayerShips();
    void placeEnemyShipsRandomly();
    bool isValidEnemyShipPlacement(const QList<QPoint>& ship);
    void checkAndHandleDestroyedShip(QPoint hitPoint);
    void markSurroundingArea(const QList<QPoint>& ship);
    void botShoot();

    // 👇 Добавлено для умного ИИ
    QPoint lastHitPoint;                // Последнее попадание по кораблю
    QVector<QPoint> targetQueue;        // Очередь клеток для прицельного добивания
    bool botHuntingMode = false;        // Активен ли режим охоты
};

#endif // BATTLEWINDOW_H
