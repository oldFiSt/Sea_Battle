#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "battlewindow.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <algorithm>
#include <QRandomGenerator>

SecondWindow::SecondWindow(MainWindow* parentMainWindow)
    : QDialog(nullptr),
    ui(new Ui::SecondWindow),
    m_mainWindow(parentMainWindow),
    currentShipSize(0),
    currentOrientation(UNKNOWN)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(944, 697);
    ui->setupUi(this);

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            gridButtons[row][col] = new QPushButton(this);
            gridButtons[row][col]->setStyleSheet("background-color: lightblue;");
            gridButtons[row][col]->setProperty("row", row);
            gridButtons[row][col]->setProperty("col", col);
            gridButtons[row][col]->setProperty("occupied", false);

            connect(gridButtons[row][col], &QPushButton::clicked, this, &SecondWindow::handleCellClick);
            ui->gridLayoutField->addWidget(gridButtons[row][col], row, col);
        }
    }

    connect(ui->btnShip1, &QPushButton::clicked, this, &SecondWindow::selectShip1);
    connect(ui->btnShip2, &QPushButton::clicked, this, &SecondWindow::selectShip2);
    connect(ui->btnShip3, &QPushButton::clicked, this, &SecondWindow::selectShip3);
    connect(ui->btnShip4, &QPushButton::clicked, this, &SecondWindow::selectShip4);
    connect(ui->btnStartBattle, &QPushButton::clicked, this, &SecondWindow::startBattle);
    connect(ui->btnAutoPlaceShips, &QPushButton::clicked, this, &SecondWindow::onAutoPlaceShipsClicked);
    connect(ui->btnResetShips, &QPushButton::clicked, this, &SecondWindow::onResetShipsClicked);
    connect(ui->btnBackToMainMenu, &QPushButton::clicked, this, &SecondWindow::returnToMainMenu);

    initializeShipCounts();
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::clearBoard()
{
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            gridButtons[row][col]->setStyleSheet("background-color: lightblue;");
            gridButtons[row][col]->setProperty("occupied", false);
        }
    }
    allPlacedShips.clear();
    resetCurrentPlacement();
    currentShipSize = 0;
    initializeShipCounts();
}

void SecondWindow::onResetShipsClicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Сброс кораблей");
    msgBox.setText("Вы уверены, что хотите удалить все расставленные корабли и начать заново?");
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesButton = msgBox.addButton(tr("Да"), QMessageBox::ActionRole);
    QPushButton *noButton = msgBox.addButton(tr("Нет"), QMessageBox::ActionRole);
    msgBox.setDefaultButton(noButton);
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        clearBoard();
    }
}

void SecondWindow::onAutoPlaceShipsClicked()
{
    clearBoard();
    if (autoPlaceAllShips()) {
        updateShipCountDisplay();
        QMessageBox::information(this, "Готово", "Все корабли расставлены автоматически.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось автоматически расставить корабли. Пожалуйста, сбросьте и попробуйте снова.");
        clearBoard();
    }
}

void SecondWindow::initializeShipCounts()
{
    shipsPlaced[1] = 0;
    shipsPlaced[2] = 0;
    shipsPlaced[3] = 0;
    shipsPlaced[4] = 0;
    updateShipCountDisplay();
}

void SecondWindow::selectShip(int size)
{
    if (!currentPlacement.isEmpty()) {
        resetCurrentPlacement();
    }
    currentShipSize = size;
    currentOrientation = UNKNOWN;

    if (shipsPlaced[currentShipSize] >= maxShips[currentShipSize]) {
        QMessageBox::information(this, "Лимит достигнут", QString("Вы уже разместили все %1-палубные корабли. Выберите другой тип.").arg(size));
        currentShipSize = 0;
        return;
    }

    QMessageBox::information(this, "Выбор корабля", QString("Вы выбрали %1-палубный корабль. Разместите его на поле.").arg(size));
}

void SecondWindow::selectShip1() { selectShip(1); }
void SecondWindow::selectShip2() { selectShip(2); }
void SecondWindow::selectShip3() { selectShip(3); }
void SecondWindow::selectShip4() { selectShip(4); }

void SecondWindow::handleCellClick()
{
    if (currentShipSize == 0) {
        QMessageBox::warning(this, "Ошибка", "Сначала выберите тип корабля из панели слева.");
        return;
    }

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn || btn->property("occupied").toBool()) {
        return;
    }

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();
    QPoint newPoint(row, col);

    if (currentPlacement.contains(newPoint)) {
        return;
    }

    if (currentPlacement.isEmpty()) {
        // Первый клик, ничего особенного не делаем
    }
    else if (currentPlacement.size() == 1) {
        // Второй клик, определяем ориентацию
        QPoint firstPoint = currentPlacement.first();
        if (firstPoint.x() == newPoint.x() && std::abs(firstPoint.y() - newPoint.y()) == 1) {
            currentOrientation = HORIZONTAL;
        } else if (firstPoint.y() == newPoint.y() && std::abs(firstPoint.x() - newPoint.x()) == 1) {
            currentOrientation = VERTICAL;
        } else {
            QMessageBox::warning(this, "Ошибка", "Корабль должен быть прямой линией. Кликните рядом с первой палубой.");
            return;
        }
    }
    else {
        // Третий и последующие клики
        QPoint startPoint = currentPlacement.first();
        QPoint endPoint = currentPlacement.last();
        bool isAdjacentToEnd = false;

        if (currentOrientation == HORIZONTAL) {
            if (newPoint.x() == startPoint.x() &&
                (std::abs(newPoint.y() - startPoint.y()) == 1 || std::abs(newPoint.y() - endPoint.y()) == 1)) {
                isAdjacentToEnd = true;
            }
        } else { // VERTICAL
            // ========================= ИСПРАВЛЕНИЕ ЗДЕСЬ =========================
            // Было: || std::abs(newPoint.x() - startPoint.x()) == 1
            // Стало: || std::abs(newPoint.x() - endPoint.x()) == 1
            if (newPoint.y() == startPoint.y() &&
                (std::abs(newPoint.x() - startPoint.x()) == 1 || std::abs(newPoint.x() - endPoint.x()) == 1)) {
                isAdjacentToEnd = true;
            }
            // ===================================================================
        }

        if (!isAdjacentToEnd) {
            QMessageBox::warning(this, "Ошибка", "Корабль должен быть прямой линией. Продолжайте размещение от одного из концов.");
            return;
        }
    }

    currentPlacement.append(newPoint);
    btn->setStyleSheet("background-color: gray;");

    // Сортируем точки, чтобы startPoint и endPoint всегда были корректными
    std::sort(currentPlacement.begin(), currentPlacement.end(), [](const QPoint &a, const QPoint &b) {
        return a.x() == b.x() ? a.y() < b.y() : a.x() < b.x();
    });

    if (currentPlacement.size() == currentShipSize) {
        if (isAdjacentToAnotherShip(currentPlacement)) {
            QMessageBox::warning(this, "Ошибка", "Корабли не могут касаться друг друга.");
            resetCurrentPlacement();
            return;
        }

        for(const QPoint& point : currentPlacement) {
            gridButtons[point.x()][point.y()]->setProperty("occupied", true);
        }

        shipsPlaced[currentShipSize]++;
        allPlacedShips.append(currentPlacement);
        updateShipCountDisplay();

        currentPlacement.clear();
        currentOrientation = UNKNOWN;

        if (shipsPlaced[currentShipSize] >= maxShips[currentShipSize]) {
            QMessageBox::information(this, "Лимит достигнут", QString("Вы разместили все %1-палубные корабли. Выберите другой тип.").arg(currentShipSize));
            currentShipSize = 0;
        } else {
            int remaining = maxShips[currentShipSize] - shipsPlaced[currentShipSize];
            QMessageBox::information(this, "Корабль размещен", QString("Отлично! Можете размещать следующий. Осталось: %1").arg(remaining));
        }
    }
}


void SecondWindow::resetCurrentPlacement()
{
    for (const QPoint& point : currentPlacement) {
        if (!gridButtons[point.x()][point.y()]->property("occupied").toBool()) {
            gridButtons[point.x()][point.y()]->setStyleSheet("background-color: lightblue;");
        }
    }
    currentPlacement.clear();
    currentOrientation = UNKNOWN;
}

void SecondWindow::updateShipCountDisplay()
{
    ui->labelShip1Count->setText(QString("1-палубные: %1/%2").arg(shipsPlaced[1]).arg(maxShips[1]));
    ui->labelShip2Count->setText(QString("2-палубные: %1/%2").arg(shipsPlaced[2]).arg(maxShips[2]));
    ui->labelShip3Count->setText(QString("3-палубные: %1/%2").arg(shipsPlaced[3]).arg(maxShips[3]));
    ui->labelShip4Count->setText(QString("4-палубные: %1/%2").arg(shipsPlaced[4]).arg(maxShips[4]));
}

bool SecondWindow::isValidPlacementForAuto(const QList<QPoint>& ship)
{
    for (const QPoint& point : ship) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = point.x() + dr;
                int nc = point.y() + dc;
                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    if (gridButtons[nr][nc]->property("occupied").toBool()) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool SecondWindow::autoPlaceAllShips()
{
    const int shipSizes[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    const int maxPlacementTries = 100;

    for (int size : shipSizes) {
        bool placed = false;
        for (int tries = 0; tries < maxPlacementTries; ++tries) {
            bool isVertical = QRandomGenerator::global()->bounded(2);
            int row = QRandomGenerator::global()->bounded(10);
            int col = QRandomGenerator::global()->bounded(10);

            QList<QPoint> newShip;
            bool fitsOnBoard = true;
            for (int i = 0; i < size; ++i) {
                int r = isVertical ? row + i : row;
                int c = isVertical ? col : col + i;
                if (r >= 10 || c >= 10) {
                    fitsOnBoard = false;
                    break;
                }
                newShip.append(QPoint(r, c));
            }

            if (fitsOnBoard && isValidPlacementForAuto(newShip)) {
                allPlacedShips.append(newShip);
                shipsPlaced[size]++;
                for (const QPoint& pt : newShip) {
                    gridButtons[pt.x()][pt.y()]->setProperty("occupied", true);
                    gridButtons[pt.x()][pt.y()]->setStyleSheet("background-color: gray;");
                }
                placed = true;
                break;
            }
        }
        if (!placed) {
            return false;
        }
    }
    return true;
}

bool SecondWindow::isAdjacentToAnotherShip(const QList<QPoint>& ship)
{
    for (const QPoint& point : ship) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;

                int newRow = point.x() + i;
                int newCol = point.y() + j;

                if (newRow >= 0 && newRow < 10 && newCol >= 0 && newCol < 10) {
                    if (gridButtons[newRow][newCol]->property("occupied").toBool() && !ship.contains(QPoint(newRow, newCol))) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void SecondWindow::startBattle()
{
    if (shipsPlaced[1] != 4 || shipsPlaced[2] != 3 || shipsPlaced[3] != 2 || shipsPlaced[4] != 1) {
        QMessageBox::warning(this, "Ошибка", "Сначала расставьте все корабли!");
        return;
    }

    // Используем архитектуру сигнал-слот, которую мы обсуждали ранее
    emit shipsPlacedAndReady(allPlacedShips);
    this->close();
}

void SecondWindow::returnToMainMenu()
{
    this->close();
    if (m_mainWindow) {
        m_mainWindow->show();
    }
}
