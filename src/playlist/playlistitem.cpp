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

#include "playlistitem.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

PlaylistItem::PlaylistItem(QObject *parent, QString path, bool localcollection, QSqlDatabase* db) : QObject(parent) {
    this->db = db;
    this->_path = path;
    this->_localcollection = localcollection;
}

/**
 * @brief PlaylistItem::initialised
 *
 * After all the properties have been assigned, call this.
 *
 * It will cause further changes to be reflected on the db.
 */
void PlaylistItem::initialised() {
    this->init = false;
}

QString PlaylistItem::path() {
    return this->_path;
}

QString PlaylistItem::cover() {
    return this->_cover;
}

QString PlaylistItem::album() {
    return this->_album;
}

bool PlaylistItem::localcollection() {
    return this->_localcollection;
}

int PlaylistItem::score() {
    return this->_score;
}

int PlaylistItem::counter() {
    return this->_counter;
}

int PlaylistItem::last_played() {
    return this->_last_played;
}

QString PlaylistItem::album_artist() {
    return this->_album_artist;
}

QString PlaylistItem::artist() {
    return this->_artist;
}

QString PlaylistItem::comment() {
    return this->_comment;
}

QString PlaylistItem::date() {
    return this->_date;
}

QString PlaylistItem::disc() {
    return this->_disc;
}

QString PlaylistItem::genre() {
    return this->_genre;
}

QString PlaylistItem::publisher() {
    return this->_publisher;
}

QString PlaylistItem::title() {
    return this->_title;
}

QString PlaylistItem::track() {
    return this->_track;
}

void PlaylistItem::apply_changes(QString field, QVariant value) {
    if (this->init)
        return;
    return; //TODO

    QSqlQuery query(*this->db);
    query.prepare(
                "UPDATE tracks"
                "SET " + field + "= :value"
                "WHERE path = :path ;"

                );
    query.bindValue(":value", value);
    query.bindValue(":path", this->_path);
    if (!query.exec()) {
        qDebug() << "Query error" << query.lastError();
        qDebug() << "Last query" << query.lastQuery();
        qDebug() << "Last values" << query.boundValues() ;
        qDebug() << "Path" << this->_path;
    }
}

void PlaylistItem::setCover(QString v) {
    this->apply_changes("cover", v);
    emit coverChanged(v);
    this->_cover = v;
}

void PlaylistItem::setAlbum(QString v) {
    this->apply_changes("album", v);
    emit albumChanged(v);
    this->_album = v;
}

void PlaylistItem::setAlbum_artist(QString v) {
    this->apply_changes("album_artist", v);
    emit this->album_artistChanged(v);
    this->_album_artist = v;
}

void PlaylistItem::setArtist(QString v) {
    this->apply_changes("artist", v);
    emit artistChanged(v);
    this->_artist = v;
}

void PlaylistItem::setComment(QString v) {
    this->apply_changes("comment", v);
    emit commentChanged(v);
    this->_comment = v;
}

void PlaylistItem::setDate(QString v) {
    this->apply_changes("date", v);
    emit dateChanged(v);
    this->_date = v;
}

void PlaylistItem::setDisc(QString v) {
    this->apply_changes("disc", v);
    emit discChanged(v);
    this->_disc = v;
}

void PlaylistItem::setGenre(QString v) {
    this->apply_changes("genre", v);
    emit genreChanged(v);
    this->_genre = v;
}

void PlaylistItem::setPublisher(QString v) {
    this->apply_changes("publisher", v);
    emit publisherChanged(v);
    this->_publisher = v;
}

void PlaylistItem::setTitle(QString v) {
    this->apply_changes("title", v);
    emit titleChanged(v);
    this->_title = v;
}

void PlaylistItem::setTrack(QString v) {
    this->apply_changes("track", v);
    emit trackChanged(v);
    this->_track = v;
}

void PlaylistItem::setScore(int v) {
    this->apply_changes("score", v);
    emit scoreChanged(v);
    this->_score = v;
}

void PlaylistItem::setCounter(int v) {
    this->apply_changes("counter", v);
    emit counterChanged(v);
    this->_counter = v;
}

void PlaylistItem::setLast_played(int v) {
    this->apply_changes("last_played", v);
    emit last_playedChanged(v);
    this->_last_played = v;
}
