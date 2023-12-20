#include "dockConfig.h"
// #include "debug.h"
#include "qsize.h"
#include "qwidget.h"
#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <variant>
#include <string>
#include <functional>
#include <map>

namespace VARS {
    int iconSize;
    int buttonIconDiffrence;
    int buttonSize;
    int squeezeNum;

    int totalWidth;
    int totalButtons;

    int zoomAnimDuration;
    int scaleFactor;
    float floatScaleFactor;

    int bounceAnimDuration;
    int bounceIconSize;

    const std::string dockAppOrderFile = "/home/mellw/.dockAppOrder";

    char* test;
}
void initializeVars() {
    VARS::iconSize = 48;
    VARS::buttonIconDiffrence = 22;
    VARS::squeezeNum = 0;

    VARS::zoomAnimDuration = 60;
    VARS::scaleFactor = 120;
    VARS::floatScaleFactor = VARS::scaleFactor / 100.0f;

    VARS::bounceAnimDuration = 190;
    VARS::bounceIconSize = 8;
    VARS::buttonSize = 48 /*(((VARS::iconSize * VARS::scaleFactor) / 100) + 1) + VARS::bounceIconSize*/ /*VARS::iconSize + VARS::buttonIconDiffrence*/;
}
const std::string dockFile(File config){
    switch (config) {
        case APP_ORDER:
            return VARS::dockAppOrderFile;
            break;
    }
    return "";
}
using FunctionReturn = std::variant<int, std::string, double, bool>;

std::map<Buttons::GET, std::function<FunctionReturn()>> functionMap {
     {Buttons::GET::SIZE, []() { return num(BUTTON_SIZE); }},
};

int dock(Buttons::GET action) {
    auto it = functionMap.find(action);
    if (it != functionMap.end()) {
        FunctionReturn result = it->second(); // Call the function
        if (auto intValue = std::get_if<int>(&result)) {
            return *intValue;
        }
    }
    return -1;
}


// FunctionReturn dock(Buttons config) {
//     auto it = functionMap.find(config);
//     if (it != functionMap.end()) {
//         return it->second(); // Call the function and return its result
//     }
//     // Handle the case where the action is not found
//     return {}; // Return an empty variant
// }
int num(Vars config) {
    switch (config) {
        case ICON_SIZE:
            return VARS::iconSize;
        case BUTTON_SIZE:
            return VARS::buttonSize;
        case BUTTON_ICON_DIFFRENCE:
            return VARS::buttonIconDiffrence;
        case TOTAL_WIDTH:
            return VARS::totalWidth;
        case ZOOM_ANIM_DUR:
            return VARS::zoomAnimDuration;
        case BOUNCE_ANIM_DUR:
            return VARS::bounceAnimDuration;
        case BOUNCE_ICON_SIZE:
            return VARS::bounceIconSize;
        case SQUEEZE_NUM:
            return VARS::squeezeNum;
        case SCALE_FACTOR:
            return VARS::scaleFactor;
        case TOTAL_BUTTONS:
            return VARS::totalButtons;
    }
    return 0;
}
void changeNum(Vars config, const int num) {
    switch (config) {
        case ICON_SIZE:
            VARS::iconSize = num;
            // VARS::buttonSize = num + VARS::buttonIconDiffrence;
            break;
        case BUTTON_SIZE:
            VARS::buttonSize = num;
            VARS::buttonIconDiffrence = VARS::buttonSize - VARS::iconSize;
            break;
        case BUTTON_ICON_DIFFRENCE:
            VARS::buttonIconDiffrence = num;
            break;
        case TOTAL_WIDTH:
            VARS::totalWidth = num;
            break;
        case ZOOM_ANIM_DUR:
            VARS::zoomAnimDuration = num;
            break;
        case BOUNCE_ANIM_DUR:
            VARS::bounceAnimDuration = num;
            break;
        case BOUNCE_ICON_SIZE:
            VARS::bounceIconSize = num;
            // VARS::buttonSize = (((VARS::iconSize * VARS::scaleFactor) / 100) + 1) + VARS::bounceIconSize;
            break;
        case SQUEEZE_NUM:
            VARS::squeezeNum = num;
            break;
        case SCALE_FACTOR:
            VARS::scaleFactor = num;
            VARS::floatScaleFactor = num / 100.0f;
            break;
        case TOTAL_BUTTONS:
            VARS::totalButtons = num;
            break;
    }
}
QSize Qnum(Vars config){
    switch (config) {
        case BUTTON_SIZE:
            return QSize(num(BUTTON_SIZE), num(BUTTON_SIZE));
        case ICON_SIZE:
            return QSize(num(ICON_SIZE), num(ICON_SIZE));
        case BUTTON_ICON_DIFFRENCE:
        case TOTAL_WIDTH:
        case ZOOM_ANIM_DUR:
        case BOUNCE_ANIM_DUR:
        case BOUNCE_ICON_SIZE:
            return QSize(num(BOUNCE_ICON_SIZE), num(BOUNCE_ICON_SIZE));
        case SQUEEZE_NUM:
        case SCALE_FACTOR:
        default:
            break;
    }
    return QSize();
}
double doubleNum(Float config) {
    switch (config) {
        case FLOAT_SCALE_FACTOR:
            return VARS::scaleFactor / 100.0f;
    }
    return 0;
}
double calculate(Calculate config) {
    double dif = (num(BUTTON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR)) - num(BUTTON_SIZE);
    double steps = static_cast<int>(std::round(dif));
    double timeForEachStep = num(ZOOM_ANIM_DUR) / steps;
    switch (config) {
        case SCALE_STEPS:
            return steps;
        case TIME_PER_STEP:
            return timeForEachStep;
        case DIFF:
            return dif;
    }
    return 0;
}





