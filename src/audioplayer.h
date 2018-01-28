#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QByteArray>
#include <mpv/client.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

private:
    mpv_handle *mpv = NULL;
    void handle_mpv_event(mpv_event*);

signals:
    void completed();
    void duration(double duration);
    void progress(double pos);
    void mpv_events();

private slots:
    void on_mpv_events();

public slots:
    void open(QByteArray path);
    void seek(double position);

};

#endif // AUDIOPLAYER_H
