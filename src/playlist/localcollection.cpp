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

#include "localcollection.h"

#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

#define DB_PATH "/tmp/scurpiddu.db" //FIXME put it somewhere where it makes sense
#define MEDIA_PATH "/home/salvo/mp3/" //FIXME make this configurable

void LocalCollection::create_db() {
    qDebug() << "Initializing db";
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS tracks ("
               "path TEXT PRIMARY KEY,"
               "hash TEXT UNIQUE,"
               "cover TEXT,"
               "album TEXT,"
               "album_artist TEXT,"
               "artist TEXT,"
               "comment TEXT,"
               "date TEXT,"
               "disc TEXT,"
               "genre TEXT,"
               "publisher TEXT,"
               "title TEXT,"
               "track TEXT"
               ");");
    qDebug() << query.lastError();
}

void LocalCollection::populate() {

}

LocalCollection::LocalCollection(QObject *parent) : QObject(parent)
{
    QFile f(DB_PATH);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);
    db.open();
    if (! f.exists() || f.size() == 0) {
        create_db();
        populate();
    }
}
