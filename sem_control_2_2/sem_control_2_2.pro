TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = clang++
QMAKE_CC = clang

CONFIG += c++14

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    dict.h \
    io_streams.h \
    task.h

