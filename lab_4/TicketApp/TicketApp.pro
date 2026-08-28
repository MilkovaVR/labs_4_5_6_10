QT += widgets
CONFIG += c++17

QMAKE_CXXFLAGS += -Wno-implicit-function-declaration

TARGET = TicketApp
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp ticket.cpp
HEADERS += mainwindow.h ticket.h
FORMS += mainwindow.ui