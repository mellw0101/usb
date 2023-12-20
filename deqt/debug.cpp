#include "debug.h"
#include <string>
#include <QSize>

namespace DEBUG {
    void variableValue(const QString functionName, const QString variableName, const QString variableValue) {
        QString line = functionName + ": " + variableName + ": " + variableValue;
        std::string line_string = line.toStdString();
        const char* line_char = line_string.c_str();
        qDebug("%s", line_char);
    }
    void output(const QString& string) {
        QString line = string;
        std::string line_string = line.toStdString();
        const char* line_char = line_string.c_str();
        qDebug("%s", line_char);
    }
    QString QStringList_to_QString(const QStringList list) {
        if (list.empty())
            return QString();
        QString result;
        result += "(";
        for (int i = 0; i <= list.size() - 1; i += 1) {
            if (i == list.size() - 1)
                result += list[i];
            else
                result += list[i] + ", ";
        }
        result += ")";
        return result;
    }
    QString blue(const QString& string) {
        return "\e[34m" + string + "\e[0m";
    }
    std::string constCharToStdString(char*const& string) {
        std::string stdString = string;
        return stdString;
    }
}

// Helper function for final printing
void debugPrint(const QStringList& list) {
    QString qString = list.join("");
    std::string stdString = qString.toStdString();
    const char* charString = stdString.c_str();
    qDebug("%s", charString);
}


