//
// Created by arch_ on 24-10-24.
//
#pragma once

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
