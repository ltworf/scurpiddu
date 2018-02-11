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

#include "metadata.h"

Metadata::Metadata(QObject *parent) : QObject(parent)
{

}

void Metadata::setMetadata(QString key, QString value) {
    _map[key] = value;
    if (key == "album")
        emit albumChanged(value);
    else if (key == "album_artist")
        emit album_artistChanged(value);
    else if (key == "artist")
        emit artistChanged(value);
    else if (key == "comment")
        emit commentChanged(value);
    else if (key == "date")
        emit dateChanged(value);
    else if (key == "disc")
        emit discChanged(value);
    else if (key == "genre")
        emit genreChanged(value);
    else if (key == "publisher")
        emit publisherChanged(value);
    else if (key == "title")
        emit titleChanged(value);
    else if (key == "track")
        emit trackChanged(value);
}

void Metadata::clear() {
    _map.clear();
    emit albumChanged("");
    emit album_artistChanged("");
    emit artistChanged("");
    emit commentChanged("");
    emit dateChanged("");
    emit discChanged("");
    emit genreChanged("");
    emit publisherChanged("");
    emit titleChanged("");
    emit trackChanged("");
}

QString Metadata::album() { return _map.value("album", ""); }
QString Metadata::album_artist() { return _map.value("album_artist", ""); }
QString Metadata::artist() { return _map.value("artist", ""); }
QString Metadata::comment() { return _map.value("comment", ""); }
QString Metadata::date() { return _map.value("date", ""); }
QString Metadata::disc() { return _map.value("disc", ""); }
QString Metadata::genre() { return _map.value("genre", ""); }
QString Metadata::publisher() { return _map.value("publisher", ""); }
QString Metadata::title() { return _map.value("title", ""); }
QString Metadata::track() { return _map.value("track", ""); }
