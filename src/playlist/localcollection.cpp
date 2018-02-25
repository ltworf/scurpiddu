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

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <taglib/fileref.h>


#define INSERT_FIELDS "path, hash, title, album, artist, genre, track, date, comment, timestamp"
#define INSERT_MAPPED_FIELDS ":path, :hash, :title, :album, :artist, :genre, :track, :date, :comment, :timestamp"
#define DB_PATH "/tmp/scurpiddu.db" //FIXME put it somewhere where it makes sense
#define MEDIA_PATH "/home/salvo/mp3/bassplaylist" //FIXME make this configurable

void LocalCollection::create_db() {
    qDebug() << "Initializing db";
    QSqlQuery query;
    if(!query.exec("CREATE TABLE IF NOT EXISTS tracks ("
               //Special attributes
               "path TEXT PRIMARY KEY,"
               "hash TEXT UNIQUE,"
               "timestamp INTEGER,"
               //Tags
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
               ");"))
            qDebug() << query.lastError();
}

QString md5sum(QString path) {
    QFile file(path);
    QCryptographicHash hasher(QCryptographicHash::Md5);
    file.open(QIODevice::ReadOnly);
    while (true) {
        QByteArray data = file.read(1024);
        if (data.length() == 0)
            break;
        hasher.addData(data);
    }
    return QString(hasher.result().toHex());
}

void LocalCollection::populate() {
    qDebug() << "Scanning collection...";
    static const char * extensions[] = {
        ".mp3",
        ".opus",
        ".m4a",
        ".ogg",
        ".flac",
    };

    QDirIterator i(MEDIA_PATH, QDir::NoFilter, QDirIterator::Subdirectories);
    while (i.hasNext()) {
        i.next();

        //Skip non-files
        if (! i.fileInfo().isFile()) {
            continue;
        }

        //Skip non allowed extensions
        bool media = false;
        for (unsigned int j=0;j<sizeof(extensions)/sizeof(int*); j++) {
            if (i.fileName().endsWith(extensions[j])) {
                media = true;
            }
        }
        if (!media)
            continue;

         QSqlQuery query;

        //Do not rescan scanned files
        query.prepare ("SELECT timestamp FROM tracks WHERE path = :path");
        query.bindValue(":path", i.filePath());
        query.exec();
        if (query.next()) {
            qlonglong timestamp = query.value(0).toLongLong();
            if (timestamp > i.fileInfo().lastModified().toSecsSinceEpoch()) {
                // Not modified since last scan
                continue;
            } else {
                // Modified since last scan
                query.prepare("DELETE FROM tracks WHERE path = :path");
                query.bindValue(":path", i.filePath());
                query.exec();
            }
        }

        //Deal with files with the same hash
        QString hash = md5sum(i.filePath());
        query.prepare ("SELECT path FROM tracks WHERE hash = :hash");
        query.bindValue(":hash", hash);
        query.exec();
        if (query.next()) {
            QFile f(query.value(0).toString());
            if (! f.exists()) {
                // File seems to have been moved, delete the old one
                query.prepare("DELETE FROM tracks WHERE hash = :hash");
                query.bindValue(":hash", hash);
                query.exec();
            } else {
                // Just a duplicated file, skip
                continue;
            }
        }

        // Adding file to the collection
        query.prepare("INSERT INTO tracks (" INSERT_FIELDS ") VALUES (" INSERT_MAPPED_FIELDS ");");

        query.bindValue(":path", i.filePath());
        query.bindValue(":hash", hash);
        query.bindValue(":timestamp", QDateTime::currentSecsSinceEpoch());

        TagLib::FileRef tags(i.filePath().toStdString().c_str());
        //TODO tags.tag()->properties()
        if (!tags.tag()->isEmpty()) {
            query.bindValue(":title", tags.tag()->title().toCString(true));
            query.bindValue(":album", tags.tag()->album().toCString(true));
            query.bindValue(":artist", tags.tag()->artist().toCString(true));
            query.bindValue(":genre", tags.tag()->genre().toCString(true));
            query.bindValue(":track", tags.tag()->track());
            query.bindValue(":date", tags.tag()->year());
            query.bindValue(":comment", tags.tag()->comment().toCString(true));
        }

        if(!query.exec()) {
            qDebug() << "Query error" << query.lastError();
            qDebug() << "Last query" << query.lastQuery();
            qDebug() << "Last values" << query.boundValues() ;
            qDebug() << "Path" << i.filePath();
        }
    }
    qDebug() << "Scan completed";
}

LocalCollection::LocalCollection(QObject *parent) : QObject(parent)
{
    QFile f(DB_PATH);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);
    db.open();
    create_db();
    populate();
    if (! f.exists() || f.size() == 0) {

    }
}
