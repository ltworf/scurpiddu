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
    src/playlist/playlistitem.cpp \
    src/playlist/playlist.cpp \
    src/playlist/counterfilter.cpp \
    src/playlist/lastplayedfilter.cpp \
    src/playlist/playlistcreator.cpp \
    src/playlist/searchfilter.cpp
HEADERS += \
    src/audioplayer.h \
    src/mainwindow.h \
    src/metadata.h \
    src/playlist/localcollection.h \
    src/playlist/filter.h \
    src/playlist/randomfilter.h \
    src/playlist/playlistitem.h \
    src/playlist/playlist.h \
    src/playlist/lastplayedfilter.h \
    src/playlist/counterfilter.h \
    src/playlist/playlistcreator.h \
    src/playlist/searchfilter.h

FORMS += \
    ui/mainwindow.ui \
    src/playlist/playlistcreator.ui
