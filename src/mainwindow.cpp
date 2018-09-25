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

#include <QDebug>
#include <QPushButton>
#include <QIcon>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlist/playlistcreator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Volume
    connect(
                ui->volume,
                &QAbstractSlider::valueChanged,
                &this->player,
                &AudioPlayer::setVolume
    );

    // Seek bar
    connect(
                &player,
                &AudioPlayer::durationChanged,
                ui->seekBar,
                &QAbstractSlider::setMaximum
    );

    connect(
                &player,
                &AudioPlayer::progressChanged,
                ui->seekBar,
                &QAbstractSlider::setValue
    );

    connect(
                ui->seekBar,
                &QAbstractSlider::sliderMoved,
                &player,
                &AudioPlayer::seek
    );

    // Play/pause
    connect(
                ui->cmdPlayPause,
                &QPushButton::clicked,
                &player,
                &AudioPlayer::playpause
    );

    connect(
                &player,
                &AudioPlayer::stateChanged,
                this,
                &MainWindow::player_status_changed
    );

    //Metadata
    connect(
                &player,
                &AudioPlayer::metadataChanged,
                this,
                &MainWindow::update_metadata
    );

    // Next on track completed
    connect(
                &player,
                &AudioPlayer::completed,
                this,
                &MainWindow::nextTrack
    );

    // Append playlists from creator
    connect(
                ui->playlistCreator,
                &PlaylistCreator::appendPlaylist,
                &this->playlist,
                &Playlist::appendPlaylist
    );

    //Playlist controls
    connect(
                ui->cmdClearPlaylist,
                &QPushButton::clicked,
                &playlist,
                &Playlist::clear
    );

    connect(
                ui->cmdShufflePlaylist,
                &QPushButton::clicked,
                &playlist,
                &Playlist::shuffle
    );

    // Associate model & view
    ui->playlistView->setModel(&playlist);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::player_status_changed(AudioPlayer::States newstate) {
    switch (newstate) {
        case  AudioPlayer::States::PLAYING:
            ui->cmdPlayPause->setIcon(QIcon::fromTheme("media-playback-pause"));
            break;
        case  AudioPlayer::States::PAUSED:
        case  AudioPlayer::States::STOPPED:
            ui->cmdPlayPause->setIcon(QIcon::fromTheme("media-playback-start"));
            break;
    }
}

void MainWindow::update_metadata(QString key, QString value) {
    PlaylistItem* current = this->playlist.getPlaying();

    if (key == "title") {
        ui->lblTitle->setText(value);
        if (current) current->setTitle(value);
    } else

    if (key == "album") {
        ui->lblAlbum->setText(value);
        if (current) current->setAlbum(value);
    } else

    if (key == "album_artist") {
        if (current) current->setAlbum_artist(value);
    } else

    if (key == "artist") {
        ui->lblArtist->setText(value);
        if (current) current->setArtist(value);
    } else

    if (key == "publisher") {
        if (current) current->setPublisher(value);
    } else

    if (key == "disc") {
        if (current) current->setDisc(value);
    } else

    if (key == "comment") {
        if (current) current->setComment(value);
    } else

    if (key == "date") {
        if (current) current->setDate(value);
    } else

    if (key == "genre") {
        if (current) current->setGenre(value);
    } else

    if (key == "track") {
        if (current) current->setTrack(value);
    }
}

void MainWindow::playlistSelect(QModelIndex i) {
    PlaylistItem* item = playlist.setPlaying(i);
    if (item)
        player.open(item->path().toUtf8());
}

void MainWindow::nextTrack() {
    PlaylistItem* item = playlist.next();
    if (item)
        player.open(item->path().toUtf8());
}

void MainWindow::previousTrack() {
    PlaylistItem* item = playlist.previous();
    if (item)
        player.open(item->path().toUtf8());
}
