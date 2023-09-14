#include <QApplication>
#include <Windows.h>
#include <QDir>
#include "loghelper.h"
#include "maintask.h"

QScopedPointer<QFile> logFile_;

void log(QtMsgType aType, const QMessageLogContext &aContext, const QString &aMessage) {
    const char *function = aContext.function ? aContext.function : "";

    QString debug(function);
    if (debug.length() != 0 ) {
        debug += "  ";
    }
    debug += aMessage;
    OutputDebugString(reinterpret_cast<const wchar_t *>(debug.utf16()));

    if (aType == QtDebugMsg) {
        return;
    }

    QTextStream out(logFile_.data());

    out << QDateTime::currentDateTime().toString("hh:mm:ss "); //yyyy-MM-dd hh:mm:ss

    switch (aType) {
    case QtInfoMsg: {
//        QTextStream outI(infoFile_.data());
//        outI << QDateTime::currentDateTime().toString("hh:mm:ss "); //yyyy-MM-dd hh:mm:ss
//        outI << aMessage << Qt::endl;
//        outI.flush();

        break; //out << "INFO "; break;
    }
    case QtDebugMsg: {
        return;
    }
    case QtWarningMsg: {
//        QTextStream outW(errorFile_.data());
//        outW << QDateTime::currentDateTime().toString("hh:mm:ss "); //yyyy-MM-dd hh:mm:ss
//        outW << aMessage << Qt::endl;
//        outW.flush();

        out << "WRNG ";
        break;
    }
    case QtCriticalMsg: {
        out << "CRCL ";
        break;
    }
    case QtFatalMsg: {
        out << "FATL ";
        break;
    }
    default: ;
    }

    if (QString(aContext.category) == "Error") {
        out << "ERROR ";
    }

    out << aMessage << Qt::endl;
    out.flush();

}

void initLog() {
//    //Удаление старых файлов
//    QDir dirLogs("logs/");
//    dirLogs.setFilter(QDir::Files | QDir::NoSymLinks);
//    dirLogs.setSorting(QDir::Name);
//    QFileInfoList list = dirLogs.entryInfoList();
//    for(auto &file: list) {
//        if (file.fileName().indexOf("log_") == 0) {
//            QDateTime date;
//            date = QDateTime::fromString(file.fileName().remove("log_").remove(".txt"), "yyyy.MM.dd");
//            if (date < QDateTime::currentDateTime().addMonths(-2)) {
//                QFile::remove(file.filePath());
//            }
//        }
//    }

    MainTask::createDir("logs/messangerServer/");
    logFile_.reset(new QFile("logs/messangerServer/" + QDateTime::currentDateTime().toString("logu_yyyy.MM.dd") + ".txt"));
    logFile_.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(log);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LogHelper::setDebugMessageHandler();
//    initLog();
    QApplication::setQuitOnLastWindowClosed(false);

    MainTask *task = new MainTask(argc, argv);
    QObject::connect(task, SIGNAL(s_finished()), &a, SLOT(quit()));
//    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
