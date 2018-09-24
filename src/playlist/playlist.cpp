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

#include "playlist.h"

#include <QBrush>
#include <QDateTime>
#include <QSize>
#include <algorithm>

Playlist::Playlist(QObject *parent): QAbstractListModel(parent)
{

}

int Playlist::rowCount(const QModelIndex &parent) const {
    return playlist.size();
}

QVariant Playlist::data(const QModelIndex &index, int role) const {
    PlaylistItem* item = playlist[index.row()];

    switch (role) {
    case Qt::DisplayRole:
        if (item->title().size())
            return item->title();
        else
            return item->path();
    case Qt::ToolTipRole: {
        return item->artist() + " - " + item->title();
    }
    case Qt::BackgroundRole:
        if (index.row() == _playing)
            return QBrush(Qt::darkRed, Qt::Dense4Pattern);
        break;
    case Qt::ForegroundRole:
        if (index.row() == _playing)
            return QBrush(Qt::white);
        break;
    default:
        ;
    }
    return QVariant();
}

void Playlist::setPlaylist(QList<PlaylistItem *> l) {
    qDeleteAll(playlist.begin(), playlist.end());
    playlist = l;

    emit this->dataChanged(
        this->createIndex(0, 0),
        this->createIndex(l.size() - 1, 0)
    );
    _playing = -1;
}

void Playlist::appendPlaylist(QList<PlaylistItem*> l) {
    int initial_size = playlist.size();
    playlist += l;
    emit this->dataChanged(
        this->createIndex(initial_size, 0),
        this->createIndex(l.size() - 1, 0)
    );
}

PlaylistItem* Playlist::playing_int(int i) {
    emit this->dataChanged(
        this->createIndex(_playing, 0),
        this->createIndex(_playing - 1, 0)
    );
    _playing = i;
    if (i == -1) {
        return NULL;
    }
    PlaylistItem* item = playlist[i];
    item->setCounter(item->counter() + 1);
    item->setLast_played(QDateTime().toSecsSinceEpoch());
    emit this->dataChanged(
        this->createIndex(_playing, 0),
        this->createIndex(_playing - 1, 0)
    );
    return item;
}

PlaylistItem* Playlist::playing(QModelIndex i) {
    return playing_int(i.row());
}

PlaylistItem* Playlist::next() {
    int now_playing = -1;
    if (_playing == -1 && playlist.size() > 0) {
        now_playing = 0;
    } else if (_playing == playlist.size() - 1) {
        now_playing = -1;
    } else {
        now_playing = _playing + 1;
    }
    return playing_int(now_playing);
}

PlaylistItem* Playlist::previous() {
    int now_playing = -1;
    if (_playing != -1) {
        now_playing = _playing - 1;
    }
    return playing_int(now_playing);
}

void Playlist::clear() {
    QList<PlaylistItem*> l;
    setPlaylist(l);
}

void Playlist::shuffle() {
    std::random_shuffle(playlist.begin(), playlist.end());
    emit this->dataChanged(
        this->createIndex(0, 0),
        this->createIndex(playlist.size() - 1, 0)
    );
}
