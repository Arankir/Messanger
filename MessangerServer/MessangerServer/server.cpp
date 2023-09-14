#include "server.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDataStream>

MyServer::MyServer(QObject *parent) : QObject(parent) {
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, &MyServer::slotNewConnection);
}

void MyServer::start(quint16 port) {
    if(!server_->listen(QHostAddress::Any, port)) {
        emit s_started(false);
        qDebug() << "server is not started";
    } else {
        emit s_started(true);
        qDebug() << "server is started";
    }
}

void MyServer::slotNewConnection() {
    while (server_->hasPendingConnections()) {
        QTcpSocket *client = server_->nextPendingConnection();
        qDebug() << "client connected" << client->peerAddress();
        sockets_.append(client);
        connect(client, &QTcpSocket::readyRead, this, &MyServer::slotServerRead);
        connect(client, &QTcpSocket::disconnected, this, &MyServer::slotClientDisconnected);
        buffers.insert(client, new QByteArray());
        sizes.insert(client, new qint32(0));
        emit s_connect(client->peerAddress());
    }
}

qint32 arrayToInt(QByteArray source) {
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

void MyServer::slotServerRead() {
//    qDebug() << "read";
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    qint32 size = *s;
    while(socket->bytesAvailable() > 0) {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) {//While can process data, process it
            if (size == 0 && buffer->size() >= 4) {//if size of data has received completely, then store it on our global variable
                size = arrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) {// If data has received completely, then emit our SIGNAL with the data
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                NetworkData rts = NetworkData::fromJson(QJsonDocument::fromJson(data).object());
//                qDebug() << "read" << rts.toJson();
                emit s_read(socket->peerAddress(), rts);
            }
        }
//        QByteArray array = socket->readAll();
//        RequestToServer rts = RequestToServer::fromJson(QJsonDocument::fromJson(array).object());
//        emit s_read(socket->peerAddress(), rts);
//        socket->write(array);
    }
}

void MyServer::slotsWrite(const NetworkData &aReply) {
//    qDebug() << "writes";
    const QByteArray message = QJsonDocument(aReply.toJson()).toJson();
    int i = 0;
    for (auto client: sockets_) {
//        qDebug() << "write" << client->peerAddress() << aReply.toJson();
        client->write(message);
        ++i;
    }
}

int MyServer::slotWrite(const QHostAddress &aAddress, const NetworkData &aReply) {
    qDebug() << "write" << aAddress << aReply.toJson();
    const QByteArray message = QJsonDocument(aReply.toJson()).toJson();
    for (int i = sockets_.count() - 1; i >= 0; --i) {
        if (sockets_[i]->peerAddress() == aAddress) {
//            qDebug() << "write" << i;
            return sockets_[i]->write(message);
        }
    }
    qWarning() << "didn't find address" << aAddress;
    return -2;
}

void MyServer::slotClientDisconnected() {
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qDebug() << socket->peerAddress() << "disconnected";
    emit s_disconnect(socket->peerAddress());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    sockets_.removeOne(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

NetworkData::NetworkData(const QString &aRequestType, const QJsonObject &aJObject): type(aRequestType), value(aJObject) {}

QJsonObject NetworkData::toJson() const {
    QJsonObject object;
    object["type"] = this->type;
    object["value"] = this->value;
    return object;
}

NetworkData NetworkData::fromJson(const QJsonObject &aObject) {
    NetworkData reply;
    reply.type = aObject.value("type").toString();
    reply.value = aObject.value("value");
    return reply;
}

QByteArray fileToArray(QFile &aFile) {
    if (!aFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file";
        return QByteArray();
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_1);

    out << (quint32)0 << aFile.fileName();

    QByteArray q = aFile.readAll();
    block.append(q);
    aFile.close();

    out.device()->seek(0);
    // This difference appear because of we send file name
    out << (quint32)(block.size() - sizeof(quint32));

    //      qint64 x = 0;
    //      while (x < block.size()) {
    //          qint64 y = socket->write(block);
    //          x += y;
    //          //qDebug() << x;    // summary size you send, so you can check recieved and replied sizes
    //      }
    return block;
}

QFile *arrayToFile(QString &aFileName, const QByteArray &aByteArray) {
//    QDataStream in(aByteArray);
//    in.setVersion(QDataStream::Qt_6_1);

    //        if (m_blockSize == 0) {
    //            if (m_socket->bytesAvailable() < sizeof(quint32))
    //            return;
    //            in >> m_blockSize;
    //        }
    //        if (m_socket->bytesAvailable() < m_blockSize)
    //            return;

    QString fileName = aFileName;
    // get sending file name
//    QByteArray line;
//    in >> /*fileName >> */line;
//    qDebug() << line;
    QString filePath = "C://logs"; // your file path for receiving
    fileName = fileName.section("/", -1);
    QFile *target = new QFile(filePath + "/" + fileName);
    qDebug() << filePath + "/" + fileName;
    if (!target->open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open file for written";
        return nullptr;
    }
    target->write(aByteArray);
    target->close();
    return target;
}

//void MyServer::slotReadClient() {
//    pClientSocket = (QTcpSocket*)sender();
//    QDataStream in(pClientSocket);
//    in.setVersion(QDataStream::Qt_5_2);
//    int mode;
//    for (;;) {

//        if (!NextBlockSize) {
//            if (pClientSocket->bytesAvailable() < (int)sizeof(quint16)) {
//                break;
//            }
//            in >> NextBlockSize;
//            qDebug() << "NexrBlockSize: " << NextBlockSize;
//        }

//        if (pClientSocket->bytesAvailable() < NextBlockSize) {
//            break;
//        }

//        QTime   time;
//        QString message, name; // users message and users name
//        in >> mode;
//        if(mode == 0) {
//            in >> time >> name >> message;

//            //check name
//            if(!checkName(name))
//                addNameToNameBase(pClientSocket, name);

//            QString strMessage = time.toString() + " " + "[" + name + "]:" + message;
//            TextField->append(strMessage);



//            //look who can get message
//            QStringList Temp;
//            in >> Temp;
//            for(int i = 0; i < Temp.count(); ++i)
//                sendToClient(ClientDataBase[Temp.at(i)], name, message);
//        }
//        if(mode == 1) {
//            in >> TempL >> FileName >> sizeReceiveFile;
//            qDebug() << "Size Recieve: " << sizeReceiveFile << "FileName: " << FileName;
//            receiveFile(FileName);
//            break;
//            // SEND FILE
//        }
//        NextBlockSize = 0;
//    }


//}

//void Server::receiveFile(QString fileName) {
//    QString savePath = "Downloads/";
//    QDir dir;
//    dir.mkpath(savePath);
//    File = new QFile(savePath + fileName);
//    File->open(QFile::WriteOnly);
//    sizeReceivedData = 0;
//    receiveFile();
//}

//void Server::receiveFile() {
//    QDataStream in(pClientSocket);
//    char block[1024];
//    while(!in.atEnd()){
//        qint64 toFile = in.readRawData(block, sizeof(block));
//        sizeReceivedData += toFile;
//        qDebug() << "sizeReceivedData: " << sizeReceivedData;
//        File->write(block, toFile);
//        if(sizeReceivedData == sizeReceiveFile){
//            File->close();
//            File = NULL;
//            qDebug() << "sizeReceivedData END: " << sizeReceivedData;
//            sizeReceiveFile = 0;
//            sizeReceivedData = 0;
//        }
//    }
//}
