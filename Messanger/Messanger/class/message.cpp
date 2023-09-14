#include "message.h"
#include <QBuffer>

long long int Message::createLocalVcode() {
    static long long int localVcode = 0;
    return ++localVcode;
}

Message::Message(): localVcode(createLocalVcode()) {

}

QJsonObject Message::toJson() const {
    QJsonObject obj;
    obj["message"] = message;
    obj["from"] = sendFrom;
    switch (type) {
    case ReceiverType::account: {
        obj["account"] = sendTo;
        obj["chat"] = "";
        break;
    }
    case ReceiverType::group: {
        obj["account"] = "";
        obj["chat"] = sendTo;
        break;
    }
    default: {

    }
    }
    obj["cdate"] = cdate.toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["readed"] = readed;
    obj["peoples"] = readers;
    obj["vcode"] = vcode;
    obj["localVcode"] = QString::number(localVcode);
    obj["status"] = static_cast<int>(status);

    QJsonArray filesArray;
    int i = 0;
    for (const auto &file: files) {
        filesArray.append(file.toJson());
        ++i;
        if (i > 9) {
            break;
        }
    }
    obj["files"] = filesArray;

    QString sImage;
    if (image.isNull()) {
        sImage = "NULL";
    } else {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        sImage = "\"" + QString::fromUtf8(buffer.data().toBase64()) + "\"";
    }
    obj["image"] = sImage;
    return obj;
}

Message Message::fromJson(QJsonObject aObject) {
    Message mMessage;
    mMessage.message = aObject.value("message").toString();
    mMessage.sendFrom = aObject.value("from").toString();
    if (!aObject.value("account").toString().isEmpty() && aObject.value("chat").toString().isEmpty()) {
        mMessage.sendTo = aObject.value("account").toString();
        mMessage.type = ReceiverType::account;
    }
    if (aObject.value("account").toString().isEmpty() && !aObject.value("chat").toString().isEmpty()) {
        mMessage.sendTo = aObject.value("chat").toString();
        mMessage.type = ReceiverType::group;
    }
    mMessage.image.loadFromData(QByteArray::fromBase64(aObject.value("image").toString().toUtf8()), "PNG");
    mMessage.cdate = QDateTime::fromString(aObject.value("cdate").toString(), "yyyy.MM.dd hh:mm:ss.zzz");
    mMessage.readed = aObject.value("readed").toInt();
    mMessage.readers = aObject.value("peoples").toInt();
    auto filesArray = aObject.value("files").toArray();
    QList<File> files;
    for (auto file: filesArray) {
        files << File::fromJson(file.toObject());
    }
    mMessage.files = files;
    mMessage.status = static_cast<MessageStatus>(aObject.value("status").toInt());
    if (mMessage.readed >= mMessage.readers) {
        mMessage.status = MessageStatus::readedAll;
    } else if (mMessage.readed <= 0) {
        mMessage.status = MessageStatus::successOnServer;
    } else if (mMessage.readed > 0) {
        mMessage.status = MessageStatus::readedOnce;
    } else {
        mMessage.status = MessageStatus::unknown;
    }
    mMessage.vcode = aObject.value("vcode").toString();
    long long int localVcode = aObject.value("localVcode").toString().toLongLong();
    if (localVcode > 0) {
        mMessage.localVcode = localVcode;
    } else {
        mMessage.localVcode = createLocalVcode();
    }
    return mMessage;
}

long long Message::getLocalVcode() const {
    return localVcode;
}

File::File(): localVcode(createLocalVcode()) {

}

File::File(const QString &aFileName): File() {
    QFile fFile(aFileName);
    QFileInfo info(fFile);
    originalFilePath = aFileName;
    fileName = info.fileName();
    baseName = info.baseName();
    suffix = info.suffix();
    birthDate = info.birthTime();
    modifiedDate = info.lastModified();
    readDate = info.lastRead();
}

QJsonObject File::toJson() const {
    QFile file(originalFilePath);
    QByteArray ba;
    if (file.open(QIODevice::ReadOnly)) {
        ba = file.readAll();
//        qDebug() << ba;
        file.close();
    }
    QFileInfo info(file);
    QJsonObject obj;
    obj["originalFilePath"] = originalFilePath;
    obj["file"] = QString::fromLocal8Bit(ba);
    obj["fileName"] = info.fileName();
    obj["baseName"] = info.baseName();
    obj["suffix"] = info.suffix();
    obj["birthDate"] = info.birthTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["modifiedDate"] = info.lastModified().toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["readDate"] = info.lastRead().toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["fileSize"] = file.size();
    obj["localVcode"] = QString::number(localVcode);
    obj["vcode"] = vcode;
    return obj;
}

File File::fromJson(QJsonObject aObject) {
    File file;
    QString originalFilePath = aObject.value("originalFilePath").toString();
    if (!originalFilePath.isEmpty() && QFileInfo::exists(originalFilePath)) {
        QFile fFile(originalFilePath);
        QFileInfo info(fFile);
        file.originalFilePath = originalFilePath;
        file.fileName = info.fileName();
        file.baseName = info.baseName();
        file.suffix = info.suffix();
        file.birthDate = info.birthTime();
        file.modifiedDate = info.lastModified();
        file.readDate = info.lastRead();
    } else {
        file.fileName = aObject.value("fileName").toString();
        file.baseName = aObject.value("baseName").toString();
        file.suffix = aObject.value("suffix").toString();
        file.birthDate = QDateTime::fromString(aObject.value("birthDate").toString(), "yyyy.MM.dd hh:mm:ss.zzz");
        file.modifiedDate = QDateTime::fromString(aObject.value("modifiedDate").toString(), "yyyy.MM.dd hh:mm:ss.zzz");
        file.readDate = QDateTime::fromString(aObject.value("readDate").toString(), "yyyy.MM.dd hh:mm:ss.zzz");;
    }
    file.vcode = aObject.value("vcode").toString();
    long long int localVcode = aObject.value("localVcode").toString().toLongLong();
    if (localVcode > 0) {
        file.localVcode = localVcode;
    } else {
        file.localVcode = createLocalVcode();
    }
    return file;
}

long long File::getLocalVcode() const {
    return localVcode;
}

long long File::createLocalVcode() {
    static long long int localVcode = 0;
    return ++localVcode;
}
