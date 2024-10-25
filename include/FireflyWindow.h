//
// Created by arch_ on 24-10-25.
//

#pragma once
#include <QMainWindow>
#include <QWidget>
#include "RoleProperty.h"
#include "Action.h"
#include "FireflyWindowConfig.h"
#include <QLabel>

enum class WalkingDirection {
    left,
    right
};

class MainWindow final : public QMainWindow {
    Q_OBJECT
signals:
    void startActionEventTimerSignal();
    void stopActionEventTimerSignal();

public:
    explicit MainWindow(QApplication* parent = nullptr);
    ~MainWindow() override;
    void change_background(const QString& filePath);
    void startTimer();
    void stopTimer() const;
    void actionEventMethod(const QString& result) const;

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QApplication* app;
    ConfigFile* main_config;
    Firefly* firefly_property;
    QString bg_image;
    int scaling;
    bool play_voice_on_close, play_voice_on_start;
    bool is_free_walking;
    WalkingDirection walking_direction;
    QLabel label;
    QPoint drag_pos {};
    QTimer* action_timer = nullptr;

    Action::ActionEvent* action_event_QThread {};
};
