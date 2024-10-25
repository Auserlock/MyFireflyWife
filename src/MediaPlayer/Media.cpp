//
// Created by arch_ on 24-10-24.
//
#include "Media.h"
#include <stdexcept>
#include "Log.h"
#include <thread>
#include <chrono>

AudioPlayer::AudioPlayer(const string& filePath)
{
    if (!buffer.loadFromFile(filePath)) {
        logger->error("加载音频文件失败: " + filePath);
        throw std::runtime_error("加载音频文件失败: " + filePath);
    }
    sound.setBuffer(buffer);
}

[[nodiscard]] bool AudioPlayer::isPlaying() const
{
    return sound.getStatus() == sf::Sound::Playing;
}

std::future<int> AudioPlayer::play()
{
    std::promise<int> promise;
    auto future = promise.get_future();

    std::thread([this, promise = std::move(promise)]() mutable {
        try {
            const auto start = std::chrono::high_resolution_clock::now();
            sound.play();

            while (isPlaying()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            promise.set_value(static_cast<int>(duration));
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    }).detach();

    return future;
}

void AudioPlayerQThread::run()
{
    try {
        const auto duration = player->play().get();
        emit finish(duration);
    } catch (...) {
        logger->error("音频播放器遭遇未知错误.");
        emit finish(0);
    }
}