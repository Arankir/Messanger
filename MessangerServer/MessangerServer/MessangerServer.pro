
QT += network sql core gui widgets
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        database.cpp \
        formsettings.cpp \
        loghelper.cpp \
        main.cpp \
        maintask.cpp \
        server.cpp \
        tokengenerator.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    database.h \
    formsettings.h \
    loghelper.h \
    maintask.h \
    server.h \
    tokengenerator.h

RESOURCES += \
    images/images.qrc

FORMS += \
    formsettings.ui
