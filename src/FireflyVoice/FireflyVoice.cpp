//
// Created by arch_ on 24-10-26.
//
#include "FireflyVoice.h"
#include <QTime>
#include <fstream>
#include <random>

json& FireflyVoice::getAllVoices()
{
    std::ifstream config_file { FIREFLY_VOICE };
    if (!config_file.is_open()) {
        logger->error("无法打开配置文件: " + FIREFLY_VOICE);
        throw std::runtime_error("无法打开配置文件: " + FIREFLY_VOICE);
    }

    if (!(config_file >> voices)) {
        logger->error("读取配置失败");
        throw std::runtime_error("读取配置失败");
    }
    return voices;
}

QString FireflyVoice::getTimeOfDay()
{
    {
        const int hour = QTime::currentTime().hour();
        if (hour >= 6 && hour < 8)
            return "morning";
        if (hour >= 10 && hour < 12)
            return "noon";
        if (hour >= 18 && hour < 21)
            return "evening";
        if (hour >= 21 || hour < 6)
            return "night";
        return "other";
    }
}

FireflyVoiceQThread::FireflyVoiceQThread(const string& key_)
{
    key = key_;
    voice = new FireflyVoice {};
    voice->getAllVoices();
    audioPlayer = nullptr;
}

void FireflyVoiceQThread::run()
{
    json selected_voice {};
    try {
        json voice_time = voice->getVoice(key);
        const string time = FireflyVoice::getTimeOfDay().toStdString();
        auto voice_dict = voice_time[time];

        std::random_device rd; // 获取随机数种子
        std::mt19937 gen(rd()); // 使用 Mersenne Twister 算法
        std::uniform_int_distribution<> dis(0, static_cast<int>(voice_dict.size() - 1));

        const int random_index = dis(gen);
        selected_voice = voice_dict[random_index];
        audioPlayer = new AudioPlayer(selected_voice["wav"]);
    } catch (std::exception& e) {
        logger->error(e.what());
    }

    emit startPlay(selected_voice["title"]);
    if (audioPlayer) {
        const auto duration = audioPlayer->play().get();
        emit finishPlay(duration);
    } else {
        logger->error("无法初始化音频播放器。");
    }
    exec();
}
