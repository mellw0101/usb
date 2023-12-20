#ifndef DOCKMANAGER_HPP
#define DOCKMANAGER_HPP

#include <QString>
#include <QMap>
#include <QStringList>
#include <QUuid>
#include <QVector>

class DockManager {
public:
    DockManager() {}

    // Add a process with its sub-processes and their names
    void addProcess(const QString& appName, const QString& mainPID, const QMap<QString, QString>& subPIDs) {
        processes[appName][mainPID] = subPIDs;
    }

    // Get the names of sub-PIDs for a specific main PID of an application
    QMap<QString, QString> getSubPIDs(const QString& appName, const QString& mainPID) const {
        if (processes.contains(appName) && processes[appName].contains(mainPID)) {
            return processes[appName][mainPID];
        }
        return QMap<QString, QString>();
    }

    // Get all main PIDs and their sub-PIDs for an application
    QMap<QString, QMap<QString, QString>> getAppPIDs(const QString& appName) const {
        return processes.value(appName);
    }

    QMap<QString, QMap<QString, QMap<QString, QString>>> DATA() {
        return processes;
    }
    void addToUuidMap(const QString& key, const QUuid& uuid) {
        // Check if the key already exists
        if (uuidMap.contains(key)) {
            // Add the uuid to the existing vector
            uuidMap[key].append(uuid);
        } else {
            // Create a new vector with the uuid and add it to the map
            uuidMap.insert(key, QVector<QUuid>{uuid});
        }
    }
    void addToMap(QMap<QString, QVector<QUuid>>& map, const QString& key, const QUuid& uuid) {
        // Check if the key already exists
        if (map.contains(key)) {
            // Add the uuid to the existing vector
            map[key].append(uuid);
        } else {
            // Create a new vector with the uuid and add it to the map
            map.insert(key, QVector<QUuid>{uuid});
        }
    }

private:
    // Main map structure: AppName -> (MainPID -> (SubPID -> SubPIDName))
    QMap<QString, QMap<QString, QMap<QString, QString>>> processes;
    QMap<QString, QVector<QUuid>> uuidMap;
};

#endif // DOCKMANAGER_HPP
