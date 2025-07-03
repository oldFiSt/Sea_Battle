#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QList>

GameWindow::GameWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    // Инициализация сетки кнопок на поле игрока
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            playerGrid[row][col] = new QPushButton(this);
            playerGrid[row][col]->setStyleSheet("background-color: lightblue;");
            playerGrid[row][col]->setProperty("row", row);
            playerGrid[row][col]->setProperty("col", col);

            connect(playerGrid[row][col], &QPushButton::clicked, this, &GameWindow::onCellClickedPlayer);

            ui->playerGridLayout->addWidget(playerGrid[row][col], row, col);
        }
    }

    // Инициализация сетки кнопок на поле бота
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            botGrid[row][col] = new QPushButton(this);
            botGrid[row][col]->setStyleSheet("background-color: lightblue;");
            botGrid[row][col]->setProperty("row", row);
            botGrid[row][col]->setProperty("col", col);

            connect(botGrid[row][col], &QPushButton::clicked, this, &GameWindow::onCellClickedBot);

            ui->botGridLayout->addWidget(botGrid[row][col], row, col);
        }
    }

    // Инициализация кораблей игрока и бота
    initializeGrids();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::initializeGrids()
{
    // Пример расстановки кораблей игрока
    playerShips = {
        {QPoint(0, 0), QPoint(0, 1)},  // 2-палубный
        {QPoint(3, 3)},                // 1-палубный
        {QPoint(6, 6), QPoint(6, 7), QPoint(6, 8)},  // 3-палубный
        {QPoint(8, 1), QPoint(9, 1)}   // 2-палубный
    };

    // Пример расстановки кораблей бота
    botShips = {
        {QPoint(1, 1), QPoint(1, 2)},  // 2-палубный
        {QPoint(4, 4)},                // 1-палубный
        {QPoint(7, 6), QPoint(7, 7), QPoint(7, 8)},  // 3-палубный
        {QPoint(9, 9), QPoint(8, 9)}   // 2-палубный
    };

    // Отображение кораблей на поле игрока
    for (const auto &ship : playerShips) {
        for (const QPoint &point : ship) {
            playerGrid[point.x()][point.y()]->setStyleSheet("background-color: gray;");
            playerGrid[point.x()][point.y()]->setProperty("occupied", true);
        }
    }
}

void GameWindow::onCellClickedPlayer()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    // Игрок не должен стрелять по своему полю, только по полю бота
    if (row < 10 && col < 10) {
        // Если уже стреляли по этой клетке
        if (btn->styleSheet() == "background-color: red;" || btn->styleSheet() == "background-color: white;") {
            return;
        }

        // Проверка попадания по кораблю бота
        if (isHit(row, col, botShips)) {
            btn->setStyleSheet("background-color: red;");
            // Уничтожаем корабль на этой клетке
            removeShipAt(row, col, botShips);

            if (isGameOver(botShips)) {
                QMessageBox::information(this, "Победа", "Вы победили!");
            }
        } else {
            btn->setStyleSheet("background-color: white;");
        }

        // Ход бота
        botTurn();
    }
}

void GameWindow::onCellClickedBot()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    // Игрок не должен стрелять по своему полю, только по полю бота
    if (row < 10 && col < 10) {
        // Если уже стреляли по этой клетке, игнорируем
        if (btn->styleSheet() == "background-color: red;" || btn->styleSheet() == "background-color: white;") {
            return;
        }

        // Проверка попадания по кораблю игрока
        if (isHit(row, col, playerShips)) {
            btn->setStyleSheet("background-color: red;");
            removeShipAt(row, col, playerShips);

            if (isGameOver(playerShips)) {
                QMessageBox::information(this, "Поражение", "Вы проиграли!");
            }
        } else {
            btn->setStyleSheet("background-color: white;");
        }

        // Ход бота
        botTurn();
    }
}

bool GameWindow::isHit(int row, int col, QList<QList<QPoint>> &ships)
{
    for (auto &ship : ships) {
        for (auto &point : ship) {
            if (point.x() == row && point.y() == col) {
                return true;
            }
        }
    }
    return false;
}

void GameWindow::removeShipAt(int row, int col, QList<QList<QPoint>> &ships)
{
    for (auto &ship : ships) {
        for (int i = 0; i < ship.size(); ++i) {
            if (ship[i].x() == row && ship[i].y() == col) {
                ship.removeAt(i);
                return;
            }
        }
    }
}

bool GameWindow::isGameOver(QList<QList<QPoint>> &ships)
{
    for (auto &ship : ships) {
        if (!ship.isEmpty()) {
            return false;
        }
    }
    return true;
}

void GameWindow::botTurn()
{
    // Пример простого хода бота: случайный выстрел по полю игрока
    int row = rand() % 10;
    int col = rand() % 10;

    QPushButton *btn = playerGrid[row][col];
    if (btn->styleSheet() != "background-color: white;" && btn->styleSheet() != "background-color: red;") {
        // Если клетка не была еще выбрана, стреляем по ней
        if (isHit(row, col, playerShips)) {
            btn->setStyleSheet("background-color: red;");
            removeShipAt(row, col, playerShips);

            if (isGameOver(playerShips)) {
                QMessageBox::information(this, "Поражение", "Вы проиграли!");
            }
        } else {
            btn->setStyleSheet("background-color: white;");
        }
    } else {
        // Если клетка уже была выбрана, бот делает новый ход
        botTurn();
    }
}
