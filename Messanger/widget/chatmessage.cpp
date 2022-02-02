#include "chatmessage.h"
#include "ui_chatmessage.h"
#include <QBuffer>

ChatMessage::ChatMessage(const Message &aMessage, QWidget *parent):
QWidget(parent), ui(new Ui::ChatMessage), message_(aMessage) {
    ui->setupUi(this);
    dataToUi();
}

ChatMessage::~ChatMessage() {
    delete ui;
}

MessageStatus ChatMessage::status() const {
    return message_.status;
}

void ChatMessage::setStatus(MessageStatus newStatus) {
    message_.status = newStatus;
    dataToUi();
}

const Message &ChatMessage::message() const {
    return message_;
}

void ChatMessage::setMessage(const Message &newMessage) {
    message_ = newMessage;
    dataToUi();
}

void ChatMessage::dataToUi() {
    ui->labelName->setText(message_.sendFrom);
    QStringList sFiles;
    for (auto file: message_.files) {
        sFiles << file.fileName;
    }

    ui->labelMessage->setText(message_.message + "(" + sFiles.join(",") + ")");
    ui->labelDate->setText(QLocale().toString(message_.cdate, "dd MMMM hh:mm"));
    switch(message_.status) {
    case MessageStatus::sendToServer: {
        ui->labelName->setText(ui->labelName->text() + " 1");
        ui->labelMessage->setStyleSheet("color: #555555;");
        ui->labelDate->setStyleSheet("color: black;");
        break;
    }
    case MessageStatus::successOnServer: {
        ui->labelName->setText(ui->labelName->text() + " 2");
        ui->labelMessage->setStyleSheet("color: black;");
        ui->labelDate->setStyleSheet("color: black;");
        break;
    }
    case MessageStatus::errorOnServer: {
        ui->labelName->setText(ui->labelName->text() + " 3");
        ui->labelMessage->setStyleSheet("color: black;");
        ui->labelDate->setStyleSheet("color: red;");
        break;
    }
    case MessageStatus::readedOnce: {
        ui->labelName->setText(ui->labelName->text() + " 4");
        ui->labelMessage->setStyleSheet("color: black;");
        ui->labelDate->setStyleSheet("color: aqua;");
        break;
    }
    case MessageStatus::readedAll: {
        ui->labelName->setText(ui->labelName->text() + " 5");
        ui->labelMessage->setStyleSheet("color: black;");
        ui->labelDate->setStyleSheet("color: blue;");
        break;
    }
    default: {
        ui->labelName->setText(ui->labelName->text() + " ???");
        ui->labelMessage->setStyleSheet("color: red;");
        ui->labelDate->setStyleSheet("color: red;");
    }
    }
}
