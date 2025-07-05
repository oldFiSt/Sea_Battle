#ifndef FRIENDWINDOW_H
#define FRIENDWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QList>
#include <QPoint>
#include <QSet>
#include <QRandomGenerator>
#include <QMessageBox>

namespace Ui {
class FriendWindow;
}

class FriendWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FriendWindow(QWidget *parent = nullptr);
    ~FriendWindow();

private slots:
    void handlePlayer1CellClick();
    void handlePlayer2CellClick();
    void restartGame();

private:
    Ui::FriendWindow *ui;

    QGridLayout* player1GridLayout;
    QGridLayout* player2GridLayout;
    QPushButton* player1Field[10][10];
    QPushButton* player2Field[10][10];

    QList<QList<QPoint>> player1Ships;
    QList<QList<QPoint>> player1ShipsForever;
    QList<QList<QPoint>> player2Ships;
    QList<QList<QPoint>> player2ShipsForever;

    bool player1Turn;
    int player1ShipsAlive;
    int player2ShipsAlive;

    void setupFields();
    void placeShipsRandomly(QPushButton* field[10][10], QList<QList<QPoint>>& shipsGrouped, QList<QList<QPoint>>& shipsForever);
    bool isValidShipPlacement(const QList<QPoint>& ship, QPushButton* field[10][10]);
    void handlePlayerMove(int row, int col, QPushButton* targetField[10][10],
                          QList<QList<QPoint>>& targetShipsGrouped,
                          QList<QList<QPoint>>& targetShipsForever,
                          int& targetShipsAlive,
                          QLabel* label);
    void checkAndHandleDestroyedShip(QPoint hitPoint,
                                     QList<QList<QPoint>>& targetShipsGrouped,
                                     QList<QList<QPoint>>& targetShipsForever,
                                     int& targetShipsAlive,
                                     QPushButton* targetField[10][10],
                                     QLabel* label);
    void markSurroundingArea(const QList<QPoint>& ship, QPushButton* field[10][10]);
    void switchTurn();
    void updateShipsCount();
    void checkGameOver();
    void resetGame();
};

#endif // FRIENDWINDOW_H
