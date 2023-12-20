#ifndef PIDMANAGER_H
#define PIDMANAGER_H

# ifdef PIDMANAGER_H

    #include "debug.h"
    #include "qcontainerfwd.h"
    #include "qtypes.h"
#   include "DockManager.hpp"

    #include <unistd.h>
    #include <QDragEnterEvent>
    #include <cstdlib>
    #include <dirent.h>
    #include <fstream>
    #include <future>
    #include <iostream>
    #include <spawn.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <cstdio>
    #include <ctime>
    #include <fstream>
    #include <string>
    #include <sys/types.h>
    #include <vector>
    #include <sstream>

# endif

class PidManager {
private:
    QList<qint64> pidList;

public:
    QStringList pidInfo(pid_t pid) {
        QStringList subprocesses = getSubprocesses(pid);

        pid_t parentPid = getParentPid(pid);
        debug(F, __func__, ": ", V, "PPID: ", N, parentPid, ": ", VV, getProcessNameByPid(parentPid), R);
        debug(F, __func__, ": ", V, "PID: ", N, pid, ": ", VV, getProcessNameByPid(pid), R);

        for (const QString &sub : subprocesses) {
            debug(F, __func__, ": ", V, "SUBPID: ", N, sub, ": ", VV, getProcessNameByPid(sub.toInt()), R);
            // pidStatus(sub.toInt());
        }

        return QStringList();
    }
    QStringList getPidByName(const QString& procName) {
        DIR* dir;
        struct dirent* ent;
        std::string path;
        std::string line;
        QStringList subprocesses;
        pid_t pid;
        QStringList prosseses;

        if ((dir = opendir("/proc")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                // Check if the directory is a PID
                if (ent->d_type == DT_DIR) {
                    path = std::string("/proc/") + ent->d_name + "/comm";
                    std::ifstream comm(path.c_str());
                    if (comm.good()) {
                        std::getline(comm, line);
                        if (line == procName.toStdString()) {
                            pid = std::stoi(ent->d_name);
                            findSubprocesses(pid);
                            prosseses << QString::number(pid);
                        }
                    }
                }
            }
            closedir(dir);
        }
        return prosseses;
    }
    pid_t getParentPid(pid_t pid) {
        std::string path = "/proc/" + std::to_string(pid) + "/stat";
        std::ifstream statFile(path.c_str());
        std::string value;
        pid_t ppid = -1;

        if (statFile.good()) {
            // Skip PID and comm fields
            statFile >> value; // PID
            statFile.ignore(std::numeric_limits<std::streamsize>::max(), ')'); // Skip comm
            statFile >> value; // State
            statFile >> ppid; // PPID
        }

        return ppid;
    }
    QString getCorrectProssesName(const QString& launchName) {
        DIR* dir;
        struct dirent* ent;
        std::string path;
        std::string line;

        QStringList parts = launchName.split("-");
        for (int i = 0; i <= parts.size() - 1; i += 1) {
            if ((dir = opendir("/proc")) != NULL) {
                while ((ent = readdir(dir)) != NULL) {
                    // Check if the directory is a PID
                    if (ent->d_type == DT_DIR) {
                        path = std::string("/proc/") + ent->d_name + "/comm";
                        std::ifstream comm(path.c_str());
                        if (comm.good()) {
                            std::getline(comm, line);
                            if (line == parts[i].toStdString()) {
                                return parts[i];
                            }
                        }
                    }
                }
                closedir(dir);
            }
        }
        return QString();
    }
    QStringList getSubprocesses(pid_t parent_pid) {
        QStringList subprocesses;
        DIR* dir;
        struct dirent* entry;
        std::string tasks_path = "/proc/" + std::to_string(parent_pid) + "/task/";

        dir = opendir(tasks_path.c_str());
        if (!dir) {
            perror("opendir");
            return subprocesses;
        }

        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                pid_t pid = std::atoi(entry->d_name);
                if (pid > 0 && pid != parent_pid) { // Exclude the parent pid and '.' or '..' directories
                    subprocesses << QString::number(pid);
                }
            }
        }

        closedir(dir);
        return subprocesses;
    }
    QStringList findSubprocesses(int pid) {
        QStringList subprocesses;
        DIR* procDir = opendir("/proc");
        if (procDir == nullptr) {
            perror("opendir");
            return subprocesses;
        }

        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            int id = atoi(entry->d_name);
            if (id > 0) {
                std::string statPath = std::string("/proc/") + entry->d_name + "/stat";
                std::ifstream statFile(statPath);
                std::string line;
                if (std::getline(statFile, line)) {
                    std::istringstream iss(line);
                    std::vector<std::string> tokens;
                    std::string token;
                    while (std::getline(iss, token, ' ')) {
                        tokens.push_back(token);
                    }
                    if (tokens.size() > 3) {
                        try {
                            // Check if the token is a valid number
                            if (tokens[3].find_first_not_of("0123456789") == std::string::npos) {
                                int ppid = std::stoi(tokens[3]);
                                if (ppid == pid) {
                                    subprocesses.append(QString::number(id));
                                    debug(F, __func__, V, " id:", N, id, " ", VV, getProcessNameByPid(id), R);
                                }
                            }
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Invalid argument: " << e.what() << '\n';
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Out of range: " << e.what() << '\n';
                        }
                    }
                }
            }
        }
        closedir(procDir);
        return subprocesses;
    }
    std::vector<int> searchPIDsForTTY(const std::string& tty) {
        std::vector<int> matchingPIDs;
        DIR* procDir = opendir("/proc");
        struct dirent* entry;

        if (procDir == nullptr) {
            std::cerr << "Failed to open /proc directory" << std::endl;
            return matchingPIDs;
        }

        while ((entry = readdir(procDir)) != nullptr) {
            std::string pidDir = entry->d_name;
            if (pidDir.find_first_not_of("0123456789") == std::string::npos) {
                std::string fdPath = "/proc/" + pidDir + "/fd/";
                DIR* fdDir = opendir(fdPath.c_str());

                if (fdDir != nullptr) {
                    struct dirent* fdEntry;
                    while ((fdEntry = readdir(fdDir)) != nullptr) {
                        std::string fdLink = fdPath + fdEntry->d_name;
                        char linkPath[1024];
                        ssize_t len = readlink(fdLink.c_str(), linkPath, sizeof(linkPath)-1);
                        if (len != -1) {
                            linkPath[len] = '\0';
                            std::string linkPathStr = std::string(linkPath);
                            if (linkPathStr.find(tty) != std::string::npos) {
                                matchingPIDs.push_back(std::stoi(pidDir));
                                int pid = std::stoi(pidDir);
                                debug(F, __func__, R, ": ", tty, ": ", V, "PID:", N, pidDir, VV, getProcessNameByPid(pid), R);
                                break;
                            }
                        }
                    }
                    closedir(fdDir);
                }
            }
        }

        closedir(procDir);
        return matchingPIDs;
    }
    std::vector<std::pair<int, std::string>> getPIDsWithTTYs() {
        std::vector<std::pair<int, std::string>> pidTTYs;
        DIR* procDir = opendir("/proc");
        struct dirent* entry;

        if (procDir == nullptr) {
            std::cerr << "Failed to open /proc directory" << std::endl;
            return pidTTYs;
        }

        while ((entry = readdir(procDir)) != nullptr) {
            std::string pidDir = entry->d_name;
            if (pidDir.find_first_not_of("0123456789") == std::string::npos) {
                std::string fdPath = "/proc/" + pidDir + "/fd/";
                DIR* fdDir = opendir(fdPath.c_str());
                int pid = std::stoi(pidDir);

                if (fdDir != nullptr) {
                    struct dirent* fdEntry;
                    while ((fdEntry = readdir(fdDir)) != nullptr) {
                        std::string fdLink = fdPath + fdEntry->d_name;
                        char linkPath[1024];
                        ssize_t len = readlink(fdLink.c_str(), linkPath, sizeof(linkPath)-1);
                        if (len != -1) {
                            linkPath[len] = '\0';
                            if (strstr(linkPath, "/dev/pts/") != nullptr || strstr(linkPath, "/dev/tty") != nullptr) {
                                pidTTYs.push_back({pid, std::string(linkPath)});
                                debug(F, __func__, ": ", V, "PID:", N, pid, " ",VV, getProcessNameByPid(pid), V, " tty:", VV, std::string(linkPath), R);
                                pidInfo(pid);
                                QStringList subprocesses = findSubprocesses(pid);
                                for (const QString &sub : subprocesses) {
                                    pidInfo(sub.toInt());
                                }

                                break; // Assuming one TTY per PID, break here
                            }
                        }
                    }
                    closedir(fdDir);
                }
            }
        }

        closedir(procDir);
        return pidTTYs;
    }
    QString pidCmdLine(const pid_t& pid) {
        QString line = "/proc/" + QString::number(pid) + "/cmdline";
        std::ifstream file;
        file.open(line.toStdString());
        std::string var;
        std::stringstream buffer;
        while (getline(file, var)) {
            buffer << var << '\n';
        }
        std::string result;
        result = buffer.str();
        debug("pidCmdLine: " + QString::fromStdString(result));
        QString test = QString::fromStdString(result);
        QStringList parts = test.split(" ");
        if (parts.size() == 1) {
            file.close();
            return "mainPid";
        }
        file.close();
        return QString();
    }
    QString pidStatus(const pid_t& pid) {
        QString line = "/proc/" + QString::number(pid) + "/status";
        std::ifstream file;
        file.open(line.toStdString());
        std::string var;
        std::stringstream buffer;
        while (getline(file, var)) {
            buffer << var << '\n';
        }
        std::string result;
        result = buffer.str();
        debug("pidStatus: ", result);
        file.close();
        return QString();
    }
    QString pidFileDescriptors(pid_t pid) {
        QString result = QString();
        std::string path = "/proc/" + std::to_string(pid) + "/fd/";

        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            perror("opendir failed");
            return QString();
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            // Skipping "." and ".." entries
            if (entry->d_name[0] != '.') {
                result += QString::fromStdString(entry->d_name);
            }
        }

        closedir(dir);
        return result;
    }
    QString getProcessNameByPid(pid_t pid) {
        std::string path = "/proc/" + std::to_string(pid) + "/comm";
        std::ifstream commFile(path);
        std::string name;

        if (commFile.good()) {
            std::getline(commFile, name);
            return QString::fromStdString(name);
        } else {
            return "Process not found";
        }
    }
    QStringList getRunningProsseses(const QString& Prossesname){
        QString name = getCorrectProssesName(Prossesname);
        // searchPIDsForTTY("pts");
        // searchPIDsForTTY("tty");
        // getPIDsWithTTYs();
        return getPidByName(name);
    }
    void updateDockManager(DockManager& dockManager, const QString& appName) {
        QStringList mainPIDs = getRunningProsseses(appName);
        for (const QString& mainPID : mainPIDs) {
            QMap<QString, QString> subPIDsWithNames;

            QStringList mainSubPIDs = findSubprocesses(mainPID.toInt());
            for (const QString &mainSubPID : mainSubPIDs) {
                QString mainSubPIDName = getProcessNameByPid(mainSubPID.toInt());
                subPIDsWithNames[mainSubPID] = mainSubPIDName;
            }

            QStringList subPIDs = getSubprocesses(mainPID.toInt());
            for (const QString& subPID : subPIDs) {
                QString subPIDName = getProcessNameByPid(subPID.toInt());
                subPIDsWithNames[subPID] = subPIDName;
            }
            dockManager.addProcess(appName, mainPID, subPIDsWithNames);
        }
    }
};

#endif // PIDMANAGER_H
