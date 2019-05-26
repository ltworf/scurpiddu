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

#include <QDebug>
#include "playlistcreator.h"
#include "ui_playlistcreator.h"
#include "randomfilter.h"
#include "lastplayedfilter.h"
#include "counterfilter.h"
#include "searchfilter.h"
#include "restorefilter.h"

PlaylistCreator::PlaylistCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistCreator)
{
    ui->setupUi(this);
}

PlaylistCreator::~PlaylistCreator()
{
    delete ui;
}

void PlaylistCreator::random() {
    RandomFilter f(ui->spinLimit->value());
    emit appendPlaylist(localcollection.filter(&f));
}

void PlaylistCreator::counter() {
    CounterFilter f(ui->spinLimit->value(), ui->checkDesc->checkState());
    emit appendPlaylist(localcollection.filter(&f));
}

void PlaylistCreator::lastPlayed() {
    LastPlayedFilter f(ui->spinLimit->value(), ui->checkDesc->checkState());
    emit appendPlaylist(localcollection.filter(&f));
}

void PlaylistCreator::search() {
    SearchFilter f(ui->spinLimit->value(), this->ui->txtSearch->text());
    emit appendPlaylist(localcollection.filter(&f));
}

void PlaylistCreator::restore() {
    RestoreFilter f;
    emit appendPlaylist(localcollection.filter(&f));
}

