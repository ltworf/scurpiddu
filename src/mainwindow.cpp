#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(
                ui->volume,
                &QAbstractSlider::valueChanged,
                &this->player,
                &AudioPlayer::setVolume
    );

    // Seek bar
    connect(
                &player,
                &AudioPlayer::duration,
                ui->seekBar,
                &QAbstractSlider::setMaximum
    );

    connect(
                &player,
                &AudioPlayer::progress,
                ui->seekBar,
                &QAbstractSlider::setValue
    );

    connect(
                ui->seekBar,
                &QAbstractSlider::sliderMoved,
                &player,
                &AudioPlayer::seek
    );

}

MainWindow::~MainWindow()
{
    delete ui;
}
