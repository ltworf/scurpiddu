/*
This file is part of Scurpiddu.

Scurpiddu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Scurpiddu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Scurpiddu. If not, see <http://www.gnu.org/licenses/>.

Copyright (C) 2018  Salvo "LtWorf" Tomaselli <tiposchi@tiscali.it>
*/

#include <QDebug>
#include <mpv/qthelper.hpp>

#include "audioplayer.h"

static void wakeup(void *mpv)
{
    AudioPlayer *mainwindow = (AudioPlayer*)mpv;
    emit mainwindow->mpv_events();
}

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent)
{
    std::setlocale(LC_NUMERIC, "C");

    mpv = mpv_create();
    /*if (!mpv) TODO
        throw std::runtime_error("can't create mpv instance");
        */
    // Let us receive property change events with MPV_EVENT_PROPERTY_CHANGE if
    // this property changes.
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "volume", MPV_FORMAT_DOUBLE);


    // From this point on, the wakeup function will be called. The callback
    // can come from any thread, so we use the QueuedConnection mechanism to
    // relay the wakeup in a thread-safe way.
    connect(this, &AudioPlayer::mpv_events, this, &AudioPlayer::on_mpv_events,
            Qt::QueuedConnection);
    mpv_set_wakeup_callback(mpv, wakeup, this);

    if (mpv_initialize(mpv) < 0)
        throw std::runtime_error("mpv failed to initialize");
}

AudioPlayer::~AudioPlayer() {
    if (mpv)
        mpv_terminate_destroy(mpv);
}

double AudioPlayer::volume()  {
    double result = 0;
    mpv_get_property(mpv, "volume", MPV_FORMAT_DOUBLE, &result);
    return result;
}

void AudioPlayer::setVolume(double v) {
    mpv_set_property_async(mpv, 0, "volume", MPV_FORMAT_DOUBLE, &v);
}

void AudioPlayer::open(QByteArray path) {
    const char *args[] = {"loadfile", path, NULL};
    mpv_command_async(mpv, 0, args);
}

void AudioPlayer::seek(double position) {
    const char* value = QString::number(position).toStdString().c_str();
    const char *args[] = {"seek", value, "absolute", NULL};
    mpv_command_async(mpv, 0, args);
}

void AudioPlayer::handle_mpv_event(mpv_event *event)
{
    mpv_event_property *prop = (mpv_event_property *)event->data;

    switch (event->event_id) {
        case MPV_EVENT_LOG_MESSAGE:
            qDebug() << "log message";
            break;
        case MPV_EVENT_GET_PROPERTY_REPLY:
            qDebug() << "get prop reply";
            break;
        case MPV_EVENT_SET_PROPERTY_REPLY:
            qDebug() << "set prop reply";
            break;
        case MPV_EVENT_COMMAND_REPLY:
            qDebug() << "command reply";
            break;
        case MPV_EVENT_START_FILE:
            qDebug() << "start file";
            break;
        case MPV_EVENT_FILE_LOADED: {
            double result = 0;
            mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &result);
            emit duration(result);
            break;
            }
        case MPV_EVENT_TRACKS_CHANGED:
            qDebug() << "tracks changed";
            break;
        case MPV_EVENT_TRACK_SWITCHED:
            qDebug() << "tracks switched";
           break;
        case MPV_EVENT_IDLE:
            qDebug() << "idle";
            break;
        case MPV_EVENT_PAUSE:
            qDebug() << "pause";
           break;
        case MPV_EVENT_UNPAUSE:
            qDebug() << "unpause";
            break;
        case MPV_EVENT_TICK:
            qDebug() << "tick";
            break;
        case MPV_EVENT_SCRIPT_INPUT_DISPATCH:
            qDebug() << "script input dispatch";
            break;
        case MPV_EVENT_CLIENT_MESSAGE:
            qDebug() << "client message";
            break;
        case MPV_EVENT_VIDEO_RECONFIG:
            qDebug() << "video reconfig";
            break;
        case MPV_EVENT_AUDIO_RECONFIG:
            qDebug() << "audio reconfig";
            break;
        case MPV_EVENT_METADATA_UPDATE:
            qDebug() << "metadata update";
            break;
        case MPV_EVENT_SEEK:
            qDebug() << "seek";
            break;
        case MPV_EVENT_PLAYBACK_RESTART:
            qDebug() << "playback restart";
            break;
        case MPV_EVENT_CHAPTER_CHANGE:
            qDebug() << "chapter change";
            break;
        case MPV_EVENT_QUEUE_OVERFLOW:
            qDebug() << "queue overflow";
            break;
        case MPV_EVENT_END_FILE:
            emit completed();
            break;
        case MPV_EVENT_PROPERTY_CHANGE: {
            QString name(prop->name);
            double value = 0;
            switch(prop->format){
                case MPV_FORMAT_DOUBLE:
                    value = *(double *)prop->data;
                default:
                    ;
            }

            if (name == "time-pos") {
                emit progress(value);
            } else if (name == "volume") {
                emit volumeChanged(value);
            }
            break;
            }
        case MPV_EVENT_SHUTDOWN:
            mpv_terminate_destroy(mpv);
            mpv = NULL;
            break;
        default:
            break;
    }
}

// This slot is invoked by wakeup() (through the mpv_events signal).
void AudioPlayer::on_mpv_events()
{
    // Process all events, until the event queue is empty.
    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;
        handle_mpv_event(event);
    }
}

