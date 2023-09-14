#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QImage>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include "class/chatreceiver.h"

struct File {
    File();
    File(const QString &fileName);
    QString originalFilePath;
    QString fileName;
    QString baseName;
    QString suffix;
    QDateTime birthDate;
    QDateTime modifiedDate;
    QDateTime readDate;
    int fileSize;
    QString vcode;

    QJsonObject toJson() const;
    static File fromJson(QJsonObject);
    friend QDebug operator<<(QDebug dbg, const File &a) {
        dbg.nospace() << "File(" << a.toJson() << ")\n";
        return dbg.space();
    }

    long long getLocalVcode() const;

private:
    long long int localVcode;
    static long long int createLocalVcode();
};

enum class MessageStatus {
    sendToServer,
    successOnServer,
    errorOnServer,
    readedOnce,
    readedAll,
    unknown
};

struct Message {
    Message();
    QString message;
    QString sendFrom;
    QString sendTo;
    ReceiverType type;
    QDateTime cdate;
    int readed;
    int readers;
    QString vcode;
    QImage image;
    QList<File> files;

    MessageStatus status;

    QJsonObject toJson() const;
    static Message fromJson(QJsonObject);
    friend QDebug operator<<(QDebug dbg, const Message &a) {
        dbg.nospace() << "Message(" << a.toJson() << ")\n";
        return dbg.space();
    }

    long long getLocalVcode() const;

private:
    long long int localVcode;
    static long long createLocalVcode();
};

#endif // MESSAGE_H
