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


#ifndef PLAYLISTCREATOR_H
#define PLAYLISTCREATOR_H

#include <QList>
#include <QWidget>

#include "localcollection.h"
#include "playlistitem.h"

namespace Ui {
class PlaylistCreator;
}

class PlaylistCreator : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistCreator(QWidget *parent = 0);
    ~PlaylistCreator();
public slots:
    void random();
    void counter();
    void lastPlayed();
signals:
    void appendPlaylist(QList<PlaylistItem*>);

private:
    Ui::PlaylistCreator *ui;
    LocalCollection localcollection;
};

#endif // PLAYLISTCREATOR_H
