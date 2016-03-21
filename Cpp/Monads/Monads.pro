CONFIG += c++11
CONFIG -= Qt

TARGET = Monads
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    monad.cpp \
    control.cpp

HEADERS += \
    monad.h \
    control.h
