######################################################################
# Automatically generated by qmake (3.1) Sat Oct 26 13:12:54 2024
######################################################################

TEMPLATE = app
TARGET = tetris
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += ../common/common.h \
           tetris.h \
           figures.h \
           controller.h \
           fsm.h \
           ../../gui/desktop/gamewidget.h \
           ../../gui/desktop/infowidget.h\
           ../../gui/desktop/mainwindow.h
SOURCES += ../common/common.c \
           tetris.c \
           figures.c \
           controller.c \
           fsm.c \
           ../../gui/desktop/gamewidget.cpp \
           ../../gui/desktop/infowidget.cpp\
           ../../gui/desktop/main.cpp \
           ../../gui/desktop/mainwindow.cpp
QT += widgets
CONFIG += debug

