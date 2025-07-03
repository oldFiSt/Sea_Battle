/********************************************************************************
** Form generated from reading UI file 'secondwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDWINDOW_H
#define UI_SECONDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SecondWindow
{
public:
    QLabel *label;
    QGroupBox *groupBox;
    QPushButton *btnShip1;
    QPushButton *btnShip2;
    QPushButton *btnShip3;
    QPushButton *btnShip4;
    QLabel *labelShip1Count;
    QLabel *labelShip2Count;
    QLabel *labelShip3Count;
    QLabel *labelShip4Count;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayoutField;
    QPushButton *btnStartBattle;

    void setupUi(QDialog *SecondWindow)
    {
        if (SecondWindow->objectName().isEmpty())
            SecondWindow->setObjectName("SecondWindow");
        SecondWindow->resize(1000, 600);
        label = new QLabel(SecondWindow);
        label->setObjectName("label");
        label->setGeometry(QRect(340, 0, 361, 111));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font.setPointSize(22);
        font.setBold(true);
        label->setFont(font);
        groupBox = new QGroupBox(SecondWindow);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(50, 190, 181, 121));
        btnShip1 = new QPushButton(groupBox);
        btnShip1->setObjectName("btnShip1");
        btnShip1->setGeometry(QRect(10, 50, 75, 24));
        btnShip2 = new QPushButton(groupBox);
        btnShip2->setObjectName("btnShip2");
        btnShip2->setGeometry(QRect(90, 50, 75, 24));
        btnShip3 = new QPushButton(groupBox);
        btnShip3->setObjectName("btnShip3");
        btnShip3->setGeometry(QRect(10, 80, 75, 24));
        btnShip4 = new QPushButton(groupBox);
        btnShip4->setObjectName("btnShip4");
        btnShip4->setGeometry(QRect(90, 80, 75, 24));
        labelShip1Count = new QLabel(SecondWindow);
        labelShip1Count->setObjectName("labelShip1Count");
        labelShip1Count->setGeometry(QRect(40, 370, 200, 30));
        labelShip2Count = new QLabel(SecondWindow);
        labelShip2Count->setObjectName("labelShip2Count");
        labelShip2Count->setGeometry(QRect(40, 410, 200, 30));
        labelShip3Count = new QLabel(SecondWindow);
        labelShip3Count->setObjectName("labelShip3Count");
        labelShip3Count->setGeometry(QRect(40, 450, 200, 30));
        labelShip4Count = new QLabel(SecondWindow);
        labelShip4Count->setObjectName("labelShip4Count");
        labelShip4Count->setGeometry(QRect(40, 490, 200, 30));
        gridLayoutWidget = new QWidget(SecondWindow);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(420, 160, 461, 381));
        gridLayoutField = new QGridLayout(gridLayoutWidget);
        gridLayoutField->setObjectName("gridLayoutField");
        gridLayoutField->setContentsMargins(0, 0, 0, 0);
        btnStartBattle = new QPushButton(SecondWindow);
        btnStartBattle->setObjectName("btnStartBattle");
        btnStartBattle->setGeometry(QRect(200, 450, 161, 101));
        QFont font1;
        font1.setPointSize(14);
        btnStartBattle->setFont(font1);

        retranslateUi(SecondWindow);

        QMetaObject::connectSlotsByName(SecondWindow);
    } // setupUi

    void retranslateUi(QDialog *SecondWindow)
    {
        SecondWindow->setWindowTitle(QCoreApplication::translate("SecondWindow", "\320\222\321\213\320\261\320\276\321\200 \320\272\320\276\321\200\320\260\320\261\320\273\320\265\320\271", nullptr));
        label->setText(QCoreApplication::translate("SecondWindow", "\320\221\320\276\320\271 \320\277\321\200\320\276\321\202\320\270\320\262 \320\261\320\276\321\202\320\260", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SecondWindow", "\320\232\320\276\321\200\320\260\320\261\320\273\320\270", nullptr));
        btnShip1->setText(QCoreApplication::translate("SecondWindow", "1-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\271", nullptr));
        btnShip2->setText(QCoreApplication::translate("SecondWindow", "2-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\271", nullptr));
        btnShip3->setText(QCoreApplication::translate("SecondWindow", "3-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\271", nullptr));
        btnShip4->setText(QCoreApplication::translate("SecondWindow", "4-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\271", nullptr));
        labelShip1Count->setText(QCoreApplication::translate("SecondWindow", "1-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\265: 0/4", nullptr));
        labelShip2Count->setText(QCoreApplication::translate("SecondWindow", "2-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\265: 0/3", nullptr));
        labelShip3Count->setText(QCoreApplication::translate("SecondWindow", "3-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\265: 0/2", nullptr));
        labelShip4Count->setText(QCoreApplication::translate("SecondWindow", "4-\320\277\320\260\320\273\321\203\320\261\320\275\321\213\320\265: 0/1", nullptr));
        btnStartBattle->setText(QCoreApplication::translate("SecondWindow", "\320\222 \320\261\320\276\320\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecondWindow: public Ui_SecondWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDWINDOW_H
