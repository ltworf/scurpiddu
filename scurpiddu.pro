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
    src/playlist/localcollection.cpp \
    src/playlist/randomfilter.cpp \
    src/playlist/playlistitem.cpp
HEADERS += \
    src/audioplayer.h \
    src/mainwindow.h \
    src/metadata.h \
    src/playlist/localcollection.h \
    src/playlist/filter.h \
    src/playlist/randomfilter.h \
    src/playlist/playlistitem.h

FORMS += \
    ui/mainwindow.ui
