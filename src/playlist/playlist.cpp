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

#include <QDebug>
#include <QSize>

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
        return item->title();
    case Qt::ToolTipRole: {
        return item->artist() + " - " + item->title();
    }
    case Qt::DecorationRole:
        //TODO if (item->cover().size()) {}
    default:
        return QVariant();
    }
}

void Playlist::setPlaylist(QList<PlaylistItem *> l) {
    qDeleteAll(playlist.begin(), playlist.end());
    playlist = l;

    emit this->dataChanged(
        this->createIndex(0, 0),
        this->createIndex(l.size() - 1, 0)
    );
}

PlaylistItem* Playlist::getItem(QModelIndex i) {
    return playlist[i.row()];
}
