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

Copyright (C) 2018-2019  Salvo "LtWorf" Tomaselli <tiposchi@tiscali.it>
*/

#include "playlist.h"

#include <QBrush>
#include <QDateTime>
#include <QSize>
#include <QSettings>
#include <algorithm>

Playlist::Playlist(QObject *parent): QAbstractListModel(parent)
{

}

void Playlist::saveDump() {
    QSettings settings;
    settings.setValue("playlist/playlist", this->dump());
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

void Playlist::removeTracks(QModelIndexList l) {
    int min = playlist.size();
    int removed = 0;

    //The selection is sorted in order of click, sort it
    std::sort(l.begin(), l.end());

    for (int i = 0; i < l.size(); i++) {
        int row = l[i].row();
        if (row - removed >= playlist.size())
            break;

        if (min > row)
            min = row;
        if (_playing == row - removed)
            updatePlaying(-1);
        else if (_playing > row - removed)
            _playing--;

        delete playlist[row - removed];
        playlist.removeAt(row - removed);

        removed++;
    }

    emit this->dataChanged(
        this->createIndex(min, 0),
        this->createIndex(playlist.size() - 1, 0)
    );
    this->saveDump();
    QSettings settings;
    settings.setValue("playlist/current", this->_playing);
}

void Playlist::setPlaylist(QList<PlaylistItem *> l) {
    qDeleteAll(playlist.begin(), playlist.end());
    playlist = l;

    emit this->dataChanged(
        this->createIndex(0, 0),
        this->createIndex(l.size() - 1, 0)
    );
    updatePlaying(-1);
    this->saveDump();
}

void Playlist::appendPlaylist(QList<PlaylistItem*> l) {
    int initial_size = playlist.size();
    playlist += l;
    emit this->dataChanged(
        this->createIndex(initial_size, 0),
        this->createIndex(l.size() - 1, 0)
    );
    this->saveDump();
}

PlaylistItem* Playlist::playing_int(int i) {
    emit this->dataChanged(
        this->createIndex(_playing, 0),
        this->createIndex(_playing - 1, 0)
    );
    updatePlaying(i);
    if (i == -1) {
        return nullptr;
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

PlaylistItem* Playlist::setPlaying(QModelIndex i) {
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

PlaylistItem* Playlist::getPlaying() {
    if (_playing != -1)
        return this->playlist[_playing];
    return nullptr;
}

void Playlist::clear() {
    QList<PlaylistItem*> l;
    setPlaylist(l);
}

void Playlist::shuffle() {
    updatePlaying(-1);
    std::random_shuffle(playlist.begin(), playlist.end());
    emit this->dataChanged(
        this->createIndex(0, 0),
        this->createIndex(playlist.size() - 1, 0)
    );
    this->saveDump();
}

void Playlist::updatePlaying(int new_val) {
    if (new_val == _playing)
        return;
    _playing = new_val;
    emit trackChanged();

    QSettings settings;
    settings.setValue("playlist/current", this->_playing);
}

QStringList Playlist::dump() {
    QStringList r;
    for (int i = 0; i < playlist.size(); i++) {
        r.append(playlist[i]->path());
    }
    return r;
}
