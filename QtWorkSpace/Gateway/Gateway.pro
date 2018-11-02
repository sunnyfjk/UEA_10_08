#-------------------------------------------------
#
# Project created by QtCreator 2018-10-31T11:55:36
#
#-------------------------------------------------

QT       += core gui serialport sql mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gateway
TEMPLATE = app

#要部署的在开发板上目录
target.path=/usr/share/demo
#安装目标文件
INSTALLS+=target


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        home.cpp \
    bedroom.cpp \
    cookroom.cpp \
    parlour.cpp \
    studyroom.cpp \
    subalternroom.cpp \
    toilet.cpp \
    getewaybase.cpp \
    stm32f407.cpp \
    historydata.cpp \
    mqttcommunications.cpp

HEADERS += \
        home.h \
    bedroom.h \
    cookroom.h \
    parlour.h \
    studyroom.h \
    subalternroom.h \
    toilet.h \
    getewaybase.h \
    stm32f407.h \
    historydata.h \
    mqttcommunications.h

FORMS += \
        home.ui \
    bedroom.ui \
    cookroom.ui \
    parlour.ui \
    studyroom.ui \
    subalternroom.ui \
    toilet.ui \
    historydata.ui
