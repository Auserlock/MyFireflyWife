//
// Created by arch_ on 24-10-25.
//
#include "Action.h"
#include <fstream>
#include <filesystem>
#include <QTimer>
#include <utility>
#include "Log.h"

namespace fs = std::filesystem;

Action::ActionPicture::ActionPicture()
{
    std::ifstream actionFile { Action::filePath };
    if (!actionFile.is_open()) {
        logger->error("无法打开文件: " + filePath);
        throw std::runtime_error("无法打开文件: " + filePath);
    }
    if (!(actionFile >> action)) {
        logger->error("读取动作配置失败");
        throw std::runtime_error("读取配置失败");
    }
    actionFile.close();
}

vector<string> Action::ActionPicture::read(const string& key)
{
    if (action.find(key) == action.end()) {
        logger->error("未找到 key: " + key);
        return {};
    }

    string path = action[key].value("path", "");
    string type = action[key].value("type", "");
    if (path.empty() || type.empty()) {
        logger->error("读取 key: " + key);
        return {};
    }

    std::vector<std::string> actions;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry.status()) && entry.path().extension() == type) {
            actions.push_back(entry.path().string());
        }
    }

    std::ranges::sort(actions, [](const std::string& a, const std::string& b) {
        return fs::path(a).filename() < fs::path(b).filename();
    });

    return actions;
}

using ActionEvent = Action::ActionEvent;

ActionEvent::ActionEvent(const function<void(const QString&)>& change_background_func)
{
    init_timer = new QTimer(this);
    Q_ASSERT(connect(init_timer, &QTimer::timeout, this, &ActionEvent::onTimerTimeout));

    action_picture_vec
        = map<string, vector<string>> { { "standby", action_picture.read("standby") },
              { "mention", action_picture.read("mention") },
              { "sleep", action_picture.read("sleep") },
              { "discomfort", action_picture.read("discomfort") },
              { "left", action_picture.read("left") },
              { "right", action_picture.read("right") },
              { "eat", action_picture.read("eat") },
              { "love", action_picture.read("love") } };

    change_background = change_background_func;
    for (auto& picture : action_picture_vec[action_type]) {
        action_event_picture.push_back(picture);
    }
}

void ActionEvent::load(const string& key)
{
    action_type = key;

    action_event_picture = list<string> {};
    for (auto& picture : action_picture_vec[action_type]) {
        action_event_picture.push_back(picture);
    }
}

void ActionEvent::playNextImage()
{
    if (request_interruption)
        return;

    if (action_event_picture.empty()) {
        emit result(action_type.data());
    }

    const string picture_pop = action_event_picture.front();
    action_event_picture.pop_front();
    change_background(picture_pop.data());

    if (action_type != "love" && action_type != "eat") {
        action_event_picture.push_back(picture_pop);
    }

    if (action_type == "left" || action_type == "right") {
        emit result(action_type.data());
    }
}

void ActionEvent::start() const
{
    init_timer->start(1000);
    logger->info("加载中...");
}

void ActionEvent::onTimerTimeout()
{
    init_timer->stop();
    emit startActionEventTimerSignal();
}