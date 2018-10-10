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
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <taglib/fileref.h>


#define INSERT_FIELDS "path, hash, title, album, artist, genre, track, date, comment, timestamp"
#define INSERT_MAPPED_FIELDS ":path, :hash, :title, :album, :artist, :genre, :track, :date, :comment, :timestamp"

void LocalCollection::create_db() {
    qDebug() << "Initializing db";
    QSqlQuery query;
    if(!query.exec("CREATE TABLE IF NOT EXISTS tracks ("
               //Special attributes
               "path TEXT PRIMARY KEY,"
               "hash TEXT UNIQUE,"
               "timestamp INTEGER,"
               //Playback attributes
               "score INTEGER DEFAULT 0,"
               "counter INTEGER DEFAULT 0,"
               "last_played INTEGER DEFAULT 0,"
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
    static const char * extensions[] = {
        ".mp3",
        ".opus",
        ".m4a",
        ".ogg",
        ".flac",
    };

    int inscounter = 0;
    int delcounter = 0;

    QStringList mediapaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    if (mediapaths.length() == 0) {
        qDebug() << "No music directory available";
        return;
    }
    qDebug() << "Scanning directory" << mediapaths[0];

    QDirIterator i(mediapaths[0], QDir::NoFilter, QDirIterator::Subdirectories);
    while (i.hasNext()) {
        i.next();

        //Skip non-files
        if (! i.fileInfo().isFile()) {
            continue;
        }

        //Skip non allowed extensions
        bool media = false;
        for (unsigned int j=0;j<sizeof(extensions)/sizeof(int*); j++) {
            if (i.fileName().toLower().endsWith(extensions[j])) {
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
                delcounter++;
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
                delcounter++;
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
        if (!tags.isNull() && !tags.tag()->isEmpty()) {
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
        } else
            inscounter++;
    }
    qDebug() << "Scan completed." << inscounter << "insertions," << delcounter << "removals";
}

LocalCollection::LocalCollection(QObject *parent) : QObject(parent)
{
    QDir cachedir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    cachedir.mkdir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));

    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/scurpiddu.db";
    qDebug() << "path" << dbpath;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbpath);
    db.open();
    create_db();
    populate();
}

/**
 * @brief LocalCollection::filter
 * @param f
 * @return a list of items.
 *
 * The items must all be freed separately from the list.
 */
QList<PlaylistItem*> LocalCollection::filter(Filter *f) {
    QList<PlaylistItem*> r;
    QSqlQuery query;
    query.setForwardOnly(true);
    f->prepare(&query);
    query.exec();
    while (query.next()) {
        PlaylistItem* item = new PlaylistItem(
                    this,
                    query.value("path").toString(),
                    true,
                    &this->db
        );
        item->setScore(query.value("score").toInt());
        item->setCounter(query.value("counter").toInt());
        item->setLast_played(query.value("last_played").toInt());
        item->setCover(query.value("cover").toString());
        item->setAlbum(query.value("album").toString());
        item->setAlbum_artist(query.value("album_artist").toString());
        item->setArtist(query.value("artist").toString());
        item->setComment(query.value("comment").toString());
        item->setDate(query.value("date").toString());
        item->setDisc(query.value("disc").toString());
        item->setGenre(query.value("genre").toString());
        item->setPublisher(query.value("publisher").toString());
        item->setTitle(query.value("title").toString());
        item->setTrack(query.value("track").toString());
        item->initialised();
        r.append(item);
    }
    return r;
}
