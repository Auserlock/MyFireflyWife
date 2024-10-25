//
// Created by arch_ on 24-10-25.
//

#pragma once
#include <string>
#include <list>
#include <vector>
#include <nlohmann/json.hpp>
#include <map>
#include <functional>
#include <QThread>
#include <QTimer>
#include "Log.h"

using nlohmann::json;
using std::function;
using std::list;
using std::map;
using std::string;
using std::vector;

namespace Action {
const std::array<std::string, 8> ActionType = {
    "standby",
    "mention",
    "sleep",
    "discomfort",
    "left",
    "right",
    "eat",
    "love"
};

static string filePath { "./data/config/action.json" };
class ActionPicture {
public:
    ActionPicture();
    vector<string> read(const string& key);

private:
    json action;
};

class ActionEvent final : public QThread {
    Q_OBJECT
signals:
    void result(const QString& str);
    void startActionEventTimerSignal();
    void stopActionEventTimerSignal();

public:
    explicit ActionEvent(const function<void(const QString&)>& change_background_func);
    ~ActionEvent() override { delete init_timer; }
    void start() const;
    void playNextImage();
    void mentionEvent() { load("mention"); }
    void standbyEvent() { load("standby"); }
    void eatEvent() { load("eat"); }
    void sleepEvent() { load("sleep"); }
    void loveEvent() { load("love"); }
    void discomfortEvent() { load("discomfort"); }
    void left() { load("left"); }
    void right() { load("right"); }

private slots:
    void onTimerTimeout();

private:
    ActionPicture action_picture {};
    map<string, vector<string>> action_picture_vec;
    string action_type = "standby";
    function<void(const QString&)> change_background;
    bool request_interruption = false;
    std::list<string> action_event_picture {};

    QTimer* init_timer = nullptr;

    void load(const string& key);
};

}
