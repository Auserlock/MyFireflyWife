//
// Created by arch_ on 24-10-26.
//

#pragma once
#include "Media.h"
#include <nlohmann/json.hpp>
#include <utility>
#include "Log.h"
using nlohmann::json;
const string FIREFLY_VOICE = "data/config/voice.json";
class FireflyVoice {
public:
    json& getAllVoices();
    json getVoice(const string& key) { return voices.contains(key) ? voices[key] : json(); }
    static QString getTimeOfDay();

private:
    json voices;
};

class FireflyVoiceQThread final : public QThread {
    Q_OBJECT
signals:
    void finishPlay(int);
    void startPlay(std::string);

public:
    explicit FireflyVoiceQThread(const string& key_ = "");
    ~FireflyVoiceQThread() override
    {
        delete voice;
        delete audioPlayer;
    }
    void run() override;

private:
    string key;
    FireflyVoice* voice;
    AudioPlayer* audioPlayer;
};