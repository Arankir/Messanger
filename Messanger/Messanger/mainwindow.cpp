#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(MyClient *aClient, const QString &aToken, const QString &nickname, QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow), client_(aClient), token_(aToken), nickname_(nickname) {
    ui->setupUi(this);
    ui->listWidgetUsers->setAlternatingRowColors(true);
    ui->chat->setUser(nickname_);

    NetworkData data("chats", jsonWithToken());
    client_->writeData(data);

    connect(client_, &MyClient::s_read, this, &MainWindow::clientRead);
    connect(client_, &MyClient::disconnected, client_, &MyClient::reconnect);
    connect(ui->listWidgetUsers, &QListWidget::itemClicked, this, &MainWindow::userClickedSlot);
    connect(ui->chat, &Chat::s_sendMessage, this, &MainWindow::sendMessage);
}

MainWindow::~MainWindow() {
    delete ui;
}

QJsonObject MainWindow::jsonWithToken() {
    QJsonObject obj;
    obj["token"] = token_;
    return obj;
}

void MainWindow::messageNotification(const Message &aMessage) {
    PopUp *ppp = new PopUp();
    ppp->setPopupText(aMessage.message);
    ppp->show(-1);
}

void MainWindow::clientRead(NetworkData aReply) {
//    qDebug() << "client read" << aReply.requestType << aReply.value;
    if (aReply.requestType == "logOn") {

    }
    if (aReply.requestType == "sendMessage") {
        auto obj = aReply.value.toObject();
        Message message = Message::fromJson(obj.value("message").toObject());
        if (obj.value("status").toString() == "error") {
            ui->chat->changeMessageStatus(message, MessageStatus::errorOnServer);
        } else if (obj.value("status").toString() == "success") {
            ui->chat->changeMessageStatus(message, MessageStatus::successOnServer);
        }
    }
    if (aReply.requestType == "showMessage") {
        auto obj = aReply.value.toObject();
        QList<Message> messages;
        for (const QJsonValue &jMessage: obj.value("messages").toArray()) {
            messages << Message::fromJson(jMessage.toObject());
        }
        ui->chat->setMessages(messages);
    }
    if (aReply.requestType == "onlineUsers") {
        auto obj = aReply.value.toObject();
        ui->listWidgetUsers->clear();
        for (const QJsonValue &user: obj.value("users").toArray()) {
            addUserToList(new AccountWidget(Account(user.toObject().value("nickname").toString(), user.toObject().value("status").toString())));
        }

        NetworkData data("chats");
        QJsonObject obj2;
        obj2["token"] = token_;
        data.value = obj2;
        client_->writeData(data);
    }
    if (aReply.requestType == "groups") {
        auto obj = aReply.value.toObject();
        for (const auto chat: obj.value("groups").toArray()) {
            QList<Account> accounts;
            for (const auto user: chat.toObject().value("users").toArray()) {
                accounts << Account(user.toObject().value("nickname").toString(), user.toObject().value("status").toString());
            }
            Group group;
            group.title = chat.toObject().value("title").toString();
            group.accounts = accounts;
            addUserToList(new GroupWidget(group));
//            ui->textEdit->append(QString("\n%1 (%2)").arg(chat.toObject().value("title").toString(), users.join(",")));
        }
    }
    if (aReply.requestType == "chats") {
        auto obj = aReply.value.toObject();
        receivers_.clear();
        for (const auto chat: obj.value("chats").toArray()) {
            if (chat.toObject().value("type").toString() == "account") {
                receivers_.append(new Account(chat.toObject().value("account").toObject()));
            }
            if (chat.toObject().value("type").toString() == "group") {
                receivers_.append(new Group(chat.toObject().value("group").toObject()));
            }
        }
        receiversToUi();
    }
    if (aReply.requestType == "newInfo") {
        auto obj = aReply.value.toObject();
        if (!obj.value("message").toObject().isEmpty()) {
            Message messageCh = Message::fromJson(obj.value("message").toObject());
            qDebug() << messageCh.sendFrom << ui->chat->user();
            if (messageCh.sendFrom == ui->chat->user()) {
                ui->chat->addMessage(messageCh);
            } else {
                messageNotification(messageCh);
            }
            setItemToTop(messageCh.sendFrom);
        }
        if (!obj.value("messageShowed").toObject().isEmpty()) {
            Message messageCh = Message::fromJson(obj.value("message").toObject());
            ui->chat->changeMessageStatus(messageCh, MessageStatus::readedOnce);
        }
        if (!obj.value("user").toObject().isEmpty()) {
            QJsonObject user = obj.value("user").toObject();
            Account *account = new Account(user);
            int row = findReceiver(account->nickname);
            delete (receivers_[row]);
            receivers_[row] = account;
            receiversToUi();
//            QString nickname = user.value("nickName").toString();
//            QString status = user.value("status").toString();
//            bool isFind = false;
//            for (int i = 0; i < ui->listWidgetUsers->count(); ++i) {
//                if (auto widget = dynamic_cast<AccountWidget*>(ui->listWidgetUsers->itemWidget(ui->listWidgetUsers->item(i)))) {
//                    if (widget->nickname() == nickname) {
//                        widget->setStatus(status);
//                        isFind = true;
//                        break;
//                    }
//                }
//            }
//            if (!isFind) {
//                addUserToList(new AccountWidget(Account(nickname, status)));
//            }
        }
    }
    if (aReply.requestType == "file") {
        auto obj = aReply.value.toObject();
        File file = File::fromJson(obj.value("file").toObject());
        if (obj.value("status").toString() == "success") {
            qDebug() << "fileVcode=" << file.vcode << file.getLocalVcode();
        }
        if (obj.value("status").toString() == "error") {
            qDebug() << "fileError=" << obj.value("error").toString();
        }
    }
}

void MainWindow::receiversToUi() {
    ui->listWidgetUsers->clear();
    for (auto receiver: receivers_) {
        switch(receiver->type_) {
        case ReceiverType::account: {
            addUserToList(new AccountWidget(*(static_cast<Account*>(receiver))));
            break;
        }
        case ReceiverType::group: {
            addUserToList(new GroupWidget(*(static_cast<Group*>(receiver))));
            break;
        }
        default: {
            break;
        }
        }
    }
}

void MainWindow::addUserToList(ChatReceiver *aChatReceiver) {
    auto item = new QListWidgetItem();
    item->setSizeHint(aChatReceiver->sizeHint());
    ui->listWidgetUsers->addItem(item);
    ui->listWidgetUsers->setItemWidget(item, aChatReceiver);
}

void MainWindow::userClickedSlot(QListWidgetItem *aItem) {
    QWidget *widget = ui->listWidgetUsers->itemWidget(aItem);
    if (auto uWidget = dynamic_cast<AccountWidget*>(widget)) {
        ui->chat->setReceiver(uWidget->nickname(), ReceiverType::account);
        NetworkData data("showMessage");
        QJsonObject obj;
        obj["token"] = token_;
        obj["accountTo"] = uWidget->nickname();
        obj["chatTo"] = "";
        data.value = obj;
        client_->writeData(data);
    }
    if (auto gWidget = dynamic_cast<GroupWidget*>(widget)) {
        ui->chat->setReceiver(gWidget->title(), ReceiverType::group);
        NetworkData data("showMessage");
        QJsonObject obj;
        obj["token"] = token_;
        obj["accountTo"] = "";
        obj["chatTo"] = gWidget->title();
        data.value = obj;
        client_->writeData(data);
    }
}

int MainWindow::findReceiver(const QString &aReceiver) {
    for (int i = 0; i < receivers_.count(); ++i) {
        switch(receivers_[i]->type_) {
        case ReceiverType::account: {
            if ((*(static_cast<Account*>(receivers_[i]))).nickname == aReceiver) {
                return i;
            }
            break;
        }
        case ReceiverType::group: {
            if ((*(static_cast<Group*>(receivers_[i]))).title == aReceiver) {
                return i;
            }
            break;
        }
        default: {
            break;
        }
        }
    }
    return -1;
}

void MainWindow::setItemToTop(const QString &aReceiver) {
    int row = findReceiver(aReceiver);
    qDebug() << row;
    if (row >= 0) {
        auto item = receivers_.takeAt(row);
        receivers_.push_front(item);
        receiversToUi();
    }
}

void MainWindow::sendMessage(const Message &aMessage) {
    NetworkData data("sendMessage");
    QJsonObject obj = aMessage.toJson();

    setItemToTop(aMessage.sendTo);

    obj["token"] = token_;
    data.value = obj;
    client_->writeData(data);
}

void MainWindow::on_pushButtonReconnect_clicked() {
    client_->reconnect();
}

void MainWindow::on_pushButtonRelogin_clicked() {
    emit s_relogin();
    close();
}

void MainWindow::on_pushButtonTest_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Выберите файл для отправки"), "С://", tr("Отправочные файлы (*)"));
//    qDebug() << fileName;
    if (!filePath.isEmpty()) {
        File file(filePath);
        QJsonObject object = jsonWithToken();
        object["file"] = file.toJson();
        client_->writeData(NetworkData("file", object));
    }
}

void MainWindow::on_pushButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Выберите файл для отправки"), "С://", tr("Отправочные файлы (*)"));
//    qDebug() << fileName;
    if (!filePath.isEmpty()) {
        File file(filePath);
        QJsonObject object = jsonWithToken();
        object["file"] = file.toJson();
//        qInfo() << object;
        client_->writeData(NetworkData("file", object));
    }
}

