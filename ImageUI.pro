#-------------------------------------------------
#
# Project created by QtCreator 2019-05-18T14:25:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        filter.cpp \
        main.cpp \
        mainwindow.cpp \
        morphology.cpp \
        other.cpp \
        other2.cpp \
        threshold.cpp \
        utils.cpp

HEADERS += \
        filter.h \
        mainwindow.h \
        morphology.h \
        other.h \
        other2.h \
        threshold.h \
        utils.h

FORMS += \
        filter.ui \
        mainwindow.ui \
        morphology.ui \
        other.ui \
        other2.ui \
        threshold.ui
INCLUDEPATH += D:\OpenCV\opencv\build\include \
               D:\ObenCV\opencv\build\include\opencv \
               D:\OpenCV\opencv\build\include\opencv2 \

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world330
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world330d
else:unix: LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world330

INCLUDEPATH += $$PWD/../../OpenCV/opencv/build/x64/vc14
DEPENDPATH += $$PWD/../../OpenCV/opencv/build/x64/vc14
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
