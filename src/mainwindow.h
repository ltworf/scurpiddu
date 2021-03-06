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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QString>
#include <QPushButton>
#include <QSystemTrayIcon>

#include "audioplayer.h"
#include "playlist/playlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void playlistSelect(QModelIndex);
    void nextTrack();
    void previousTrack();
    void player_status_changed(AudioPlayer::States);
    void update_metadata(QString key, QString value);
    void update_track_info(QPushButton*);
    void playlist_track_changed();
private slots:
    void tray_action(QSystemTrayIcon::ActivationReason);

private:
    Ui::MainWindow *ui;
    AudioPlayer player;
    Playlist playlist;
    QSystemTrayIcon tray;
};

#endif // MAINWINDOW_H
