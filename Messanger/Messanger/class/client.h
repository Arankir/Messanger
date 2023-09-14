#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

struct NetworkData {
    NetworkData() {;}
    NetworkData(const QString&, const QJsonObject & = QJsonObject());

    QString requestType;
    QJsonValue value;

    QJsonObject toJson() const;
    static NetworkData fromJson(const QJsonObject &);
};

class MyClient : public QTcpSocket {
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = nullptr);
    bool connect(const QHostAddress &, quint16 port);
    void read();
    bool writeData(const NetworkData &data);

signals:
    void s_read(NetworkData);

public slots:
    bool reconnect();

private:
    QByteArray buffer_;
    QDateTime lastUpdate_;
    QHostAddress host_;
    quint16 port_;

};
#endif // CLIENT_H
