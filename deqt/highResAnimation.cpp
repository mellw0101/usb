#include "highResAnimation.h"
#include "debug.h"

#include <QAbstractAnimation>
#include <chrono>
#include <complex>
#include <functional>
#include "qwindowdefs.h"
#include <QTime>
#include <QTimer>

// HighResAnimation::HighResAnimation(int duration, const std::vector<int>& keyframes, QObject *parent)
//     : QAbstractAnimation(parent), keyframes(keyframes) {
//         // Constructor code
//     }

// HighResAnimation::HighResAnimation(int duration, int numKeyframes, std::function<void(int)> keyframeCallback, QObject *parent)
//     : QAbstractAnimation(parent), durationMs(duration), numKeyframes(numKeyframes), onKeyframe(keyframeCallback) {
//         // Initialize the timer
//         highFreqTimer = new QTimer(this);
//         highFreqTimer->setInterval(1);                      // 1 millisecond
//
//         connect(highFreqTimer, &QTimer::timeout, this, [this, numKeyframes]() {
//             auto now = std::chrono::high_resolution_clock::now();
//             auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
//
//             // Calculate the current keyframe index based on elapsed time
//             int keyframeIndex = elapsed / keyframeInterval;
//
//             // Check if the keyframe index has changed since the last signal emission
//             if (keyframeIndex != lastKeyframeIndex && keyframeIndex < numKeyframes) {
//                 lastKeyframeIndex = keyframeIndex; // Update the last keyframe index
//                 emit highFreqSignal(keyframeIndex); // Emit the signal with the keyframe index
//             }
//         });
//
//
//         lastKeyframeIndex = -1;
//         // highFreqTimer->start();
//         // startTime = std::chrono::high_resolution_clock::now();
//     }

void HighResAnimation::updateCurrentTime(int currentTime) {
    // if (onKeyframe) {
    // }
        // highFreqTimer->start();
        // startTime = std::chrono::high_resolution_clock::now();
}
