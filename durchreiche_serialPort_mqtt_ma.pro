QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    frmmain.cpp \
    portwrapper_ma.cpp

HEADERS += \
    async_mqtt/include/async_mqtt/all.hpp \
    async_mqtt/include/async_mqtt/client.hpp \
    frmmain.h \
    portwrapper_ma.h

FORMS += \
    frmmain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
