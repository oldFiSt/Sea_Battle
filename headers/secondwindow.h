#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QPushButton>

class MainWindow; // Forward declaration

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(MainWindow *parentMainWindow);
    ~SecondWindow();

    // === НОВЫЙ РАЗДЕЛ signals ===
signals:
    // Сигнал, который отправляется главному окну, когда корабли расставлены.
    void shipsPlacedAndReady(const QList<QList<QPoint>>& ships);

private slots:
    void selectShip1();
    void selectShip2();
    void selectShip3();
    void selectShip4();
    void handleCellClick();
    void startBattle();
    void returnToMainMenu();
    void onResetShipsClicked();
    void onAutoPlaceShipsClicked();

private:
    Ui::SecondWindow *ui;
    MainWindow* m_mainWindow; // Pointer to the main window
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
    void clearBoard();
    bool autoPlaceAllShips();
    bool isValidPlacementForAuto(const QList<QPoint>& ship);
    bool isAdjacentToAnotherShip(const QList<QPoint>& ship);
    void resetCurrentPlacement();
};

#endif // SECONDWINDOW_H
