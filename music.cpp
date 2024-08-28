#include "music.h"

Music* Music::getInstance() {
    static Music *instance = nullptr;
    if (!instance) {
        instance = new Music();
    }
    return instance;
}

Music::Music(QObject *parent) : QObject(parent), m_mediaPlayer(new QMediaPlayer(this))
{
    //
    // 大坑！！！ 得写QUrl("qrc:/1.wav")
    // QUrl：：fromlocalfile必须绝对路径！
    //
    m_musicList << "qrc:/1.wav" << "qrc:/2.wav" << "qrc:/3.wav"; // 初始化音乐列表
    mCurrentTrack = 0;


    m_audioOutput = new QAudioOutput(this); // 在构造函数中创建音频输出实例
    m_mediaPlayer->setAudioOutput(m_audioOutput); // 将音频输出与媒体播放器关联

    // 连接正确的信号槽
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &Music::onPlaybackStateChanged);

    start(); // 默认开始播放
}

void Music::start() {
    if (mCurrentTrack >= 0 && mCurrentTrack < m_musicList.size()) {
        playByName(m_musicList.at(mCurrentTrack));
    }
}

void Music::stop() {
    m_lastStoppedPosition = m_mediaPlayer->position(); // 记录停止位置
    m_mediaPlayer->stop();
}

void Music::next() {
    mCurrentTrack = (mCurrentTrack + 1) % m_musicList.size();
    playByName(m_musicList.at(mCurrentTrack));
}

void Music::playByName(const QString &musicName) {
    int index = m_musicList.indexOf(musicName);
    if (index != -1) {
        // 找到了音乐名，更新当前播放曲目索引
        mCurrentTrack = index;

        m_mediaPlayer->setSource(QUrl(musicName));
        m_lastStoppedPosition = 0; // 重置停止位置，因为是从头开始播放

        m_mediaPlayer->play();
    }
}

void Music::continuePlay() {
    if (m_lastStoppedPosition > 0) {
        m_mediaPlayer->setPosition(m_lastStoppedPosition); // 继续播放
        m_mediaPlayer->play();
    } else {
        start(); // 如果没有记录停止位置，则从头开始播放
    }
}

void Music::setVolume(int volume) {
    // 限制音量在0到100之间
    volume = qBound(0, volume, 100);

    // 设置音量（转换为0.0到1.0之间的小数）
    m_audioOutput->setVolume(float(volume) / 100.0);
}

void Music::onPlaybackStateChanged(QMediaPlayer::PlaybackState status) {
    // 注意：这里使用MediaStatus代替了之前的State
    if (status == QMediaPlayer::StoppedState && m_mediaPlayer->error() == QMediaPlayer::NoError) { // 注意：此处StoppedState属于MediaStatus枚举
        // 如果是正常停止，则记录位置
        m_lastStoppedPosition = 0;
        next();
    }
    // 可能需要根据MediaStatus的其他值来调整其他逻辑
}
