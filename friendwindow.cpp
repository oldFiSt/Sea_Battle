#include "friendwindow.h"
#include "ui_friendwindow.h"

FriendWindow::FriendWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendWindow),
    player1Turn(true),
    player1ShipsAlive(10),
    player2ShipsAlive(10)
{
    setFixedSize(900, 700);
    ui->setupUi(this);

    player1GridLayout = new QGridLayout(ui->gridLayoutPlayer1);
    player2GridLayout = new QGridLayout(ui->gridLayoutPlayer2);

    player1GridLayout->setContentsMargins(0, 0, 0, 0);
    player1GridLayout->setSpacing(1);
    player2GridLayout->setContentsMargins(0, 0, 0, 0);
    player2GridLayout->setSpacing(1);

    setupFields();
    resetGame();

    connect(ui->btnRestart, &QPushButton::clicked, this, &FriendWindow::restartGame);
}

FriendWindow::~FriendWindow()
{
    delete ui;
}

void FriendWindow::setupFields()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            // Player 1 field
            player1Field[row][col] = new QPushButton(this);
            player1Field[row][col]->setFixedSize(30, 30);
            player1Field[row][col]->setStyleSheet("background-color: lightblue;");
            player1GridLayout->addWidget(player1Field[row][col], row, col);
            player1Field[row][col]->setProperty("hasShip", false);
            player1Field[row][col]->setEnabled(false); // Disabled by default

            connect(player1Field[row][col], &QPushButton::clicked, this, &FriendWindow::handlePlayer1CellClick);
            player1Field[row][col]->setProperty("row", row);
            player1Field[row][col]->setProperty("col", col);

            // Player 2 field
            player2Field[row][col] = new QPushButton(this);
            player2Field[row][col]->setFixedSize(30, 30);
            player2Field[row][col]->setStyleSheet("background-color: lightblue;");
            player2GridLayout->addWidget(player2Field[row][col], row, col);
            player2Field[row][col]->setProperty("hasShip", false);
            player2Field[row][col]->setEnabled(false); // Disabled by default

            connect(player2Field[row][col], &QPushButton::clicked, this, &FriendWindow::handlePlayer2CellClick);
            player2Field[row][col]->setProperty("row", row);
            player2Field[row][col]->setProperty("col", col);
        }
    }
}

void FriendWindow::restartGame()
{
    resetGame();
    ui->labelTurnInfo->setText("Ход Игрока 1");
    ui->btnRestart->setVisible(false);
    updateShipsCount();
}

void FriendWindow::resetGame()
{
    // Reset fields
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            player1Field[row][col]->setStyleSheet("background-color: lightblue;");
            player1Field[row][col]->setEnabled(false);
            player1Field[row][col]->setProperty("hasShip", false);

            player2Field[row][col]->setStyleSheet("background-color: lightblue;");
            player2Field[row][col]->setEnabled(false);
            player2Field[row][col]->setProperty("hasShip", false);
        }
    }

    // Reset game state
    player1Turn = true;
    player1ShipsAlive = 10;
    player2ShipsAlive = 10;

    // Clear ship lists
    player1Ships.clear();
    player1ShipsForever.clear();
    player2Ships.clear();
    player2ShipsForever.clear();

    // Place ships randomly
    placeShipsRandomly(player1Field, player1Ships, player1ShipsForever);
    placeShipsRandomly(player2Field, player2Ships, player2ShipsForever);

    // Enable appropriate field
    player2Field[0][0]->setEnabled(true); // Start with player 1's turn
}

void FriendWindow::placeShipsRandomly(QPushButton* field[10][10],
                                      QList<QList<QPoint>>& shipsGrouped,
                                      QList<QList<QPoint>>& shipsForever)
{
    int maxShips[5] = {0, 4, 3, 2, 1};
    auto tryPlaceShip = [&](int size) -> bool {
        int maxAttempts = 1000;
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

            // Check if valid placement
            bool valid = true;
            for (const QPoint& point : newShip) {
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nr = point.x() + dr;
                        int nc = point.y() + dc;
                        if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                            if (field[nr][nc]->property("hasShip").toBool()) {
                                valid = false;
                                break;
                            }
                        }
                    }
                    if (!valid) break;
                }
                if (!valid) break;
            }

            if (!valid) continue;

            // Place the ship
            shipsGrouped.append(newShip);
            shipsForever.append(newShip);
            for (const QPoint& pt : newShip) {
                field[pt.x()][pt.y()]->setProperty("hasShip", true);
                field[pt.x()][pt.y()]->setStyleSheet("background-color: green;");
            }
            return true;
        }
        return false;
    };

    for (int size = 4; size >= 1; --size) {
        for (int i = 0; i < maxShips[size]; ++i) {
            tryPlaceShip(size);
        }
    }
}

void FriendWindow::handlePlayer1CellClick()
{
    if (!player1Turn) return;

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    handlePlayerMove(row, col, player2Field, player2Ships, player2ShipsForever, player2ShipsAlive, ui->labelPlayer2Ships);
}

void FriendWindow::handlePlayer2CellClick()
{
    if (player1Turn) return;

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    handlePlayerMove(row, col, player1Field, player1Ships, player1ShipsForever, player1ShipsAlive, ui->labelPlayer1Ships);
}

void FriendWindow::handlePlayerMove(int row, int col, QPushButton* targetField[10][10],
                                    QList<QList<QPoint>>& targetShipsGrouped,
                                    QList<QList<QPoint>>& targetShipsForever,
                                    int& targetShipsAlive,
                                    QLabel* label)
{
    QPushButton* cell = targetField[row][col];
    if (!cell->isEnabled()) return; // Already clicked

    bool hasShip = cell->property("hasShip").toBool();
    if (hasShip) {
        cell->setStyleSheet("background-color: red;");
        cell->setEnabled(false);
        checkAndHandleDestroyedShip(QPoint(row, col), targetShipsGrouped, targetShipsForever, targetShipsAlive, targetField, label);
    } else {
        cell->setStyleSheet("background-color: white;");
        cell->setEnabled(false);
        switchTurn();
    }

    checkGameOver();
}

void FriendWindow::checkAndHandleDestroyedShip(QPoint hitPoint,
                                               QList<QList<QPoint>>& targetShipsGrouped,
                                               QList<QList<QPoint>>& targetShipsForever,
                                               int& targetShipsAlive,
                                               QPushButton* targetField[10][10],
                                               QLabel* label)
{
    bool shipDestroyed = false;

    for (int i = 0; i < targetShipsGrouped.size(); ++i) {
        QList<QPoint>& ship = targetShipsGrouped[i];
        if (ship.contains(hitPoint)) {
            ship.removeOne(hitPoint);

            if (ship.isEmpty()) {
                // Find the original ship in forever list
                for (int j = 0; j < targetShipsForever.size(); ++j) {
                    if (targetShipsForever[j].contains(hitPoint)) {
                        markSurroundingArea(targetShipsForever[j], targetField);
                        targetShipsForever.removeAt(j);
                        break;
                    }
                }

                targetShipsGrouped.removeAt(i);
                targetShipsAlive--;
                label->setText("Корабли: " + QString::number(targetShipsAlive));
                shipDestroyed = true;
            }
            break;
        }
    }

    // Only switch turn if no ship was destroyed
    if (!shipDestroyed) {
        // Player gets another turn when hitting a ship
        return;
    }

    // Check if all ships are destroyed
    if (targetShipsAlive == 0) {
        return;
    }

    // Switch turn after destroying a ship
    switchTurn();
}

void FriendWindow::markSurroundingArea(const QList<QPoint>& ship, QPushButton* field[10][10])
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

                        QPushButton* btn = field[nr][nc];
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

void FriendWindow::switchTurn()
{
    player1Turn = !player1Turn;

    // Disable both fields first
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            player1Field[row][col]->setEnabled(false);
            player2Field[row][col]->setEnabled(false);
        }
    }

    // Enable the field for the current player
    if (player1Turn) {
        ui->labelTurnInfo->setText("Ход Игрока 1");
        // Player 1 shoots at player 2's field
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (player2Field[row][col]->isEnabled()) {
                    player2Field[row][col]->setEnabled(true);
                }
            }
        }
    } else {
        ui->labelTurnInfo->setText("Ход Игрока 2");
        // Player 2 shoots at player 1's field
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (player1Field[row][col]->isEnabled()) {
                    player1Field[row][col]->setEnabled(true);
                }
            }
        }
    }
}

void FriendWindow::updateShipsCount()
{
    ui->labelPlayer1Ships->setText("Корабли: " + QString::number(player1ShipsAlive));
    ui->labelPlayer2Ships->setText("Корабли: " + QString::number(player2ShipsAlive));
}

void FriendWindow::checkGameOver()
{
    if (player1ShipsAlive == 0) {
        QMessageBox::information(this, "Победа", "Игрок 2 победил!");
        ui->btnRestart->setVisible(true);
        return;
    }

    if (player2ShipsAlive == 0) {
        QMessageBox::information(this, "Победа", "Игрок 1 победил!");
        ui->btnRestart->setVisible(true);
        return;
    }
}
