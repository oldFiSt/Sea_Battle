#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QList>
#include <QPoint>

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void onCellClickedPlayer();  // Слот для обработки клика по полю игрока
    void onCellClickedBot();     // Слот для обработки клика по полю бота

private:
    Ui::GameWindow *ui;

    // Массивы для полей
    QPushButton *playerGrid[10][10];
    QPushButton *botGrid[10][10];

    // Массивы для кораблей
    QList<QList<QPoint>> playerShips;
    QList<QList<QPoint>> botShips;

    // Инициализация полей и кораблей
    void initializeGrids();

    // Проверка попадания в корабль
    bool isHit(int row, int col, QList<QList<QPoint>> &ships);

    // Удаление корабля по клетке
    void removeShipAt(int row, int col, QList<QList<QPoint>> &ships);

    // Проверка на окончание игры
    bool isGameOver(QList<QList<QPoint>> &ships);

    // Ход бота
    void botTurn();
};

#endif // GAMEWINDOW_H
