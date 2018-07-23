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

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>

class PlaylistItem : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistItem(QObject *parent, QString path, bool localcollection, QSqlDatabase* db);

signals:
    void scoreChanged(int);
    void counterChanged(int);
    void last_playedChanged(int);
    void coverChanged(QString);
    void albumChanged(QString);
    void album_artistChanged(QString);
    void artistChanged(QString);
    void commentChanged(QString);
    void dateChanged(QString);
    void discChanged(QString);
    void genreChanged(QString);
    void publisherChanged(QString);
    void titleChanged(QString);
    void trackChanged(QString);

public slots:
    bool localcollection();
    QString path();
    int score();
    int counter();
    int last_played();
    QString cover();
    void setCover(QString);
    QString album();
    void setAlbum(QString);
    QString album_artist();
    void setAlbum_artist(QString);
    QString artist();
    void setArtist(QString);
    QString comment();
    void setComment(QString);
    QString date();
    void setDate(QString);
    QString disc();
    void setDisc(QString);
    QString genre();
    void setGenre(QString);
    QString publisher();
    void setPublisher(QString);
    QString title();
    void setTitle(QString);
    QString track();
    void setTrack(QString);
    void initialised();
    void setScore(int);
    void setCounter(int);
    void setLast_played(int);

private:
    void apply_changes(QString field, QVariant value);
    bool init = true;
    QSqlDatabase *db;
    bool _localcollection;
    QString _path;
    //Playback attributes
    int _score = 0;
    int _counter = 0;
    int _last_played = 0;
    //Tags
    QString _cover;
    QString _album;
    QString _album_artist;
    QString _artist;
    QString _comment;
    QString _date;
    QString _disc;
    QString _genre;
    QString _publisher;
    QString _title;
    QString _track;
};

#endif // PLAYLISTITEM_H

