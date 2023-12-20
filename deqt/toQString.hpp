#ifndef TOQSTRING_HPP
#define TOQSTRING_HPP

#include "quuid.h"
# ifdef TOQSTRING_HPP

#   include "qtypes.h"
#   include <QString>
#   include <QMap>
#   include <QVector>
#   include <QStringList>
#   include <type_traits>
#   include <QSize>
#   include <QPoint>

# endif

class toQString {
public:
    template <typename T>
    toQString(const T &value) {
        result = convert(value);
    }

    // Conversion operator to QString
    operator QString() const {
        return result;
    }

private:
    QString result;

    static QString convert(const QMap<QString, QVector<QString>>& map) { // Convert function for QMap<QString, QVector<QString>>
        QString str;
        for (const auto& key : map.keys()) {
            str += key + ": ";
            for (const QString& val : map[key]) {
                str += val + "; ";
            }
            str += "\n";
        }
        return str;
    }
    static QString convert(const QMap<QString, QString> &s) {
        QString result = QString();
        for (auto it = s.begin(); it != s.end(); ++it){
            result += "( " + it.key() + " : " + it.value() + " )";
        }
        return result;
    }
    static QString convert(const QMap<QString, QUuid> &s) {
        QString result = QString();
        for (auto it = s.begin(); it != s.end(); ++it){
            result += "( " + it.key() + " : " + it.value().toString(QUuid::WithoutBraces)+ " )";
        }
        return result;
    }
    static QString convert(const QMap<QString, QVector<QUuid>> &s){
        QString result = QString();
        for (auto it = s.begin(); it != s.end(); ++it){
            result += it.key() + "(";
            for (auto uuid : it.value()){
                result += uuid.toString(QUuid::WithoutBraces) + ", ";
            }
        }
        return result;
    }
    static QString QMap_QString_QString(const QString &s, const QString &s2) {
        return "        (" + s + ":" + s2 + ")\n";
    }
    static QString convert(const QMap<QString, QMap<QString, QMap<QString, QString>>> &map) {
        QString result = QString();
        for (auto it = map.begin(); it != map.end(); ++it) {
            result += it.key() + "\n";                          // it.key = appName
            QMap info = it.value();
            for (auto it = info.begin(); it != info.end(); ++it) {
                result += "    " + it.key() + "\n";             // it.key = mainPid
                QMap info2 = it.value();
                for (auto it = info2.begin(); it != info2.end(); ++it){
                    result += QMap_QString_QString(it.key(), it.value());
                }
            }
        }
        return result;
    }
    static QString convert(const bool &s) {
        if (s)
            return "(TRUE)";
        return "(FALSE)";
    }
    static QString convert(const std::vector<std::pair<double, double>> &s) {
        QString result = QString();
        result += "{ ";
        for (const auto&n : s)
            result += "(" + QString::number(n.first) + ", " + QString::number(n.second) + ") ";
        result += "}";
        return result;
    }
    static QString convert(const QSize &s) {
        return "(" + QString::number(s.width()) + ", " + QString::number(s.height()) + ")";
    }
    static QString convert(const QPoint &s) {
        return "(" + QString::number(s.x()) + ", " + QString::number(s.y()) + ")";
    }
    static QString QStringList_to_QString(const QStringList &list) {
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
    static QString convert(const std::string &s) { return QString::fromStdString(s); }
    static QString convert(const QString &s) { return s; }
    static QString convert(const QStringList &s) { return QStringList_to_QString(s); }
    static QString convert(const int &num) { return QString::number(num); }
    static QString convert(const uint &num) { return QString::number(num); }
    static QString convert(const long int &num) { return QString::number(num); }
    static QString convert(const ulong &num) { return QString::number(num); }
    static QString convert(const qlonglong &num) { return QString::number(num); }
    static QString convert(const qulonglong &num) { return QString::number(num); }
    static QString convert(const double &num){ return QString::number(num); }
    static QString convert(const char* s){ return QString::fromStdString(s); }

    template <typename T>
    static QString convert(const T & v) {                    // Fallback convert function for unhandled types
        if (v == 0)
            return "\e[35m";
        if (v == 1)
            return "\e[34m";
        if (v == 2)
            return "\e[32m";
        if (v == 3)
            return "\e[33m";
        if (v == 4)
            return "\e[0m";
        return "__";
        // return "__no__";
    }
};

#endif // TOQSTRING_HPP
