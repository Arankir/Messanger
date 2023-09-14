#include "client.h"
#include <QHostAddress>
#include <QDataStream>
#include <QDateTime>

MyClient::MyClient(QObject *parent) : QTcpSocket(parent) {
    QObject::connect(this, &QTcpSocket::readyRead, this, &MyClient::read);
}

bool MyClient::connect(const QHostAddress &host, quint16 port) {
    host_ = host;
    port_ = port;
    this->connectToHost(host_, port_);
    qInfo() << "connecting" << host_;
    return this->waitForConnected();
}

bool MyClient::reconnect() {
    this->connectToHost(host_, port_);
    qInfo() << "reconnecting" << host_;
    return this->waitForConnected();
}

void MyClient::read() {
    if (lastUpdate_.addSecs(5) < QDateTime::currentDateTime()) {
        buffer_.clear();
    }

    buffer_ += this->readAll();
    lastUpdate_ = QDateTime::currentDateTime();

    if (QJsonDocument::fromJson(buffer_).isNull()) {
        return;
    }
//    qDebug() << 1 << array;
    NetworkData rtc = NetworkData::fromJson(QJsonDocument::fromJson(buffer_).object());
    buffer_.clear();
//    qDebug() << QString(array);
    emit s_read(rtc);
}

QByteArray intToArray(qint32 source) {//Use qint32 to ensure that the number have 4 bytes
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

bool MyClient::writeData(const NetworkData &data) {
//    qDebug() << "state" << this->state();
    if(this->state() == QAbstractSocket::ConnectedState) {
//        this->write(intToArray(data.size())); //write size of data
//        this->write(QJsonDocument(data.toJson()).toJson()); //write the data itself
//        return this->waitForBytesWritten();
//        qDebug() << intToArray(QJsonDocument(data.toJson()).toJson().size()) << QJsonDocument(data.toJson()).toJson();
        this->write(intToArray(QJsonDocument(data.toJson()).toJson().size())); //write size of data
        this->write(QJsonDocument(data.toJson()).toJson()); //write the data itself
        return this->waitForBytesWritten();
    } else
        return false;
}

NetworkData::NetworkData(const QString &aRequestType, const QJsonObject &aJObject): requestType(aRequestType), value(aJObject) {}

QJsonObject NetworkData::toJson() const {
    QJsonObject object;
    object["type"] = this->requestType;
    object["value"] = this->value;
    return object;
}

NetworkData NetworkData::fromJson(const QJsonObject &aObject) {
    NetworkData reply;
    reply.requestType = aObject.value("type").toString();
    reply.value = aObject.value("value");
    return reply;
}
