QT       += core gui widgets
QMAKE_CXXFLAGS += -Wno-implicit-function-declaration

CONFIG += c++17

SOURCES += \
    addform.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    addform.h \
    mainwindow.h \
    person.h

FORMS += \
    addform.ui \
    mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
