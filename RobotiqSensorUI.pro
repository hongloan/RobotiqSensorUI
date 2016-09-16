#-------------------------------------------------
#
# Project created by QtCreator 2013-09-27T10:53:25
#
#-------------------------------------------------

QT += core gui widgets

QT += serialport

# remove -Wextra
CONFIG += warn_off
QMAKE_CXXFLAGS += -Wall

TARGET = RobotiqSensorUI
TEMPLATE = app

RC_FILE = Robotiq.rc

win32 {
    # Note: mathgl is built without C++11 on windows, and it's ABI incompatible with C++11.
    # Qt supports C++98 only up to Qt 5.6, so make sure you don't use a later version.
    # TODO: see if Linux has a similar issue or not (unlikely)
    win32: CONFIG -= c++11

    # Use gcc's standard STDIO instead of the windows one.  Windows is stuck at C89, which doesn't
    # have some features such as %zu for size_t.  Additionally, windows doesn't support %n, because,
    # and get this, it's a security issue.
    DEFINES += __USE_MINGW_ANSI_STDIO
    #DEFINES += MGL_STATIC_DEFINE

    LIBS += -L"$$PWD/external/mathgl-2.3.5.1-mingw.i686/lib/"
    LIBS += -L"$$PWD/external/gsl-1.8/lib/"
    LIBS += -L"$$PWD/external/fftw-3.3.3-dll32/"

    INCLUDEPATH += "$$PWD/external/mathgl-2.3.5.1-mingw.i686/include/"
    INCLUDEPATH += "$$PWD/external/gsl-1.8/include/"
    INCLUDEPATH += "$$PWD/external/fftw-3.3.3-dll32/"
}

LIBS += -lmgl -lgsl

win32: LIBS += -lfftw3-3
linux: LIBS += -lfftw3

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/visualizationform.cpp \
    src/communicationhandler.cpp \
    src/aboutrobotiqsensorui.cpp \
    src/adresssettingwindow.cpp \
    src/gripper.cpp \
    src/crc16lookuptable.cpp \
    src/changesensoridform.cpp

HEADERS += src/mainwindow.h \
    src/visualizationform.h \
    src/communicationhandler.h \
    src/aboutrobotiqsensorui.h \
    src/adresssettingwindow.h \
    src/gripper.h \
    src/crc16lookuptable.h \
    src/changesensoridform.h

FORMS += src/mainwindow.ui \
    src/visualizationform.ui \
    src/aboutrobotiqsensorui.ui \
    src/adresssettingwindow.ui \
    src/changesensoridform.ui

RESOURCES += \
    images/images.qrc

win32 {
    # a post-link step that copies dll files next to the executable because windows is retarded
    QMAKE_POST_LINK += cp "$$PWD/external/mathgl-2.3.5.1-mingw.i686/bin/libmgl.dll" \
                          "$$PWD/external/fftw-3.3.3-dll32/libfftw3-3.dll" \
                          "$$PWD/external/pthreadGC2.dll" \
                          .
}
