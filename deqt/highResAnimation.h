#ifndef HIGHRESANIMATION_H
#define HIGHRESANIMATION_H

#include "debug.h"
#include "qobject.h"
#include "qwindowdefs.h"
#include <QAbstractAnimation>
#include <chrono>
#include <functional>
#include <QTimer>
#include <vector>

class HighResAnimation : public QAbstractAnimation {
    Q_OBJECT
    QTimer *highFreqTimer;
private:
    int durationMs = 0;
    int numKeyframes = 0;
    int startKey = 0;
    int endKey = 0;
    bool isNgative;
    std::function<void(int)> onKeyframe;
    int lastKeyframeIndex = -1;
    std::chrono::high_resolution_clock::time_point startTime;
    int lastSignalTime = -1; // To track the last time a signal was emitted
    int keyframeInterval = durationMs / (numKeyframes - 1);

    int controlCode;

public:
    //  std::function<void(int)> keyframeCallback,onKeyframe(keyframeCallback)
    // HighResAnimation(int duration, const std::vector<int>& keyframes, QObject *parent = nullptr);
    HighResAnimation(QObject *parent)
    : QAbstractAnimation(parent), durationMs(100), numKeyframes(), startKey(), endKey() {
        // Initialize the timer
        highFreqTimer = new QTimer(this);
        highFreqTimer->setInterval(1);// 1 millisecond


        connect(highFreqTimer, &QTimer::timeout, this, [this]() {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            int keyframeInterval = durationMs / (numKeyframes - 1);
            int keyframeIndex = elapsed / keyframeInterval;
            // Check if the keyframe index has changed since the last signal emission
            if (keyframeIndex != lastKeyframeIndex && keyframeIndex < numKeyframes && keyframeIndex >= 0) {
                lastKeyframeIndex = keyframeIndex; // Update the last keyframe index
                emit highFreqSignal(controlCode);
            }
        });
        lastKeyframeIndex = -1;
    }
    void updateCurrentTime(int currentTime) override;
    void resetTimer() {
        startTime = std::chrono::high_resolution_clock::now();
        lastSignalTime = -1; // Resetting any other related variables
        // controlCode = -1;
            // Stop the high-frequency timer
    }
    void stopHighFreqTimer() {
        highFreqTimer->stop();
    }
    void start(QAbstractAnimation::DeletionPolicy policy = KeepWhenStopped) {
        QAbstractAnimation::start(policy);
        highFreqTimer->start();   // Start the high-frequency timer
        startTime = std::chrono::high_resolution_clock::now();  // Reset the start time
        lastKeyframeIndex = -1;   // Reset the last keyframe index
    }
    void stop() {
        QAbstractAnimation::stop();
        highFreqTimer->stop();
    }
    int duration() const override {
        return durationMs;
    }
    void setDuration(int num) {
        durationMs = num;
    }
    void setStartKey(int num) {
        startKey = num;
    }
    void setEndKey(int num) {
        endKey = num;
    }
    void setNumKeys(int num) {
        numKeyframes = num;
    }
    void setNegative(bool b){
        if (b){
            isNgative = true;
        }
        isNgative = false;
    }
    void setControlCode(int num){
        controlCode = num;
    }

signals:
    void keyFrameSignal(int keyframeIndex);
    void highFreqSignal(int value);


};

#endif // HIGHRESANIMATION_H
