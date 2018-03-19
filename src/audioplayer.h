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

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QByteArray>
#include <mpv/client.h>

#include "metadata.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

    enum States {
        STOPPED = 0,
        PLAYING = 1,
        PAUSED = -1,
    };

    Q_ENUM(States)

    Q_PROPERTY(
            States state
            READ state
            NOTIFY stateChanged
    )

    Q_PROPERTY(
            double volume
            READ volume
            WRITE setVolume
            NOTIFY volumeChanged
    )

    Q_PROPERTY(double duration
               READ duration
               NOTIFY durationChanged
    )

    Q_PROPERTY(
            double progress
            READ progress
            WRITE seek
            NOTIFY progressChanged
    )

private:
    mpv_handle *mpv = NULL;
    void handle_mpv_event(mpv_event*);
    double _duration;
    double _progress;
    States _state;
    void _setState(States);
    Metadata _metadata;

signals:
    void completed();
    void durationChanged(double duration);
    void progressChanged(double progress);
    void _mpv_events();
    void volumeChanged(double);
    void stateChanged(States);
    void metadataChanged(QString key, QString value);

private slots:
    void on_mpv_events();

public slots:
    void open(QByteArray path);
    void seek(double position);
    void setVolume(double);
    double volume();
    double duration();
    double progress();
    States state();
    void playpause();
    void stop();
    Metadata* metadata();
};

#endif // AUDIOPLAYER_H
