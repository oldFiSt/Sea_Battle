#include "battlewindow.h"
#include "ui_battlewindow.h"
#include "mainwindow.h"
#include "secondwindow.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>
#include <QSet>
#include <algorithm>

BattleWindow::BattleWindow(const QList<QList<QPoint>>& playerShipsGroups, MainWindow* mainWindow, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::BattleWindow),
    mainWindowPtr(mainWindow),
    playerGridLayout(nullptr),
    enemyGridLayout(nullptr),
    botTimer(nullptr),
    playerShipsAlive(0),
    enemyShipsAlive(0),
    playerTurn(true),
    botHuntingMode(false)
{
    ui->setupUi(this);

    playerGridLayout = new QGridLayout(ui->gridLayoutPlayer);
    enemyGridLayout = new QGridLayout(ui->gridLayoutEnemy);

    playerGridLayout->setContentsMargins(0, 0, 0, 0);
    playerGridLayout->setSpacing(1);
    enemyGridLayout->setContentsMargins(0, 0, 0, 0);
    enemyGridLayout->setSpacing(1);

    ui->btnReplay->setVisible(false);
    ui->btnExitToMainMenu->setVisible(false);

    connect(ui->btnReplay, &QPushButton::clicked, this, [=]() {
        this->close();
        SecondWindow* window = new SecondWindow();
        window->show();
    });

    connect(ui->btnExitToMainMenu, &QPushButton::clicked, this, &BattleWindow::on_btnExitToMainMenu_clicked);

    playerShipsGrouped = playerShipsGroups;
    playerShipsGroupedFOREVER = playerShipsGroups;

    for (const auto& ship : playerShipsGroups) {
        playerShips.append(ship);
    }

    setupFields();
    playerShipsAlive = playerShipsGroupedFOREVER.size();
    enemyShipsAlive = 10;

    ui->labelPlayerShips->setText("Ваши корабли: " + QString::number(playerShipsAlive));
    ui->labelEnemyShips->setText("Корабли противника: " + QString::number(enemyShipsAlive));

    placePlayerShips();
    placeEnemyShipsRandomly();

    botTimer = new QTimer(this);
    botTimer->setInterval(100);
    connect(botTimer, &QTimer::timeout, this, &BattleWindow::botShoot);
}

BattleWindow::~BattleWindow()
{
    delete ui;
}

void BattleWindow::on_btnExitToMainMenu_clicked()
{
    this->close();
    if (mainWindowPtr) {
        mainWindowPtr->show();
    }
}

void BattleWindow::setupFields()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            playerField[row][col] = new QPushButton(this);
            playerField[row][col]->setFixedSize(30, 30);
            playerField[row][col]->setEnabled(false);
            playerField[row][col]->setStyleSheet("background-color: lightblue;");
            playerGridLayout->addWidget(playerField[row][col], row, col);

            enemyField[row][col] = new QPushButton(this);
            enemyField[row][col]->setFixedSize(30, 30);
            enemyField[row][col]->setStyleSheet("background-color: lightblue;");
            enemyGridLayout->addWidget(enemyField[row][col], row, col);
            enemyField[row][col]->setEnabled(true);

            playerField[row][col]->setProperty("hasShip", false);

            connect(enemyField[row][col], &QPushButton::clicked, this, [=]() {
                if (!playerTurn || !enemyField[row][col]->isEnabled()) return;

                bool hasShip = enemyField[row][col]->property("hasShip").toBool();
                if (hasShip) {
                    enemyField[row][col]->setStyleSheet("background-color: red;");
                    enemyField[row][col]->setEnabled(false);
                    checkAndHandleDestroyedShip(QPoint(row, col));
                } else {
                    enemyField[row][col]->setStyleSheet("background-color: white;");
                    enemyField[row][col]->setEnabled(false);
                    playerTurn = false;
                    botTimer->start();
                }
            });
        }
    }
}

void BattleWindow::placePlayerShips()
{
    for (const auto& ship : playerShipsGroupedFOREVER) {
        for (const QPoint& pt : ship) {
            if (pt.x() >= 0 && pt.x() < 10 && pt.y() >= 0 && pt.y() < 10) {
                playerField[pt.x()][pt.y()]->setStyleSheet("background-color: green;");
                playerField[pt.x()][pt.y()]->setProperty("hasShip", true);
            }
        }
    }
}

void BattleWindow::placeEnemyShipsRandomly()
{
    int maxShips[5] = {0, 4, 3, 2, 1};

    auto tryPlaceShip = [&](int size) -> bool {
        int maxAttempts = (size == 1) ? 5000 : 1000;
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            bool vertical = QRandomGenerator::global()->bounded(2);
            int row = QRandomGenerator::global()->bounded(10);
            int col = QRandomGenerator::global()->bounded(10);
            QList<QPoint> newShip;

            for (int i = 0; i < size; ++i) {
                int r = vertical ? row + i : row;
                int c = vertical ? col : col + i;
                if (r >= 10 || c >= 10) break;
                newShip.append(QPoint(r, c));
            }

            if (newShip.size() != size) continue;

            if (isValidEnemyShipPlacement(newShip)) {
                enemyShipsGrouped.append(newShip);
                enemyShipsGroupedFOREVER.append(newShip);

                for (const QPoint& pt : newShip) {
                    enemyField[pt.x()][pt.y()]->setProperty("hasShip", true);
                }
                return true;
            }
        }
        return false;
    };

    for (int size = 4; size >= 1; --size) {
        for (int i = 0; i < maxShips[size]; ++i) {
            tryPlaceShip(size);
        }
    }

    qDebug() << "Всего кораблей противника:" << enemyShipsGrouped.size();
    for (const auto& ship : enemyShipsGroupedFOREVER) {
        qDebug() << "Корабль размером" << ship.size() << ":" << ship;
    }
}

bool BattleWindow::isValidEnemyShipPlacement(const QList<QPoint>& ship)
{
    if (ship.size() < 1) return false;

    QList<QPoint> sorted = ship;
    std::sort(sorted.begin(), sorted.end(), [](const QPoint &a, const QPoint &b) {
        return a.x() == b.x() ? a.y() < b.y() : a.x() < b.x();
    });

    bool isVertical = true, isHorizontal = true;
    int x0 = sorted[0].x();
    int y0 = sorted[0].y();

    for (const QPoint& pt : sorted) {
        if (pt.x() != x0) isHorizontal = false;
        if (pt.y() != y0) isVertical = false;
    }

    if (!isVertical && !isHorizontal) return false;

    for (int i = 1; i < sorted.size(); ++i) {
        int dx = std::abs(sorted[i].x() - sorted[i - 1].x());
        int dy = std::abs(sorted[i].y() - sorted[i - 1].y());
        if ((dx + dy) != 1) return false;
    }

    for (const QPoint& point : sorted) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = point.x() + dr;
                int nc = point.y() + dc;

                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    if (enemyField[nr][nc]->property("hasShip").toBool())
                        return false;
                }
            }
        }
    }

    return true;
}

void BattleWindow::checkAndHandleDestroyedShip(QPoint hitPoint)
{
    bool shipDestroyed = false;
    bool keepPlayerTurn = false;

    for (int i = 0; i < enemyShipsGrouped.size(); ++i) {
        QList<QPoint>& ship = enemyShipsGrouped[i];
        if (ship.contains(hitPoint)) {
            keepPlayerTurn = true;

            ship.removeOne(hitPoint);
            enemyField[hitPoint.x()][hitPoint.y()]->setStyleSheet("background-color: red;");
            enemyField[hitPoint.x()][hitPoint.y()]->setEnabled(false);

            if (ship.isEmpty()) {
                markSurroundingArea(enemyShipsGroupedFOREVER[i]);
                enemyShipsGrouped.removeAt(i);
                enemyShipsGroupedFOREVER.removeAt(i);
                enemyShipsAlive--;
                ui->labelEnemyShips->setText("Корабли противника: " + QString::number(enemyShipsAlive));
                shipDestroyed = true;
            }
            break;
        }
    }

    if (enemyShipsGrouped.isEmpty()) {
        QMessageBox::information(this, "Победа", "Вы уничтожили все корабли противника!");
        ui->btnReplay->setVisible(true);
        ui->btnExitToMainMenu->setVisible(true);
        return;
    }

    if (!keepPlayerTurn) {
        playerTurn = false;
        botTimer->start();
    }
}

void BattleWindow::markSurroundingArea(const QList<QPoint>& ship)
{
    QSet<QPoint> processed;

    for (const QPoint& pt : ship) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = pt.x() + dr;
                int nc = pt.y() + dc;

                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    QPoint neighbor(nr, nc);

                    if (!processed.contains(neighbor)) {
                        processed.insert(neighbor);

                        QPushButton* btn = enemyField[nr][nc];
                        bool isShip = btn->property("hasShip").toBool();

                        if (!isShip) {
                            btn->setStyleSheet("background-color: white;");
                            btn->setEnabled(false);
                            hitStatus[nr][nc] = false;
                        }
                    }
                }
            }
        }
    }
}

void BattleWindow::botShoot()
{
    QPoint target;

    if (botHuntingMode && !targetQueue.isEmpty()) {
        target = targetQueue.takeFirst();
    } else {
        for (int attempt = 0; attempt < 100; ++attempt) {
            int row = QRandomGenerator::global()->bounded(10);
            int col = QRandomGenerator::global()->bounded(10);
            QPushButton* cell = playerField[row][col];

            QString color = cell->styleSheet();
            if (color.contains("red") || color.contains("white")) continue;

            target = QPoint(row, col);
            break;
        }
    }

    QPushButton* cell = playerField[target.x()][target.y()];
    bool hit = cell->property("hasShip").toBool();

    if (hit) {
        cell->setStyleSheet("background-color: red;");
        cell->setEnabled(false);
        checkPlayerShipDestroyed(target);

        bool allShipsDestroyed = true;
        for (const auto& ship : playerShipsGrouped) {
            if (!ship.isEmpty()) {
                allShipsDestroyed = false;
                break;
            }
        }

        if (allShipsDestroyed) {
            QMessageBox::information(this, "Поражение", "Все ваши корабли уничтожены!");
            ui->btnReplay->setVisible(true);
            ui->btnExitToMainMenu->setVisible(true);
            return;
        }

        lastHitPoint = target;
        botHuntingMode = true;

        QVector<QPoint> directions = {
            QPoint(-1, 0), QPoint(1, 0), QPoint(0, -1), QPoint(0, 1)
    };

    for (const QPoint& dir : directions) {
        int nr = lastHitPoint.x() + dir.x();
        int nc = lastHitPoint.y() + dir.y();

        if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
            QPushButton* neighbor = playerField[nr][nc];
            QString color = neighbor->styleSheet();
            if (!color.contains("red") && !color.contains("white")) {
                targetQueue.append(QPoint(nr, nc));
            }
        }
    }

} else {
    cell->setStyleSheet("background-color: white;");
    cell->setEnabled(false);
}

if (targetQueue.isEmpty()) {
    botHuntingMode = false;
}

playerTurn = true;
botTimer->stop();
}

void BattleWindow::checkPlayerShipDestroyed(QPoint hitPoint)
{
    for (int i = 0; i < playerShipsGrouped.size(); ++i) {
        QList<QPoint>& ship = playerShipsGrouped[i];
        if (ship.contains(hitPoint)) {
            ship.removeOne(hitPoint);

            if (ship.isEmpty()) {
                markPlayerDestroyedShip(playerShipsGroupedFOREVER[i]);
                playerShipsGrouped.removeAt(i);
                playerShipsGroupedFOREVER.removeAt(i);
                playerShipsAlive--;
                ui->labelPlayerShips->setText("Ваши корабли: " + QString::number(playerShipsAlive));
            }
            break;
        }
    }
}

void BattleWindow::markPlayerDestroyedShip(const QList<QPoint>& ship)
{
    QSet<QPoint> processed;

    for (const QPoint& pt : ship) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = pt.x() + dr;
                int nc = pt.y() + dc;

                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    QPoint neighbor(nr, nc);

                    if (!processed.contains(neighbor)) {
                        processed.insert(neighbor);

                        QPushButton* btn = playerField[nr][nc];
                        bool isShip = btn->property("hasShip").toBool();

                        if (!isShip) {
                            btn->setStyleSheet("background-color: white;");
                            btn->setEnabled(false);
                        }
                    }
                }
            }
        }
    }
}
