#ifndef MAINTASK_H
#define MAINTASK_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QDateTime>
#include <QHostAddress>
#include <QPair>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include "server.h"
#include "database.h"
#include "formsettings.h"

class MainTask : public QObject {
    Q_OBJECT
public:
    MainTask(int argc, char **argv);
    ~MainTask();

    static bool createDir(const QString &aPath);

    int saveFile(File &aFile);
signals:
    void s_finished();

private:
    QSystemTrayIcon *trayIcon_;
    DataBase db_;
    MyServer *server_;
    QList<QPair<QHostAddress, QString>> usersOnline_;

    void initServer();
    void serverRead(QHostAddress aAddress, NetworkData aRequest);
    void serverOnConnect(QHostAddress aAddress);
    void serverOnDisconnect(QHostAddress aAddress);
    QMenu *createTrayIconMenu();
};
#endif // MAINTASK_H
