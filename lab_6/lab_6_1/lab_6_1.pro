QT       += core gui widgets

QMAKE_CXXFLAGS += -Wno-implicit-function-declaration

TARGET = lab_6_1
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui