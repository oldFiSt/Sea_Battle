QT += core gui widgets network

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    secondwindow.cpp \
    shipplacementwindow.cpp \
    battlewindow.cpp \
    multiplayermodewindow.cpp \
    hostsetupwindow.cpp \
    hostlobbywindow.cpp \
    connect_window.cpp \
    gameserver.cpp \
    gameclient.cpp

HEADERS += \
    mainwindow.h \
    secondwindow.h \
    shipplacementwindow.h \
    battlewindow.h \
    multiplayermodewindow.h \
    hostsetupwindow.h \
    hostlobbywindow.h \
    connect_window.h \
    gameserver.h \
    gameclient.h

FORMS += \
    mainwindow.ui \
    secondwindow.ui \
    shipplacementwindow.ui \
    battlewindow.ui \
    multiplayermodewindow.ui \
    hostsetupwindow.ui \
    hostlobbywindow.ui \
    connect_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
