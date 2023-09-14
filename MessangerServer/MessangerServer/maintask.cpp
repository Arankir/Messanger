#include "maintask.h"
#include <QInputDialog>
#include <QMessageBox>

const QString c_password = "rjrjcs743";

MainTask::MainTask(int argc, char **argv) : QObject(nullptr), trayIcon_(new QSystemTrayIcon()) {
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    if (!db_.isOpen()) {
        qWarning() << "db isn't open";
    }
    initServer();
    trayIcon_->setIcon(QIcon("://icon.png"));
    trayIcon_->setContextMenu(createTrayIconMenu());
    trayIcon_->show();

}

MainTask::~MainTask() {
    delete server_;
}

void MainTask::initServer() {
    server_ = new MyServer();
    connect(server_, &MyServer::s_started, this, [](bool started) {
//        if (started) {
//            qDebug() << "server started";
//        } else {
//            qDebug() << "server not started";
//        }
    });
    connect(server_, &MyServer::s_read, this, &MainTask::serverRead);
    connect(server_, &MyServer::s_connect, this, &MainTask::serverOnConnect);
    connect(server_, &MyServer::s_disconnect, this, &MainTask::serverOnDisconnect);
    server_->start(9999);
}

void MainTask::serverRead(QHostAddress aAddress, NetworkData aRequest) {
    qDebug() << "server read" << aAddress << aRequest.type << aRequest.value;
    NetworkData reply(aRequest.type);
    if (aRequest.type == "logOn") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QJsonObject objReply;
        QString nickname;
        QString token = db_.login(obj.value("login").toString(), obj.value("password").toString(), aAddress, nickname);

        if (token == "-1") { //not update data in account
            objReply["status"] = "error";
            objReply["error"] = tr("Ошибка обновления данных аккаунта");
        } else if (token == "-2") { //token not generated
            objReply["status"] = "error";
            objReply["error"] = tr("Ошибка создания токена");
        } else if (token == "-3") { //vcode account not found
            objReply["status"] = "error";
            objReply["error"] = tr("Ошибка нахождения аккаунта");
        } else if (token == "-4") { //error find account
            objReply["status"] = "error";
            objReply["error"] = tr("Не удается найти аккаунт");
        } else if (token == "-5") { //invalid login password
            objReply["status"] = "error";
            objReply["error"] = tr("Неверно введен логин или пароль");
        } else {
            QString nick = db_.nickFromLogin(obj.value("login").toString());
            if (nick == "-1") { //not update token in db
                objReply["status"] = "error";
                objReply["error"] = tr("Ошибка нахождения ника");
            } else if (nick == "-2") { //token not generated
                objReply["status"] = "error";
                objReply["error"] = tr("Не удается найти ник");
            } else {
                auto iterator = std::find_if(usersOnline_.begin(),
                                            usersOnline_.end(),
                                            [&](QPair<QHostAddress, QString> lUser) {
                                                return lUser.first == aAddress;
                                            });
                if (iterator != usersOnline_.end()) {
                    (*iterator).second = nick;
                    objReply["status"] = "success";
                    objReply["token"] = token;
                    objReply["nickname"] = nickname;

                    for (const auto &user: usersOnline_) {
                        if (!user.second.isEmpty()) {
                            if (user.first != aAddress) {
                                NetworkData newReply;
                                newReply.type = "newInfo";

                                QJsonObject jUser;
                                jUser["nickname"] = nick;
                                jUser["status"] = "online";

                                QJsonObject objNewInfoReply;
                                objNewInfoReply["user"] = jUser;
                                newReply.value = objNewInfoReply;
                                server_->slotWrite(user.first, newReply);
                            }
                        }
                    }
                } else {
                    objReply["status"] = "error";
                    objReply["error"] = tr("Не удается сохранить состояние, перезайдите в приложение");
                }
            }
        }
        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "onlineUsers") {
        auto users = db_.users();

        QJsonObject objReply;
        QJsonArray arrayUsers;
        for (const auto &user: users) {
            QJsonObject objUser;
            objUser["nickname"] = user.nickname;

            auto iteratorUser = std::find_if(usersOnline_.begin(),
                                             usersOnline_.end(),
                                             [&](QPair<QHostAddress, QString> lUser) {
                                                return lUser.second == user.nickname;
                                            });
            if (iteratorUser != usersOnline_.end()) {
                objUser["status"] = "online";
            } else {
                if (user.lastLogOn > user.lastLogOff) {
                    objUser["status"] = "away";
                } else {
                    objUser["status"] = "offline";
                }
            }

            arrayUsers.append(objUser);
        }
        objReply["users"] = arrayUsers;
        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "groups") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QString token = obj.value("token").toString();
        auto chats = db_.groupsUser(token);

        QJsonObject objReply;
        QJsonArray arrayChats;
        for (const auto &chat: chats) {
            QJsonObject objChat;
            objChat["title"] = chat.title;
            QJsonArray arrayUsers;
            for (const auto &user: chat.accounts) {
                QJsonObject objUser;
                objUser["nickname"] = user.nickname;
                arrayUsers << objUser;
            }
            objChat["users"] = arrayUsers;
            arrayChats.append(objChat);
        }
        objReply["groups"] = arrayChats;
        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "chats") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QString token = obj.value("token").toString();
        auto chats = db_.chatsUser(token);
        QJsonArray arrayChats;
        for (const auto &chat: chats) {
            switch (chat->type) {
            case ChatType::account: {
                if (Account *account = static_cast<Account*>(chat)) {
                    QJsonObject objAccount;
                    objAccount["nickname"] = account->nickname;

                    auto iteratorUser = std::find_if(usersOnline_.begin(),
                                                     usersOnline_.end(),
                                                     [&](QPair<QHostAddress, QString> lUser) {
                                                        return lUser.second == account->nickname;
                                                    });
                    if (iteratorUser != usersOnline_.end()) {
                        objAccount["status"] = "online";
                    } else {
                        if (account->lastLogOn > account->lastLogOff) {
                            objAccount["status"] = "away";
                        } else {
                            objAccount["status"] = "offline";
                        }
                    }
                    QJsonObject objChat;
                    objChat["type"] = "account";
                    objChat["account"] = objAccount;
                    arrayChats.append(objChat);
                    delete account;
                }
                break;
            }
            case ChatType::group: {
                if (Group *group = static_cast<Group*>(chat)) {
                    QJsonObject objGroup;
                    objGroup["title"] = group->title;
                    QJsonArray arrayUsers;
                    for (const auto &user: group->accounts) {
                        QJsonObject objUser;
                        objUser["nickname"] = user.nickname;
                        arrayUsers << objUser;
                    }
                    objGroup["users"] = arrayUsers;
                    QJsonObject objChat;
                    objChat["type"] = "group";
                    objChat["group"] = objGroup;
                    arrayChats.append(objChat);
                    delete group;
                }
                break;
            }
            default: {
                break;
            }
            }
        }
        QJsonObject objReply;
        objReply["chats"] = arrayChats;
        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "sendMessage") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QJsonObject objReply;
//        QString message = obj.value("message").toString();
        QString token = obj.value("token").toString();
        Account sender = db_.accountSender(token);
        if (!DataBase::vcodeIsValid(sender.vcode)) {
            return;
        }
//        QString accountTo = obj.value("account").toString();
//        QString chatTo = obj.value("chat").toString();
//        QImage image;
//        image.loadFromData(QByteArray::fromBase64(obj.value("image").toString().toUtf8()), "PNG");

        Message mMessage;
        mMessage.fromJson(obj);
        bool isFilesLoaded = true;
        for (auto &file: mMessage.files) {
            int code = saveFile(file);
            if (code < 0) {
                objReply["status"] = "error";
                objReply["error"] = tr("Файлы не загружены");
                isFilesLoaded = false;
            }
        }

        if (isFilesLoaded) {
            int code = db_.addMessage(sender.vcode, mMessage);
            switch (code) {
            case -1: {
                objReply["status"] = "error";
                objReply["error"] = tr("Получатель не выбран");
                break;
            }
            case -2: {
                objReply["status"] = "error";
                objReply["error"] = tr("Выбрано больше одного получателя");
                break;
            }
            case -3: {
                objReply["status"] = "error";
                objReply["error"] = tr("Выбрано больше 10 файлов");
                break;
            }
            case -4: {
                objReply["status"] = "error";
                objReply["error"] = tr("Не все файлы загружены на сервер");
                break;
            }
            case -5: {
                objReply["status"] = "error";
                objReply["error"] = tr("Ошибка нахождения аккаунта");
                break;
            }
            case -6: {
                objReply["status"] = "error";
                objReply["error"] = tr("Не удается найти аккаунт");
                break;
            }
            case -7: {
                objReply["status"] = "error";
                objReply["error"] = tr("Ошибка нахождения чата");
                break;
            }
            case -8: {
                objReply["status"] = "error";
                objReply["error"] = tr("Не удается найти чат");
                break;
            }
            case -9: {
                objReply["status"] = "error";
                objReply["error"] = tr("Не удалось отправить сообщение, попробуйте снова через какое-то время");
                break;
            }
            default: {
                objReply["status"] = "success";
                objReply["info"] = tr("Сообщение успешно отправлено");

                auto iteratorUser = std::find_if(usersOnline_.begin(),
                                                 usersOnline_.end(),
                                                 [&](const QPair<QHostAddress, QString> &lUser) {
                                                     return lUser.second == mMessage.account;
                                                 });
                if (iteratorUser != usersOnline_.end()) {
                    NetworkData newReply;
                    newReply.type = "newInfo";

//                    Message message;
//                    message.message = obj.value("message").toString();
//                    message.from = db_.accountSender(token).nickname;
//                    message.account = obj.value("account").toString();
//                    message.group = obj.value("chat").toString();
//                    message.image.loadFromData(QByteArray::fromBase64(obj.value("image").toString().toUtf8()), "PNG");

    //                QJsonObject jMessage;
    //                jMessage["message"] = obj.value("message").toString();
    //                jMessage["accountTo"] = obj.value("account").toString();
    //                jMessage["chatTo"] = obj.value("chat").toString();
    //                jMessage["image"] = obj.value("image").toString();

                    QJsonObject objNewInfoReply;
                    objNewInfoReply["message"] = mMessage.toJson();
                    newReply.value = objNewInfoReply;
                    server_->slotWrite((*iteratorUser).first, newReply);
                }
            }
            }
        }
        objReply["message"] = obj;
        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "showMessage") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QJsonObject objReply;
        QString token = obj.value("token").toString();
        QString accountTo = obj.value("accountTo").toString();
        QString chatTo = obj.value("chatTo").toString();

        auto messages = db_.showMessage(token, accountTo, chatTo);
        objReply["status"] = "unknown";
        QJsonArray jMessages;
        for (auto message: messages) {
            jMessages.append(message.toJson());
        }
        objReply["messages"] = jMessages;

        auto iteratorUser = std::find_if(usersOnline_.begin(),
                                         usersOnline_.end(),
                                         [&](const QPair<QHostAddress, QString> &lUser) {
                                             return lUser.second == accountTo;
                                         });
        if (iteratorUser != usersOnline_.end()) {
            NetworkData newReply;
            newReply.type = "newInfo";

//            QJsonObject jMessage;
//            jMessage["message"] = obj.value("message").toString();
//            jMessage["accountTo"] = obj.value("accountTo").toString();
//            jMessage["chatTo"] = obj.value("chatTo").toString();
//            jMessage["image"] = obj.value("image").toString();

            QJsonObject objNewInfoReply;
            objNewInfoReply["messageShowed"] = jMessages;
            newReply.value = objNewInfoReply;
            server_->slotWrite((*iteratorUser).first, newReply);
        }

        reply.value = objReply;
        server_->slotWrite(aAddress, reply);
    }
    if (aRequest.type == "file") {
        auto obj = aRequest.value.toObject();
        if (!db_.isOpen()) {
            db_.openDB();
        }
        QString token = obj.value("token").toString();
        Account sender = db_.accountSender(token);
        if (!DataBase::vcodeIsValid(sender.vcode)) {
            return;
        }
        File file = File::fromJson(obj.value("file").toObject());

        NetworkData newReply(aRequest.type);
        QJsonObject newObj;

        int code = saveFile(file);
        switch(code) {
        case -1: {
            newObj["status"] = "error";
            newObj["error"] = "can't open file for written";
            break;
        }
        case -2: {
            newObj["status"] = "error";
            newObj["error"] = "data not inserted";
            break;
        }
        case -3: {
            newObj["status"] = "error";
            newObj["error"] = "code not created";
            break;
        }
        case 1: {
            newObj["status"] = "success";
            newObj["file"] = file.toJson();
        }
        }
        reply.value = newObj;
        server_->slotWrite(aAddress, reply);

    }
}

void MainTask::serverOnConnect(QHostAddress aAddress) {
    usersOnline_.append(QPair<QHostAddress, QString>(aAddress, ""));
}

void MainTask::serverOnDisconnect(QHostAddress aAddress) {
    auto iterator = std::find_if(usersOnline_.begin(),
                                usersOnline_.end(),
                                [&](QPair<QHostAddress, QString> lUser) {
                                    return lUser.first == aAddress;
                                });
    if (iterator != usersOnline_.end()) {
        int count = 0;
        bool isDataUpdated = false;
        while (count < 10 && !isDataUpdated) {
            isDataUpdated = (db_.logOff((*iterator).second) == 1);
            ++count;
        }
        auto disconnectedUser = usersOnline_.takeAt(iterator - usersOnline_.begin());
        for (const auto &user: usersOnline_) {
            if (!user.second.isEmpty()) {
                if (user.first != aAddress) {
                    NetworkData newReply;
                    newReply.type = "newInfo";

                    QJsonObject jUser;
                    jUser["nickname"] = disconnectedUser.second;
                    jUser["status"] = "offline";

                    QJsonObject objNewInfoReply;
                    objNewInfoReply["user"] = jUser;
                    newReply.value = objNewInfoReply;
                    server_->slotWrite(user.first, newReply);
                }
            }
        }
    }
}

int MainTask::saveFile(File &aFile) {
    const static QString filesPath =  "files/";
    QDir(QDir::currentPath()).mkpath(filesPath);
    QString serverPath = filesPath + "%1_" + aFile.fileName/*.section("/", -1)*/;

    QString vcode = db_.insertFile(serverPath, aFile);

    if (DataBase::vcodeIsValid(vcode)) {
        QFile target(serverPath.arg(vcode));
        if (target.open(QIODevice::WriteOnly)) {
            target.write(aFile.fileData);
            target.close();
            aFile.vcode = vcode;
            return 1;
        } else {
            return -1;
        }
    } else {
        return -2;
    }
    return -3;
}

bool MainTask::createDir(const QString &aPath) {
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

QMenu *MainTask::createTrayIconMenu() {
    // Setting actions...
    QAction *settingsAction = new QAction("Настройки",       this);
    QAction *quitAction     = new QAction("Выход",           this);

    // Connecting actions to slots...
    connect (settingsAction, &QAction::triggered, this, [=](bool checked) {
        Q_UNUSED(checked);
        FormSettings *settings = new FormSettings();
        settings->show();
    });
    connect (quitAction,     &QAction::triggered, this, [=](bool checked) {
        Q_UNUSED(checked);
        QString pass = QInputDialog::getText(0, tr("Вы уверены, что хотите выйти?"), tr("Пароль:"), QLineEdit::Password, "");
        if (pass == c_password) {
            qApp->quit();
        } else {
            QMessageBox::warning(0, tr("Ошибка!"), tr("Пароль введен неверно!"));
        }
    });

    // Setting system tray's icon menu...
    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addAction(quitAction);
    return trayIconMenu;
}
