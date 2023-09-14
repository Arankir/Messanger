#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QHostAddress>
#include "tokengenerator.h"
#include <QtGui/QImage>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>

struct File {
    QString originalFilePath;
    QByteArray fileData;
    QString fileName;
    QString baseName;
    QString suffix;
    QDateTime birthDate;
    QDateTime modifiedDate;
    QDateTime readDate;
    int fileSize;
    QString vcode;
    long long localVcode;

    QJsonObject toJson(bool withData = false) const;
    static File fromJson(QJsonObject);
    friend QDebug operator<<(QDebug dbg, const File &a) {
        dbg.nospace() << "File(" << a.toJson() << ")\n";
        return dbg.space();
    }
};

struct Message {
    QString vcode;
    QString message;
    QImage image;
    QString from;
    QString account;
    QString group;
    QDateTime cdate;
    int readed;
    int peoples;
    QList<File> files;

    QJsonObject toJson();
    Message &fromJson(QJsonObject);
};

enum ChatType {
    account,
    group,
    unknown
};

struct Chat {
    Chat(ChatType = unknown) {}
    virtual ~Chat() {}
    QString vcode = "";
    ChatType type;
};

struct Account: Chat {
    Account(): Chat(ChatType::account) {}
    QString login;
    QString password;
    QString nickname;
    QDateTime lastLogOn;
    QDateTime lastLogOff;
    QDateTime datetime;

    QJsonObject toJson();
    Account &fromJson(QJsonObject);
};

struct Group: Chat {
    Group(): Chat(ChatType::group) {}
    QString title;
    QList<Account> accounts;

    QJsonObject toJson();
    Group &fromJson(QJsonObject);
};

class DataBase {
public:
    DataBase();
    bool openDB();
    bool isOpen();
    QString login(const QString &aLogin, const QString &aPassword, const QHostAddress &aAddress, QString &aNickName);
    QString generateVCode(const QString &aKey);
    static bool vcodeIsValid(const QString &aCode);
    int addMessage(const QString &aMessage, const QString &aToken, const QString &aNickTo, const QString &aChatTo, const QImage &aImage);
    int addMessage(const QString &senderVcode, const Message &aMessage);
    QList<Message> showMessage(const QString &aToken, const QString &aNickTo, const QString &aChatTo);
    QString nickFromLogin(const QString &aLogin);
    int logOff(const QString &aNick);
    QList<Account> users();
    QList<Group> groupsUser(const QString &aToken);
    QList<Chat*> chatsUser(const QString &aToken);
    Account accountFromNick(const QString &aNickname);
    Group groupFromTitle(const QString &aTitle);
    Account accountSender(const QString &aToken);
    QString insertFile(const QString &aServerPath, const File &file);
private:
    QSqlDatabase db_;
    TokenGenerator token_;

    bool init();
};

#endif // DATABASE_H
