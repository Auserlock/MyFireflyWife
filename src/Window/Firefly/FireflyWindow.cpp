//
// Created by arch_ on 24-10-25.
//
#include "FireflyWindow.h"
#include "RoleProperty.h"
#include "Log.h"
#include <QPixmap>
#include <QTimer>
#include <QMouseEvent>
#include <QApplication>

MainWindow::MainWindow(QApplication* parent)
    : app { parent }
    , main_config { new ConfigFile {} }
    , firefly_property { new Firefly {} }
    , label { this }
{
    bg_image = QString::fromStdString(main_config->get()["current_background_image"]);
    try {
        scaling = main_config->get()["scaling"].get<int>();
        play_voice_on_close = main_config->get()["play_voice_on_close"].get<bool>();
        play_voice_on_start = main_config->get()["play_voice_on_start"].get<bool>();
    } catch (const std::invalid_argument& e) {
        logger->error("main.json 参数类型错误");
        logger->error(e.what());
    } catch (const std::out_of_range& e) {
        logger->error("main.json 参数超出范围");
        logger->error(e.what());
    }

    is_free_walking = false;
    walking_direction = WalkingDirection::left;

    // 初始化 ActionEvent
    action_event_QThread = new Action::ActionEvent { [this]<typename T0>(T0&& PH1) { change_background(std::forward<T0>(PH1)); } };
    Q_ASSERT(connect(action_event_QThread, &Action::ActionEvent::result, this, &MainWindow::actionEventMethod));
    Q_ASSERT(connect(action_event_QThread, &Action::ActionEvent::startActionEventTimerSignal, this, &MainWindow::startTimer));
    Q_ASSERT(connect(action_event_QThread, &Action::ActionEvent::stopActionEventTimerSignal, this, &MainWindow::stopTimer));
    action_event_QThread->start();

    setWindowFlag(Qt::WindowType::WindowStaysOnTopHint);
    setWindowFlag(Qt::WindowType::FramelessWindowHint);
    setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    setWindowTitle("MyFlowingFireflyWife");

    setCentralWidget(&label);
    change_background(bg_image);
}

MainWindow::~MainWindow()
{
    delete main_config;
    delete firefly_property;
    delete action_timer;
    delete action_event_QThread;
}

void MainWindow::change_background(const QString& filePath)
{
    const QPixmap pixMap { filePath };
    int scaledWidth = pixMap.width();
    int scaledHeight = pixMap.height();
    if (scaling > 0) {
        scaledWidth = pixMap.width() / scaling;
        scaledHeight = pixMap.height() / scaling;
    }
    label.setPixmap(pixMap.scaled(scaledWidth, scaledHeight, Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
    bg_image = filePath;
}

void MainWindow::startTimer()
{
    if (!action_timer) {
        action_timer = new QTimer(this);
        Q_ASSERT(connect(action_timer, &QTimer::timeout, action_event_QThread, &Action::ActionEvent::playNextImage));
    }
    action_timer->start(200);
}

void MainWindow::stopTimer() const
{
    if (action_timer) {
        action_timer->stop();
    }
}

void MainWindow::actionEventMethod(const QString& result) const
{
    if (!result.data()) {
        return;
    }
    if (result != "standby" && !is_free_walking) {
        action_event_QThread->standbyEvent();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton) {
        QPoint new_point = this->pos() + event->globalPosition().toPoint() - drag_pos;

        const QRect screenGeometry = QApplication::primaryScreen()->geometry();

        new_point.setX(qMax(screenGeometry.left(), qMin(new_point.x(), screenGeometry.right() - width())));
        new_point.setY(qMax(screenGeometry.top(), qMin(new_point.y(), screenGeometry.bottom() - height())));

        this->move(new_point);
        drag_pos = event->globalPosition().toPoint();
        event->accept();
    }

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        drag_pos = event->globalPosition().toPoint();
        event->accept();

        const int trigger_height = this->height() / 4;
        const QRect trigger_area = QRect(0, 0, this->width(), trigger_height);

        if (trigger_area.contains(event->pos())) {
            action_event_QThread->loveEvent();
        } else {
            action_event_QThread->mentionEvent();
        }
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        event->accept();

        action_event_QThread->standbyEvent();
        is_free_walking = false;
    }

    QMainWindow::mouseReleaseEvent(event);
}