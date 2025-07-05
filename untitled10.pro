QT += core gui widgets testlib

CONFIG += c++17 testlib

SOURCES += \
    battlewindow.cpp \
    friendwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    secondwindow.cpp \
    tst_battle_window.cpp

HEADERS += \
    battlewindow.h \
    friendwindow.h \
    mainwindow.h \
    secondwindow.h

FORMS += \
    battlewindow.ui \
    friendwindow.ui \
    mainwindow.ui \
    secondwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
    