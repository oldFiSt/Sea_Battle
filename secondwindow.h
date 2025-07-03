#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QPushButton>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void selectShip1();
    void selectShip2();
    void selectShip3();
    void selectShip4();
    void handleCellClick();
    void startBattle();

private:
    Ui::SecondWindow *ui;
    QPushButton* gridButtons[10][10];
    int currentShipSize = 0;
    QList<QPoint> currentPlacement;
    QList<QList<QPoint>> allPlacedShips;  // Убедитесь, что это QList<QList<QPoint>>
    int shipsPlaced[5]; // Индекс 1-4 соответствует размеру корабля
    const int maxShips[5] = {0, 4, 3, 2, 1}; // Максимальное количество кораблей каждого размера

    void initializeShipCounts();
    void selectShip(int size);
    void updateShipCountDisplay();
    bool isValidShipPlacement(const QList<QPoint>& ship);
    bool isAdjacentToAnotherShip(const QList<QPoint>& ship);
    void resetCurrentPlacement();
};

#endif // SECONDWINDOW_H
