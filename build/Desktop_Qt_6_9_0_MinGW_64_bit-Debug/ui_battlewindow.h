/********************************************************************************
** Form generated from reading UI file 'battlewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATTLEWINDOW_H
#define UI_BATTLEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BattleWindow
{
public:
    QLabel *labelPlayer;
    QLabel *labelEnemy;
    QWidget *gridLayoutPlayer;
    QWidget *gridLayoutEnemy;
    QPushButton *btnReplay;
    QLabel *labelPlayerShips;
    QLabel *labelEnemyShips;

    void setupUi(QDialog *BattleWindow)
    {
        if (BattleWindow->objectName().isEmpty())
            BattleWindow->setObjectName("BattleWindow");
        BattleWindow->resize(843, 682);
        labelPlayer = new QLabel(BattleWindow);
        labelPlayer->setObjectName("labelPlayer");
        labelPlayer->setGeometry(QRect(40, 20, 300, 50));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font.setPointSize(22);
        font.setBold(true);
        labelPlayer->setFont(font);
        labelEnemy = new QLabel(BattleWindow);
        labelEnemy->setObjectName("labelEnemy");
        labelEnemy->setGeometry(QRect(460, 20, 350, 50));
        labelEnemy->setFont(font);
        gridLayoutPlayer = new QWidget(BattleWindow);
        gridLayoutPlayer->setObjectName("gridLayoutPlayer");
        gridLayoutPlayer->setGeometry(QRect(40, 80, 350, 350));
        gridLayoutEnemy = new QWidget(BattleWindow);
        gridLayoutEnemy->setObjectName("gridLayoutEnemy");
        gridLayoutEnemy->setGeometry(QRect(460, 80, 350, 350));
        btnReplay = new QPushButton(BattleWindow);
        btnReplay->setObjectName("btnReplay");
        btnReplay->setGeometry(QRect(330, 500, 180, 40));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        btnReplay->setFont(font1);
        btnReplay->setVisible(false);
        labelPlayerShips = new QLabel(BattleWindow);
        labelPlayerShips->setObjectName("labelPlayerShips");
        labelPlayerShips->setGeometry(QRect(40, 450, 350, 30));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font2.setPointSize(12);
        font2.setBold(true);
        labelPlayerShips->setFont(font2);
        labelEnemyShips = new QLabel(BattleWindow);
        labelEnemyShips->setObjectName("labelEnemyShips");
        labelEnemyShips->setGeometry(QRect(460, 450, 350, 30));
        labelEnemyShips->setFont(font2);

        retranslateUi(BattleWindow);

        QMetaObject::connectSlotsByName(BattleWindow);
    } // setupUi

    void retranslateUi(QDialog *BattleWindow)
    {
        BattleWindow->setWindowTitle(QCoreApplication::translate("BattleWindow", "\320\234\320\276\321\200\321\201\320\272\320\276\320\271 \320\261\320\276\320\271", nullptr));
        labelPlayer->setText(QCoreApplication::translate("BattleWindow", "\320\222\320\260\321\210\320\265 \320\277\320\276\320\273\320\265", nullptr));
        labelEnemy->setText(QCoreApplication::translate("BattleWindow", "\320\237\320\276\320\273\320\265 \320\277\321\200\320\276\321\202\320\270\320\262\320\275\320\270\320\272\320\260", nullptr));
        btnReplay->setText(QCoreApplication::translate("BattleWindow", "\320\241\321\213\320\263\321\200\320\260\321\202\321\214 \321\201\320\275\320\276\320\262\320\260", nullptr));
        labelPlayerShips->setText(QCoreApplication::translate("BattleWindow", "\320\222\320\260\321\210\320\270 \320\272\320\276\321\200\320\260\320\261\320\273\320\270: 10", nullptr));
        labelEnemyShips->setText(QCoreApplication::translate("BattleWindow", "\320\232\320\276\321\200\320\260\320\261\320\273\320\270 \320\277\321\200\320\276\321\202\320\270\320\262\320\275\320\270\320\272\320\260: 10", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BattleWindow: public Ui_BattleWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATTLEWINDOW_H
