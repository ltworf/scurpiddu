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

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();
    Q_PROPERTY(
            double volume
            READ volume
            WRITE setVolume
            NOTIFY volumeChanged
    )

private:
    mpv_handle *mpv = NULL;
    void handle_mpv_event(mpv_event*);

signals:
    void completed();
    void duration(double duration);
    void progress(double pos);
    void mpv_events();
    void volumeChanged(double);

private slots:
    void on_mpv_events();

public slots:
    void open(QByteArray path);
    void seek(double position);
    void setVolume(double);
    double volume();

};

#endif // AUDIOPLAYER_H
