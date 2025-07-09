QT += core gui widgets testlib
QT += network

CONFIG += c++17 testlib

SOURCES += \
    battlewindow.cpp \
    connect_window.cpp \
    friendwindow.cpp \
    gameclient.cpp \
    gameserver.cpp \
    main.cpp \
    mainwindow.cpp \
    secondwindow.cpp \
    tst_battle_window.cpp

HEADERS += \
    battlewindow.h \
    connect_window.h \
    friendwindow.h \
    gameclient.h \
    gameserver.h \
    mainwindow.h \
    secondwindow.h

FORMS += \
    battlewindow.ui \
    connect_window.ui \
    friendwindow.ui \
    mainwindow.ui \
    secondwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
    
