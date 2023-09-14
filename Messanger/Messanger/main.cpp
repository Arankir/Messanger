#include "formauth.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStyleFactory>
#include <QTranslator>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

QScopedPointer<QFile> logFile_;

void initLog();
void onCrush();
void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);

bool createDir(const QString &aPath) {
    bool exist = true;
    QString path = aPath;
    path.replace("\\", "/");
    QStringList dirs = path.split("/");
    if (dirs.last() != "") {
        dirs.removeLast();
    }
    QString pathNow = "";
    for (auto &dir: dirs) {
        pathNow += std::move(dir) + "/";
        exist = (QDir().mkdir(pathNow) && exist);
    }
    return exist;
}

int main(int argc, char *argv[]) {
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Territory::Russia));
    QApplication a(argc, argv);
    initLog();
    FormAuth w;
    w.show();
    return a.exec();
}

void log(QtMsgType aType, const QMessageLogContext &aContext, const QString &aMessage) {
    const char *function = aContext.function ? aContext.function : "";
    QTextStream out(logFile_.data());

    switch (aType) {
    case QtInfoMsg:     out << "INFO "; break;
    case QtDebugMsg:    out << "DEBG "; break;
    case QtWarningMsg:  out << "WRNG "; break;
    case QtCriticalMsg: out << "CRCL "; break;
    case QtFatalMsg:    out << "FATL "; break;
    }

    out << QTime::currentTime().toString("hh:mm:ss ");//"yyyy-MM-dd hh:mm:ss "
    out << /*aContext.category << ": " << */function << "  " << aMessage << Qt::endl;
    out.flush();

    QString output(function);
    output += "  " + aMessage + "\n";
    OutputDebugString(reinterpret_cast<const wchar_t *>(output.utf16()));
}

void initLog() {
    QString dateFormat = "yyyy.MM.dd";
    QString logsPath = QDir::currentPath() + "/logs/";
    createDir(logsPath);

    //Удаление старых файлов
    QDir dirLogs(logsPath);
    dirLogs.setFilter(QDir::Files | QDir::NoSymLinks);
    dirLogs.setSorting(QDir::Name);
    QFileInfoList list = dirLogs.entryInfoList();
    for(auto &file: list) {
        if (file.fileName().indexOf("log_") == 0) {
            QDateTime date {QDateTime::fromString(file.fileName().remove("log_").remove(".txt"), dateFormat)};
            if (date < QDateTime::currentDateTime().addMonths(-1)) {
                QFile::remove(file.filePath() + "/" + file.fileName());
            }
        }
    }

    logFile_.reset(new QFile(logsPath + QDateTime::currentDateTime().toString("log_" + dateFormat) + ".txt"));
    logFile_.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(log);
}

void onCrush() {

}
