QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    class/account.cpp \
    class/message.cpp \
    widget/groupwidget.cpp \
    widget/popup.cpp \
    widget\chat.cpp \
    widget\chatmessage.cpp \
    class\chatreceiver.cpp \
    class\client.cpp \
    formauth.cpp \
    class\group.cpp \
    main.cpp \
    mainwindow.cpp \
    widget\accountwidget.cpp

HEADERS += \
    class/account.h \
    class/message.h \
    widget/groupwidget.h \
    widget/popup.h \
    widget\chat.h \
    widget\chatmessage.h \
    class\chatreceiver.h \
    class\client.h \
    formauth.h \
    class\group.h \
    mainwindow.h \
    widget\accountwidget.h

FORMS += \
    widget/groupwidget.ui \
    widget\chat.ui \
    widget\chatmessage.ui \
    formauth.ui \
    mainwindow.ui \
    widget\accountwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
