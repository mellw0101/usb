#ifndef DPM_HPP
#define DPM_HPP

# ifdef DPM_HPP

#   include "debug.h"

#   include <QProcess>
#   include <QProcessEnvironment>
#   include <QMap>
#   include <QVector>
#   include <QStringList>
#   include <QUuid>

# endif

class PMan : public QObject {
    Q_OBJECT

public:
    PMan(const QString& program, const QStringList& arguments = QStringList(), QObject *parent = nullptr)
        : QObject(parent), uuid(QUuid::createUuid()) {
        process = new QProcess(this);
        connect(process, &QProcess::started, this, &PMan::onProcessStarted);
        connect(process, &QProcess::stateChanged, this, &PMan::onProcessStateChanged);

        connect(process, &QProcess::finished, this, &PMan::onProcessFinished);
        connect(process, &QProcess::errorOccurred, this, &PMan::onProcessErrorOccurred);
        connect(process, &QProcess::readyReadStandardOutput, this, &PMan::onProcessReadyReadStandardOutput);
        connect(process, &QProcess::readyReadStandardError, this, &PMan::onProcessReadyReadStandardError);

        process->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        process->start(program, arguments);
    }

    ~PMan() {
        delete process;
    }

    QUuid getUuid() const {
        return uuid;
    }

    QProcess* process;
signals:
    void processStarted(const QUuid& uuid);
    void processStateChanged(const QUuid& uuid, QProcess::ProcessState newState);

    void processFinished(const QUuid& uuid, int exitCode);
    void processErrorOccurred(const QUuid& uuid, QProcess::ProcessError error);
    void processReadyReadStandardOutput(const QUuid& uuid);
    void processReadyReadStandardError(const QUuid& uuid);

private slots:
    void onProcessStarted() {
        emit processStarted(uuid);
    }

    void onProcessStateChanged(QProcess::ProcessState newState) {
        emit processStateChanged(uuid, newState);
    }

    void onProcessFinished(int exitCode) {
        emit processFinished(uuid, exitCode);
    }

    void onProcessErrorOccurred(QProcess::ProcessError error) {
        emit processErrorOccurred(uuid, error);
    }

    void onProcessReadyReadStandardOutput() {
        emit processReadyReadStandardOutput(uuid);
    }

    void onProcessReadyReadStandardError() {
        emit processReadyReadStandardError(uuid);
    }

private:
    QUuid uuid;  // UUID for this process
};
// PMan* processManager = new PMan(app, QStringList());
        //
        // QObject::connect(processManager, &PMan::processStarted, [processManager](const QUuid& uuid){
        //     qDebug() << "Process started with UUID:" << uuid.toString();
        // });
        //
        // // Connection for processStateChanged signal
        // QObject::connect(processManager, &PMan::processStateChanged, [processManager](const QUuid& uuid, QProcess::ProcessState newState){
        //     qDebug() << "Process state changed for UUID:" << uuid.toString() << ", New State:" << newState;
        // });
        //
        // // Connection for processFinished signal
        // QObject::connect(processManager, &PMan::processFinished, [processManager](const QUuid& uuid, int exitCode){
        //     qDebug() << "Process with UUID:" << uuid.toString() << " finished with exit code:" << exitCode;
        // });
        //
        // // Connection for processErrorOccurred signal
        // QObject::connect(processManager, &PMan::processErrorOccurred, [processManager](const QUuid& uuid, QProcess::ProcessError error){
        //     qDebug() << "Process with UUID:" << uuid.toString() << " encountered an error:" << error;
        // });
        //
        // // Connection for processReadyReadStandardOutput signal
        // QObject::connect(processManager, &PMan::processReadyReadStandardOutput, [processManager](const QUuid& uuid){
        //     qDebug() << "Process with UUID:" << uuid.toString() << " has new standard output data.";
        //     // Optionally read the data
        //     QByteArray data = processManager->process->readAllStandardOutput();
        //     qDebug() << data;
        // });
        //
        // // Connection for processReadyReadStandardError signal
        // QObject::connect(processManager, &PMan::processReadyReadStandardError, [processManager](const QUuid& uuid){
        //     qDebug() << "Process with UUID:" << uuid.toString() << " has new standard error data.";
        //     // Optionally read the data
        //     QByteArray data = processManager->process->readAllStandardError();
        //     qDebug() << data;
        // });

class dPM : public QProcess {
    Q_OBJECT

public:
    dPM(const QString& program, const QStringList& arguments = QStringList(), QObject *parent = nullptr)
        : QProcess(parent) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        QString HOME = getenv("HOME");
        env.insert("PWD", HOME);
        env.insert("QT_QPA_PLATFORM", "wayland");

        this->setProcessEnvironment(env);

        // Start the process
        this->start(program, arguments);
    }

    static QUuid process(const QString& program, const QStringList& arguments = QStringList()) {
        QUuid uuid = QUuid::createUuid();
        dPM* newProcess = new dPM(program, arguments);
        processes.insert(uuid, newProcess);
        return uuid;
    }
    // // Later, use the UUID to get the process and send commands
    // QProcess* process = manager.getProcess(uuid);
    // if (process) {
    //     // Send commands to the process
    //     process->write("command\n");
    // }
    static QProcess* getProcess(const QUuid& uuid) {
        return processes.value(uuid, nullptr);
    }
    static void closeProcess(const QUuid& uuid) {
        QProcess* process = processes.value(uuid);
        if (process) {
            process->terminate();  // Or use process->kill() if necessary
            process->waitForFinished();  // Optionally wait for the process to finish
        }
    }
private:
    static QMap<QUuid, QProcess*> processes;
};

#endif                                                      // DPM_HPP
