//
// Created by arch_ on 24-10-24.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <SFML/Audio.hpp>
#include <future>
#include <QDebug>
#include <QThread>
using std::string;

class AudioPlayer {
public:
    explicit AudioPlayer(const string& filePath);
    std::future<int> play();
    [[nodiscard]] bool isPlaying() const;
    ~AudioPlayer() = default;

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};

class AudioPlayerQThread final : public QThread {
    Q_OBJECT

signals:
    void finish(int sec);

public:
    void run() override;
    explicit AudioPlayerQThread(const string& filePath)
        : player { new AudioPlayer { filePath } }
    {
    }
    ~AudioPlayerQThread() override { delete player; }

private:
    AudioPlayer* player;
};
#endif // PLAYER_H
