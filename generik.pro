QT += core
QT += multimedia
QT += network
QT += serialport
QT -= gui

CONFIG += c++11

TARGET = generik
CONFIG += console
CONFIG -= app_bundle
CONFIG += no_keywords

TEMPLATE = app

SOURCES += main.cpp \
    application.cpp \
    audiocontroller.cpp \
    server.cpp \
    jeu.cpp \
    iplugins.cpp \
    clientIO.cpp \
    serialport.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    application.h \
    audiocontroller.h \
    server.h \
    iplugins.h \
    jeu.h \
    clientIO.h \
    serialport.h

#INCLUDEPATH += $$PWD/socket.io-client-cpp/lib/rapidjson/include
#INCLUDEPATH += $$PWD/socket.io-client-cpp/lib/websocketpp/websocketpp
#INCLUDEPATH += $$PWD/socket.io-client-cpp/src
#INCLUDEPATH += $$PWD/socket.io-client-cpp/src/internal

INCLUDEPATH += $$PWD/socket.io-client-cpp/build/include
DEPENDPATH += $$PWD/socket.io-client-cpp/build/lib

LIBS += -L/usr/local/lib -lwiringPi

LIBS += -L$$PWD/socket.io-client-cpp/build/lib/Release/ -lsioclient
LIBS += -L$$PWD/socket.io-client-cpp/build/lib/Release/ -lboost_random
LIBS += -L$$PWD/socket.io-client-cpp/build/lib/Release/ -lboost_system
LIBS += -L$$PWD/socket.io-client-cpp/build/lib/Release/ -lboost_date_time


INCLUDEPATH += /home/pi/boost_1_69_0/
DISTFILES += \
    ../build-generik-Desktop-Debug/test.json \
    GameTech.json \
    GameFonct.json


