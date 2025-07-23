QT += core gui widgets network

CONFIG += c++17 warn_on release link_pkgconfig

TEMPLATE = app
TARGET = SeaBattle

linux {
    QMAKE_CXXFLAGS += -O3 -march=native -pipe -fPIC
    QMAKE_LFLAGS += -Wl,--as-needed
    LIBS += -lpthread
    PKGCONFIG += libnotify
}

SOURCES += \
    cpp/guidewindow.cpp \
    cpp/main.cpp \
    cpp/mainwindow.cpp \
    cpp/secondwindow.cpp \
    cpp/shipplacementwindow.cpp \
    cpp/battlewindow.cpp \
    cpp/multiplayermodewindow.cpp \
    cpp/hostsetupwindow.cpp \
    cpp/hostlobbywindow.cpp \
    cpp/connect_window.cpp \
    cpp/gameserver.cpp \
    cpp/gameclient.cpp

HEADERS += \
    headers/guidewindow.h \
    headers/mainwindow.h \
    headers/secondwindow.h \
    headers/shipplacementwindow.h \
    headers/battlewindow.h \
    headers/multiplayermodewindow.h \
    headers/hostsetupwindow.h \
    headers/hostlobbywindow.h \
    headers/connect_window.h \
    headers/gameserver.h \
    headers/gameclient.h

FORMS += \
    design/guidewindow.ui \
    design/mainwindow.ui \
    design/secondwindow.ui \
    design/shipplacementwindow.ui \
    design/battlewindow.ui \
    design/multiplayermodewindow.ui \
    design/hostsetupwindow.ui \
    design/hostlobbywindow.ui \
    design/connect_window.ui

RESOURCES += resources.qrc \  # если есть иконки и т.п.
    resources.qrc

# Установка для Linux
unix:!android {
    target.path = /usr/local/bin
    INSTALLS += target
}
