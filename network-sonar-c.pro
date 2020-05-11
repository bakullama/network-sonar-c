TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -pthread


SOURCES += \
        main.c

LIBS += -lgraph -lX11 -lxcb -pthread


