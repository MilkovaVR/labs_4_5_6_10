QT       += core gui widgets

QMAKE_CXXFLAGS += -Wno-implicit-function-declaration

TARGET = lab_10
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui