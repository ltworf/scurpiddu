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

#ifndef METADATA_H
#define METADATA_H

#include <QMap>
#include <QObject>
#include <QString>

class Metadata : public QObject
{
    Q_OBJECT
public:
    explicit Metadata(QObject *parent = nullptr);

    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(QString album_artist READ album_artist NOTIFY album_artistChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged)
    Q_PROPERTY(QString date READ date NOTIFY dateChanged)
    Q_PROPERTY(QString disc READ disc NOTIFY discChanged)
    Q_PROPERTY(QString genre READ genre NOTIFY genreChanged)
    Q_PROPERTY(QString publisher READ publisher NOTIFY publisherChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString track READ track NOTIFY trackChanged)

signals:
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
    void clear();
    void setMetadata(QString key, QString value);
    QString album();
    QString album_artist();
    QString artist();
    QString comment();
    QString date();
    QString disc();
    QString genre();
    QString publisher();
    QString title();
    QString track();

private:
    QMap<QString,QString> _map;
};

#endif // METADATA_H
