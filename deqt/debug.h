#ifndef DEBUG_H
#define DEBUG_H

// #include "dockConfig.h"
// #include "dPM.hpp"
# include "toQString.hpp"

#include "qcontainerfwd.h"
#include "qpoint.h"
#include <QDebug>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <QString>

namespace DEBUG {
    void variableValue(const QString functionName, const QString variableName, const QString variableValue);
    void output(const QString& string);
    QString QStringList_to_QString(const QStringList list);
    QString blue(const QString& string);
    std::string constCharToStdString(char*const& string);
}

enum COLOR {
    F = 0,
    V = 1,
    VV = 2,
    N = 3,
    R = 4
};

void debugPrint(const QStringList& list);

// Recursive function to collect arguments
template<typename T, typename... Args>
void debugImpl(QStringList& list, const T& first, const Args&... args) {
    list.append(toQString(first));
    if constexpr (sizeof...(args) == 0) {
        // End of recursion, call debugPrint
        debugPrint(list);
    } else {
        // Continue recursion
        debugImpl(list, args...);
    }
}
template<typename T, typename... Args>
void debug(const T& first, const Args&... args) {
    QStringList list;
    debugImpl(list, first, args...);
}

#endif // DEBUG_H
