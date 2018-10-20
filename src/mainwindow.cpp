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
#include <QInputDialog>
#include <QCoreApplication>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlist/playlistcreator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Tray icon
    tray.setIcon(QIcon::fromTheme("media-playback-start"));
    tray.setToolTip("Scurpiddu");
    tray.setContextMenu(ui->menu_File);
    tray.show();

    connect(
                &tray,
                &QSystemTrayIcon::activated,
                this,
                &MainWindow::tray_action
    );

    //Quit menu
    connect(
                ui->action_Quit,
                &QAction::triggered,
                QCoreApplication::instance(),
                &QCoreApplication::quit
    );

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
                ui->actionPlay_pause,
                &QAction::triggered,
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

    connect(
                &playlist,
                &Playlist::trackChanged,
                this,
                &MainWindow::playlist_track_changed
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

    connect(
                ui->cmdDelete,
                &QPushButton::clicked,
                [this]() {
                    this->playlist.removeTracks(this->ui->playlistView->selectionModel()->selectedIndexes());
                }
    );

    //Track info
    connect(
                ui->cmdTitle,
                &QPushButton::clicked,
                [this]() {
                    this->update_track_info(ui->cmdTitle);
                }
    );

    connect(
                ui->cmdArtist,
                &QPushButton::clicked,
                [this]() {
                    this->update_track_info(ui->cmdArtist);
                }
    );

    connect(
                ui->cmdAlbum,
                &QPushButton::clicked,
                [this]() {
                    this->update_track_info(ui->cmdAlbum);
                }
    );


    // Associate model & view
    ui->playlistView->setModel(&playlist);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tray_action(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::MiddleClick:
        this->player.playpause();
        break;
    case QSystemTrayIcon::Trigger:
        if (this->isHidden())
            this->show();
        else
            this->hide();
        break;
    default:
        break;
    }

}

void MainWindow::update_track_info(QPushButton *l) {
    PlaylistItem* current = this->playlist.getPlaying();
    if (current == NULL)
        return;
    QInputDialog dialog(this);
    bool ok;
    QString val = dialog.getText(this, "Update track info", l->toolTip(), QLineEdit::Normal, l->text(), &ok);
    if (! ok || val.isEmpty())
        return;

    //If the track changed meanwhile, do nothing
    if (this->playlist.getPlaying() != current)
        return;

    l->setText(val);
    if (l == this->ui->cmdTitle)
        current->setTitle(val);
    else if (l == this->ui->cmdAlbum)
        current->setAlbum(val);
    else if (l == this->ui->cmdArtist)
        current->setArtist(val);
}

void MainWindow::playlist_track_changed() {
    PlaylistItem* current = this->playlist.getPlaying();
    if (current == NULL)
        return;

    ui->cmdAlbum->setText(current->album());
    ui->cmdArtist->setText(current->artist());
    ui->cmdTitle->setText(current->title());
    QString msg = "Now playing: " + current->title() + " - " + current->artist();
    tray.showMessage("Scurpiddu", msg, QIcon::fromTheme("media-playback-start"), 3000);
    tray.setToolTip("<h1>Scurpiddu</h1><br>" + msg);
}

void MainWindow::player_status_changed(AudioPlayer::States newstate) {
    QIcon new_icon;
    switch (newstate) {
        case  AudioPlayer::States::PLAYING:
            new_icon = QIcon::fromTheme("media-playback-pause");
            break;
        case  AudioPlayer::States::PAUSED:
        case  AudioPlayer::States::STOPPED:
            new_icon = QIcon::fromTheme("media-playback-start");
            break;
    }
    tray.setIcon(new_icon);
    ui->cmdPlayPause->setIcon(new_icon);
    ui->actionPlay_pause->setIcon(new_icon);
}

void MainWindow::update_metadata(QString key, QString value) {
    if (key == "title") {
        ui->cmdTitle->setText(value);
    } else

    if (key == "album") {
        ui->cmdAlbum->setText(value);
    } else

    if (key == "artist") {
        ui->cmdArtist->setText(value);
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
