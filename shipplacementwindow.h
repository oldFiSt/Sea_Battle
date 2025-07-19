#ifndef SHIPPLACEMENTWINDOW_H
#define SHIPPLACEMENTWINDOW_H

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QPushButton>

namespace Ui {
class ShipPlacementWindow;
}

class ShipPlacementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ShipPlacementWindow(QWidget *parent = nullptr);
    ~ShipPlacementWindow();

signals:
    void backToConnectWindow();
    void startGame(const QList<QList<QPoint>>& ships);

private slots:
    void selectShip1();
    void selectShip2();
    void selectShip3();
    void selectShip4();
    void handleCellClick();
    void onStartBattleClicked();
    void onBackClicked();
    void onResetShipsClicked();
    void onAutoPlaceShipsClicked(); // <-- ДОБАВЛЕН НОВЫЙ СЛОТ

private:
    Ui::ShipPlacementWindow *ui;
    QPushButton* gridButtons[10][10];
    int currentShipSize;
    QList<QPoint> currentPlacement;
    QList<QList<QPoint>> allPlacedShips;
    int shipsPlaced[5];
    const int maxShips[5] = {0, 4, 3, 2, 1};

    enum ShipOrientation { UNKNOWN, HORIZONTAL, VERTICAL };
    ShipOrientation currentOrientation;

    void initializeShipCounts();
    void selectShip(int size);
    void updateShipCountDisplay();
    bool isAdjacentToAnotherShip(const QList<QPoint>& ship);
    void resetCurrentPlacement();

    // <-- ДОБАВЛЕНЫ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
    void clearBoard();
    bool autoPlaceAllShips();
    bool isValidPlacementForAuto(const QList<QPoint>& ship);
};

#endif // SHIPPLACEMENTWINDOW_H
