#ifndef MUSIC_H
#define MUSIC_H

#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QList>
#include <QUrl>
#include <QAudioOutput>
#include <QCoreApplication>

class Music : public QObject {
    Q_OBJECT
public:
    static Music* getInstance();

    // 控制接口
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();
    Q_INVOKABLE void playByName(const QString& musicName);
    Q_INVOKABLE void continuePlay();
    Q_INVOKABLE void setVolume(int volume);

private:
    explicit Music(QObject *parent = nullptr);
    Music(const Music&) = delete;
    Music& operator=(const Music&);

    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    QList<QString> m_musicList;
    int mCurrentTrack{0};
    qint64 m_lastStoppedPosition{0}; // 上次停止的位置

private slots:
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState status);
};

#endif // MUSIC_H
