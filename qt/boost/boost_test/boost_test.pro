TEMPLATE = app
CONFIG += console c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openfile
TEMPLATE = app

INCLUDEPATH += G:\c++\boost_1_62_0
LIBS += -LG:\c++\boost_1_62_0\lib64-msvc-14.0

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp
