#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

struct NetworkData {
    NetworkData() {;}
    NetworkData(const QString&, const QJsonObject & = QJsonObject());
    QString type;
    QJsonValue value;

    QJsonObject toJson() const;
    static NetworkData fromJson(const QJsonObject &);
};

class MyServer : public QObject {
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);

    void start(quint16 port);
public slots:
    void slotNewConnection();
    void slotServerRead();
    int slotWrite(const QHostAddress &aAddress, const NetworkData &aReply);
    void slotsWrite(const NetworkData &aReply);
    void slotClientDisconnected();

signals:
    void s_started(bool);
    void s_connect(QHostAddress);
    void s_read(QHostAddress, NetworkData);
    void s_disconnect(QHostAddress);

private:
    QTcpServer *server_;
    QList<QTcpSocket*> sockets_;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};

QByteArray fileToArray(QFile &aFile);
QFile *arrayToFile(QString &aFileName, const QByteArray &aByteArray);

#endif // SERVER_H
