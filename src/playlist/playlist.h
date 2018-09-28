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


#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QModelIndex>
#include <QModelIndexList>

#include "playlistitem.h"

class Playlist : public QAbstractListModel
{
    Q_OBJECT
public:
    Playlist(QObject *parent = NULL);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public slots:
    void setPlaylist(QList<PlaylistItem*>);
    void appendPlaylist(QList<PlaylistItem*>);
    PlaylistItem* setPlaying(QModelIndex);
    PlaylistItem* next();
    PlaylistItem* previous();
    PlaylistItem* getPlaying();
    void removeTracks(QModelIndexList);
    void clear();
    void shuffle();

private:
    int _playing = -1;
    QList<PlaylistItem*> playlist;
    PlaylistItem* playing_int(int);
};

#endif // PLAYLIST_H
