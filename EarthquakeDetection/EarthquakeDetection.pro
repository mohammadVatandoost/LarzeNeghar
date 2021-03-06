QT -= gui
QT += serialport
QT += sql
QT += widgets
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    sensorslist.cpp \
    csv.cpp \
    sensor.cpp \
    backend.cpp \
    algorithm.cpp \
    sensordata.cpp \
    Biquad.cpp \
    Butterworth.cpp \
    jsonfileapi.cpp \
    algorithmthread.cpp \
    LowPassFilter.cpp \
    Cascade.cpp \
    State.cpp \
    PoleFilter.cpp \
    jsonstoring.cpp

LIBS += /usr/local/lib/libfftw3.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    sensorslist.h \
    csv.h \
    sensor.h \
    backend.h \
    packetsdefine.h \
    algorithm.h \
    sensordata.h \
    Biquad.h \
    Butterworth.h \
    jsonfileapi.h \
    algorithmthread.h \
    LowPassFilter.hpp \
    Common.h \
    Cascade.h \
    State.h \
    Types.h \
    PoleFilter.h \
    MathSupplement.h \
    jsonstoring.h
