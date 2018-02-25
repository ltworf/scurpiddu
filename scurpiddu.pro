QT += core gui widgets sql

TARGET = scurpiddu
TEMPLATE = app
LIBS += -lmpv -ltag

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
@PKGCONFIG += mpv

SOURCES += \
    src/audioplayer.cpp \
    src/mainwindow.cpp \
    main.cpp \
    src/metadata.cpp \
    src/playlist/localcollection.cpp
HEADERS += \
    src/audioplayer.h \
    src/mainwindow.h \
    src/metadata.h \
    src/playlist/localcollection.h

FORMS += \
    ui/mainwindow.ui
