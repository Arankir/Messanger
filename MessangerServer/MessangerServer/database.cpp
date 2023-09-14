#include "database.h"
#include <QDebug>
#include <QBuffer>
#include <QSqlField>
#include <QSqlDriver>
#include <QStringList>

const QString c_preTable = "[messanger].[dbo].";

int randomNumber() {
    std::srand(std::time(nullptr));
    int random = std::rand();
//    qDebug() << random;
    return random;
}

QString getLastExecutedQuery(const QSqlQuery &aQuery) {
    QString sql = aQuery.executedQuery();
    const int nbBindValues = aQuery.boundValues().size();

    for(int i = 0, j = 0; j < nbBindValues; ++j) {
        i = sql.indexOf(QLatin1Char('?'), i);
        if (i <= 0) {
            break;
        }
        const QVariant &var = aQuery.boundValue(j);
        QSqlField field(QLatin1String(""), var.type());
        if (var.isNull()) {
            field.clear();
        } else {
            field.setValue(var);
        }
        QString formatV = aQuery.driver()->formatValue(field);
        sql.replace(i, 1, formatV);
        i += formatV.length();
    }
    return sql;
}

DataBase::DataBase() : token_(randomNumber(), false) {
    init();
}

bool DataBase::init() {
    db_ = QSqlDatabase::addDatabase("QODBC");
    db_.setDatabaseName(QString("DRIVER={SQL Server};"
                                "SERVER=grimnir;"
                                "DATABASE=messanger;"
                                "Persist Security Info=true;"
                                "uid=sa;"
                                "pwd=Gfhf743Djpbr"));
    return openDB();
}

bool DataBase::isOpen() {
    return db_.isOpen();
}

bool DataBase::openDB() {
    if (db_.open()) {
        qInfo () << "DB is open";
        return true;
    } else {
        qInfo () << "DB isn't open";
        return false;
    }
}

QString DataBase::generateVCode(const QString &aKey) {
    QSqlQuery query(db_);
    if (!db_.isOpen()) {
        db_.open();
    }
    query.prepare("declare @val varchar(900) \n"
                  "exec @val = [CharCodeGen] :key \n"
                  "select @val ");
    query.bindValue(":key", aKey);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        if (query.next()) {
            return query.value(0).toString();
        } else {
            qDebug() << "-" << getLastExecutedQuery(query);
            return "-"; //vcode not found
        }
    } else {
        qDebug() << "--" << getLastExecutedQuery(query);
        return "--"; //error find vcode
    }
}

bool DataBase::vcodeIsValid(const QString &aCode) {
    QRegularExpression regExp;
    regExp.setPattern("^[0-9a-zA-Z]+$");
    auto match = regExp.match(aCode);
    return match.hasMatch();
}

Account DataBase::accountSender(const QString &aToken) {
    QSqlQuery query(db_);
    if (!db_.isOpen()) {
        db_.open();
    }
    Account account;
    account.vcode = "";

    query.prepare("select vcode, login, password, nickname, lastLogOn, lastLogOff, cdate from accounts where token = :token");
    query.bindValue(":token", aToken);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        if (query.next()) {
            account.vcode = query.value(0).toString();
            account.login = query.value(1).toString();
            account.password = query.value(2).toString();
            account.nickname = query.value(3).toString();
            account.lastLogOn = query.value(4).toDateTime();
            account.lastLogOff = query.value(5).toDateTime();
            account.datetime = query.value(6).toDateTime();

            return account;
        } else {
            qDebug() << -3 << getLastExecutedQuery(query);
            return account; //vcode from account not found
        }
    } else {
        qDebug() << -2 << getLastExecutedQuery(query);
        return account; //error find from account
    }
    return account;//9
}

QString DataBase::login(const QString &aLogin, const QString &aPassword, const QHostAddress &aAddress, QString &aNickName) {
    QSqlQuery query(db_);
    db_.open();
    query.prepare("select isnull(vcode, -1), nickname from accounts where login = :login and password = :password");
    query.bindValue(":login", aLogin);
    query.bindValue(":password", aPassword);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        if (query.next()) {
            int vcode = query.value(0).toInt();
            if (vcode == -1) {
                return "-5"; //invalid log/pass
            }
            aNickName = query.value(1).toString();
            QString token;
            int count = 0;
            bool tokenIsUnique = false;
            while (count < 10 && !tokenIsUnique) {
                if (token_.GetNextToken(token)) {
                    query.prepare("select count(vcode) from accounts where token = :token");
                    query.bindValue(":token", token);
                    if (query.exec()) {
                        if (query.next()) {
                            if (query.value(0).toInt() == 0) {
                                tokenIsUnique = true;
                            }
                        }
                    }
                }
                ++count;
            }
            if (tokenIsUnique) {
                QHostAddress addressIPv4(aAddress.toIPv4Address());
                query.prepare("update accounts set token = :token, lastLogOn = getdate(), lastIP = :lastIP where vcode = :vcode");
                query.bindValue(":vcode", vcode);
                query.bindValue(":token", token);
                query.bindValue(":lastIP", addressIPv4.toString());
                if (query.exec()) {
                    return token;
                } else {
                    qDebug() << getLastExecutedQuery(query);
                    return "-1"; //not update data in account
                }
            } else {
                return "-2"; //token not generated
            }
        } else {
            return "-3"; //vcode account not found
        }
    } else {
        return "-4"; //error find account
    }
}

int DataBase::logOff(const QString &aNick) {
    qDebug() << aNick;
    QSqlQuery query(db_);
    db_.open();
    query.prepare("update accounts set lastLogOff = getdate() where nickname = :nickname");
    query.bindValue(":nickname", aNick);
    if (query.exec()) {
        qDebug() << 1;
        return 1;
    } else {
        qDebug() << getLastExecutedQuery(query);
        return -1; //not update data in account
    }
}

QList<Account> DataBase::users() {
    QList<Account> returnList;
    QSqlQuery query(db_);
    db_.open();
    query.prepare("select [vcode], [login], [password], [nickname], [lastLogOn], [lastLogOff], cdate from accounts");
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        while (query.next()) {
            Account account;
            account.vcode = query.value(0).toString();
            account.login = query.value(1).toString();
            account.password = query.value(2).toString();
            account.nickname = query.value(3).toString();
            account.lastLogOn = query.value(4).toDateTime();
            account.lastLogOff = query.value(5).toDateTime();
            returnList.append(account);
        }
    } else {
        return returnList; //error find account
    }
    return returnList;
}

QList<Group> DataBase::groupsUser(const QString &aToken) {
    QList<Group> resultList;
    QSqlQuery query(db_);
    if (!db_.isOpen()) {
        db_.open();
    }
    Account sender = accountSender(aToken);
    if (sender.vcode == "") {
        return resultList;
    }
    query.prepare("SELECT distinct c.vcode, c.title "
                  "FROM [groups] c "
                  "inner join [groupsUsers] cu "
                  "ON (c.vcode = cu.[group]) "
                  "where cu.account = :account");
    query.bindValue(":account", sender.vcode);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        while (query.next()) {
            Group group;
            group.vcode = query.value(0).toString();
            group.title = query.value(1).toString();
            QSqlQuery query2(db_);
            query2.prepare("select a.[vcode], [login], [password], [nickname], [lastLogOn], [lastLogOff], a.cdate "
                           "from accounts a "
                           "inner join [groupsUsers] cu "
                           "ON (cu.account = a.vcode) "
                           "where cu.[group] = :group ");
            query2.bindValue(":group", query.value(0).toInt());
            query2.exec();
            if (query2.lastError().type() == QSqlError::NoError) {
                while (query2.next()) {
                    Account account;
                    account.vcode = query2.value(0).toString();
                    account.login = query2.value(1).toString();
                    account.password = query2.value(2).toString();
                    account.nickname = query2.value(3).toString();
                    account.lastLogOn = query2.value(4).toDateTime();
                    account.lastLogOff = query2.value(5).toDateTime();
                    group.accounts << account;
                }
            }
            resultList << group;
        }
    } else {
        qDebug() << -4 << getLastExecutedQuery(query);
        return resultList; //error find account
    }
    return resultList;
}

Account DataBase::accountFromNick(const QString &aNickname) {
    QSqlQuery query2(db_);
    query2.prepare("select top 1 [vcode], [login], [password], [nickname], [lastLogOn], [lastLogOff], cdate "
                   "from accounts "
                   "where nickname = :nickname ");
    query2.bindValue(":nickname", aNickname);
    query2.exec();
    if (query2.lastError().type() == QSqlError::NoError) {
        while (query2.next()) {
            Account account;
            account.type = ChatType::account;
            account.vcode = query2.value(0).toString();
            account.login = query2.value(1).toString();
            account.password = query2.value(2).toString();
            account.nickname = query2.value(3).toString();
            account.lastLogOn = query2.value(4).toDateTime();
            account.lastLogOff = query2.value(5).toDateTime();
            return account;
        }
    } else {
        qDebug() << getLastExecutedQuery(query2);
    }
    return Account();
}

Group DataBase::groupFromTitle(const QString &aTitle) {
    QSqlQuery query2(db_);
    query2.prepare("select a.[vcode], [login], [password], [nickname], [lastLogOn], [lastLogOff], a.cdate, g.title, g.vcode "
                   "from groups g "
                   "inner join [groupsUsers] gu "
                   "ON (gu.[group] = g.vcode) "
                   "inner join [accounts] a "
                   "ON (gu.account = a.vcode) "
                   "where g.title = :group ");
    query2.bindValue(":group", aTitle);
    query2.exec();
    Group group;
    group.type = ChatType::group;
    if (query2.lastError().type() == QSqlError::NoError) {
        while (query2.next()) {
            group.title = query2.value(7).toString();
            group.vcode = query2.value(8).toString();
            Account account;
            account.vcode = query2.value(0).toString();
            account.login = query2.value(1).toString();
            account.password = query2.value(2).toString();
            account.nickname = query2.value(3).toString();
            account.lastLogOn = query2.value(4).toDateTime();
            account.lastLogOff = query2.value(5).toDateTime();
            group.accounts << account;
        }
    } else {
        qDebug() << getLastExecutedQuery(query2);
    }
    return group;
}

QList<Chat *> DataBase::chatsUser(const QString &aToken) {
    QList<Chat*> resultList;
    QSqlQuery query(db_);
    if (!db_.isOpen()) {
        db_.open();
    }
    Account sender = accountSender(aToken);
    if (sender.vcode == "") {
        return resultList;
    }
    query.exec("{CALL dbo.Mes_LastChatMessages (" + QStringList(QStringList() << sender.nickname).join(",") + ")}");
    if (query.lastError().type() == QSqlError::NoError) {
        while (query.next()) {
            if (query.value("type").toString() == "account") {
                resultList.append(new Account(accountFromNick(query.value("to").toString())));
            }
            if (query.value("type").toString() == "group") {
                resultList.append(new Group(groupFromTitle(query.value("to").toString())));
            }
        }
    } else {
        qDebug() << -1 << getLastExecutedQuery(query);
        return resultList; //error find account
    }
    return resultList;
}

QString DataBase::nickFromLogin(const QString &aLogin) {
    QSqlQuery query(db_);
    if (!db_.isOpen()) {
        db_.open();
    }
    query.prepare("select isnull(nickname, '') from accounts where login = :login");
    query.bindValue(":login", aLogin);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        if (query.next()) {
            QString nick = query.value(0).toString();
            return nick;
        } else {
            return "-1"; //vcode account not found
        }
    } else {
        return "-2"; //error find account
    }
}

int DataBase::addMessage(const QString &aMessage, const QString &aToken, const QString &aNickTo, const QString &aChatTo, const QImage &aImage) {
    if (aNickTo.isEmpty() && aChatTo.isEmpty()) {
        return -1; //resiever not selected
    }
    if (!aNickTo.isEmpty() && !aChatTo.isEmpty()) {
        return -2; //resiever multiple selected
    }
    QString sImage;
    if (aImage.isNull()) {
        sImage = "NULL";
    } else {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        aImage.save(&buffer, "PNG");
        sImage = "'" + QString::fromUtf8(buffer.data().toBase64()) + "'";
    }

    Account sender = accountSender(aToken);
    if (sender.vcode == "") {
        return -7; //token didn't find
    }
    QString vcodeNickTo = "0";
    QString vcodeChatTo = "0";

    if (!aNickTo.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, -1) from accounts where nickname = :nickname");
        query.bindValue(":nickname", aNickTo);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeNickTo = QString::number(query.value(0).toInt());
            } else {
                return -3; //vcode account not found
            }
        } else {
            return -4; //error find account
        }
    }
    if (!aChatTo.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, -1) from [groups] where title = :title");
        query.bindValue(":title", aChatTo);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeChatTo = QString::number(query.value(0).toInt());
            } else {
                return -5; //vcode chat not found
            }
        } else {
            return -6; //error find chat
        }
    }

    QSqlQuery query(db_);
    query.prepare("{CALL dbo.Mes_CreateMessage (" +
                  QStringList(QStringList() << "'" + aMessage + "'" << sender.vcode << vcodeNickTo << vcodeChatTo << sImage).join(",") +
                   ")}");
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        query.next();
        return 1;
    } else {
        return -9; //message not created
    }
}

int DataBase::addMessage(const QString &senderVcode, const Message &aMessage) {
    if (aMessage.account.isEmpty() && aMessage.group.isEmpty()) {
        return -1; //resiever not selected
    }
    if (!aMessage.account.isEmpty() && !aMessage.group.isEmpty()) {
        return -2; //resiever multiple selected
    }
    if (aMessage.files.size() > 10) {
        return -3; //files more than 10
    }
    QStringList filesList;
    for (const auto &file: aMessage.files) {
        if (!vcodeIsValid(file.vcode)) {
            return -4; //not all files in db
        }
        filesList << file.vcode;
    }
    QString sImage = "NULL";
//    if (aImage.isNull()) {
//        sImage = "NULL";
//    } else {
//        QBuffer buffer;
//        buffer.open(QIODevice::WriteOnly);
//        aImage.save(&buffer, "PNG");
//        sImage = "'" + QString::fromUtf8(buffer.data().toBase64()) + "'";
//    }

    QString vcodeNickTo = "0";
    QString vcodeGroupTo = "0";

    for (int i = filesList.count(); i < 10; ++i) {
        filesList << "NULL";
    }

    if (!aMessage.account.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, '') from accounts where nickname = :nickname");
        query.bindValue(":nickname", aMessage.account);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeNickTo = QString::number(query.value(0).toInt());
            } else {
                return -5; //vcode account not found
            }
        } else {
            return -6; //error find account
        }
    }
    if (!aMessage.group.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, '') from [groups] where title = :title");
        query.bindValue(":title", aMessage.group);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeGroupTo = QString::number(query.value(0).toInt());
            } else {
                return -7; //vcode chat not found
            }
        } else {
            return -8; //error find group
        }
    }

    QSqlQuery query(db_);
    query.prepare("{CALL dbo.Mes_CreateMessage (" +
                  QStringList(QStringList() << "'" + aMessage.message + "'" << senderVcode << vcodeNickTo << vcodeGroupTo << sImage << filesList).join(",") +
                   ")}");
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        query.next();
        return 1;
    } else {
        return -9; //message not created
    }
}

QList<Message> DataBase::showMessage(const QString &aToken, const QString &aNickTo, const QString &aChatTo) {
    QList<Message> resultList;
    if (aNickTo.isEmpty() && aChatTo.isEmpty()) {
        return resultList; //resiever not selected
    }
    if (!aNickTo.isEmpty() && !aChatTo.isEmpty()) {
        return resultList; //resiever multiple selected
    }

    Account sender = accountSender(aToken);
    if (sender.vcode == "") {
        return resultList; //token didn't find
    }
    QString vcodeNickTo = "0";
    QString vcodeChatTo = "0";

    if (!aNickTo.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, '') from accounts where nickname = :nickname");
        query.bindValue(":nickname", aNickTo);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeNickTo = QString::number(query.value(0).toInt());
            } else {
                qDebug() << -3 << getLastExecutedQuery(query);
                return resultList; //vcode account not found
            }
        } else {
            qDebug() << -4 << getLastExecutedQuery(query);
            return resultList; //error find account
        }
    }
    if (!aChatTo.isEmpty()) {
        QSqlQuery query(db_);
        query.prepare("select isnull(vcode, '') from [groups] where title = :title");
        query.bindValue(":title", aChatTo);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            if (query.next()) {
                vcodeChatTo = QString::number(query.value(0).toInt());
            } else {
                qDebug() << -5 << getLastExecutedQuery(query);
                return resultList; //vcode chat not found
            }
        } else {
            qDebug() << -6 << getLastExecutedQuery(query);
            return resultList; //error find chat
        }
    }

    QSqlQuery query(db_);
    query.prepare("{CALL dbo.Mes_GetMessages (" +
                  QStringList(QStringList() << sender.vcode << vcodeNickTo << vcodeChatTo).join(",") +
                  ")}");
    query.exec();
    if (query.lastError().type() == QSqlError::NoError) {
        while (query.next()) {
            Message message;
            message.vcode = query.value("vcode").toString();
            message.message = query.value("message").toString();
            message.image.loadFromData(QByteArray::fromBase64(query.value("image").toString().toUtf8()), "PNG");
            message.from = query.value("from").toString();
            message.group = query.value("group").toString();
            message.cdate = query.value("cdate").toDateTime();
            message.readed = query.value("isShowed").toInt();
            message.peoples = query.value("peoples").toInt();
            QList<int> vcodeFiles;
            for (int i = 1; i <= 10; ++i) {
                QString columnName = "file" + QString::number(i);
                if (!query.value(columnName).isNull()) {
                    vcodeFiles.append(query.value(columnName).toInt());
                }
            }
            resultList.append(message);
        }
        return resultList;
    } else {
        qDebug() << -9 << getLastExecutedQuery(query);
        return resultList; //message not created
    }
}

//File DataBase::getFile(const int &aVcode) {

//}

QString DataBase::insertFile(const QString &aServerPath, const File &file) {
    QString vcode = generateVCode("files");
    if (vcodeIsValid(vcode)) {
        QSqlQuery query(db_);
        query.prepare("insert into [files] ([vcode],[cdate],[serverPath],[fileName],[baseName],[suffix],[birthDate],[modifiedDate],[readDate],[fileSize]) "
                      "values (:vcode,getdate(),:serverPath,:fileName,:baseName,:suffix,:birthDate,:modifiedDate,:readDate,:fileSize)");
        query.bindValue(":vcode", vcode);
        query.bindValue(":serverPath", aServerPath.arg(vcode));
        query.bindValue(":fileName", file.fileName);
        query.bindValue(":baseName", file.baseName);
        query.bindValue(":suffix", file.suffix);
        query.bindValue(":birthDate", file.birthDate);
        query.bindValue(":modifiedDate", file.modifiedDate);
        query.bindValue(":readDate", file.readDate);
        query.bindValue(":fileSize", file.fileSize);
        query.exec();
        if (query.lastError().type() == QSqlError::NoError) {
            return vcode;
        } else {
            qDebug() << "-" << getLastExecutedQuery(query);
            return "-"; //error find account
        }
    }
    return "--";
}

QJsonObject Message::toJson() {
    QString sImage;
    if (image.isNull()) {
        sImage = "NULL";
    } else {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        sImage = QString::fromUtf8(buffer.data().toBase64());
    }

    QJsonObject jMessage;
    jMessage["vcode"] = vcode;
    jMessage["message"] = message;
    jMessage["image"] = sImage;
    jMessage["from"] = from;
    jMessage["account"] = account;
    jMessage["chat"] = group;
    jMessage["cdate"] = cdate.toString("yyyy.MM.dd hh:mm:ss.zzz");
    jMessage["readed"] = readed;
    jMessage["peoples"] = peoples;

    QJsonArray filesArray;
    int i = 0;
    for (const auto &file: files) {
        filesArray.append(file.toJson());
        ++i;
        if (i > 9) {
            break;
        }
    }
    jMessage["files"] = filesArray;

    return jMessage;
}

Message &Message::fromJson(QJsonObject aObject) {
    vcode = aObject.value("vcode").toString();
    message = aObject.value("message").toString();
    image.loadFromData(QByteArray::fromBase64(aObject.value("image").toString().toUtf8()), "PNG");
    from = aObject.value("from").toString();
    account = aObject.value("account").toString();
    group = aObject.value("chat").toString();
    cdate = QDateTime::fromString(aObject.value("cdate").toString(), "yyyy.MM.dd hh:mm:ss.zzz");
    readed = aObject.value("readed").toInt();
    peoples = aObject.value("peoples").toInt();
    auto filesArray = aObject.value("files").toArray();
    for (auto file: filesArray) {
        files << File::fromJson(file.toObject());
    }
    return *this;
}

QJsonObject Account::toJson() {
    QJsonObject jMessage;
    jMessage["vcode"] = vcode;
    jMessage["login"] = login;
    jMessage["password"] = password;
    jMessage["lastLogOn"] = lastLogOn.toString("MM.dd hh:mm:ss.zzz");
    jMessage["lastLogOff"] = lastLogOff.toString("MM.dd hh:mm:ss.zzz");
    jMessage["datetime"] = datetime.toString("MM.dd hh:mm:ss.zzz");
    return jMessage;
}

Account &Account::fromJson(QJsonObject aObject) {
    vcode = aObject.value("vcode").toString();
    login = aObject.value("login").toString();
    password = aObject.value("password").toString();
    lastLogOn = QDateTime::fromString(aObject.value("lastLogOn").toString(), "MM.dd hh:mm:ss.zzz");
    lastLogOff = QDateTime::fromString(aObject.value("lastLogOff").toString(), "MM.dd hh:mm:ss.zzz");
    datetime = QDateTime::fromString(aObject.value("datetime").toString(), "MM.dd hh:mm:ss.zzz");
    return *this;
}

QJsonObject Group::toJson() {
    QJsonObject jMessage;
    jMessage["vcode"] = vcode;
    jMessage["title"] = title;
    QJsonArray array;
    for (auto account: accounts) {
        array.append(account.toJson());
    }
    jMessage["accounts"] = array;
    return jMessage;
}

Group &Group::fromJson(QJsonObject aObject) {
    vcode = aObject.value("vcode").toString();
    title = aObject.value("title").toString();
    for (auto account: aObject.value("accounts").toArray()) {
        accounts.append(Account().fromJson(account.toObject()));
    }
    return *this;
}

QJsonObject File::toJson(bool withData) const {
    QJsonObject obj;
    obj["originalFilePath"] = originalFilePath;
    if (withData) {
        obj["file"] = QString::fromLocal8Bit(fileData);
    }
    obj["fileName"] = fileName;
    obj["baseName"] = baseName;
    obj["suffix"] = suffix;
    obj["birthDate"] = birthDate.toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["modifiedDate"] = modifiedDate.toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["readDate"] = readDate.toString("yyyy.MM.dd hh:mm:ss.zzz");
    obj["fileSize"] = fileSize;
    obj["localVcode"] = QString::number(localVcode);
    obj["vcode"] = vcode;
    return obj;
}

File File::fromJson(QJsonObject aObject) {
    File file;
    file.originalFilePath = aObject.value("originalFilePath").toString();
    file.fileData = aObject.value("file").toString().toLocal8Bit().append('\x00');
    file.fileName = aObject.value("fileName").toString();
    file.baseName = aObject.value("baseName").toString();
    file.suffix = aObject.value("suffix").toString();
    file.birthDate = QDateTime::fromString(aObject.value("birthDate").toString(),"yyyy.MM.dd hh:mm:ss.zzz");
    file.modifiedDate = QDateTime::fromString(aObject.value("modifiedDate").toString(),"yyyy.MM.dd hh:mm:ss.zzz");
    file.readDate = QDateTime::fromString(aObject.value("readDate").toString(),"yyyy.MM.dd hh:mm:ss.zzz");
    file.vcode = aObject.value("vcode").toString();
    file.localVcode = aObject.value("localVcode").toString().toLongLong();
    return file;
}
