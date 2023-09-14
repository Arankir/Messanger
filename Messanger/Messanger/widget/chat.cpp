#include "chat.h"
#include "ui_chat.h"
#include <QBuffer>

Chat::Chat(QWidget *parent) :
QWidget(parent),
ui(new Ui::Chat) {
    ui->setupUi(this);
    ui->listWidget->setAlternatingRowColors(true);
}

Chat::~Chat() {
    delete ui;
}

void Chat::addMessage(const Message &aMessage) {
    ChatMessage *chatMessage = new ChatMessage(aMessage);
    auto item = new QListWidgetItem();
    ui->listWidget->addItem(item);
    item->setSizeHint(chatMessage->sizeHint());
    ui->listWidget->setItemWidget(item, chatMessage);
}

void Chat::setMessages(const QList<Message> &aMessages) {
    clear();
    for (auto message: aMessages) {
        addMessage(message);
    }
    ui->listWidget->scrollToBottom();
}

void Chat::changeMessageStatus(const Message &aMessage, MessageStatus aStatus) {
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if (auto chatMessage = dynamic_cast<ChatMessage*>(ui->listWidget->itemWidget(ui->listWidget->item(i)))) {
            if (chatMessage->message().getLocalVcode() == aMessage.getLocalVcode()) {
                chatMessage->setStatus(aStatus);
            }
        }
    }
}

void Chat::setReceiver(const QString &aReceiver, ReceiverType aType) {
    ui->labelReceiver->setText(aReceiver);
    type_ = aType;
}

void Chat::clear() {
    ui->listWidget->clear();
}

void Chat::on_pushButtonSend_clicked() {
    Message message;
    message.message = ui->textEditMessage->toPlainText();
    message.sendFrom = user_;
    message.sendTo = ui->labelReceiver->text();
    message.type = type_;
    message.cdate = QDateTime::currentDateTime();
    message.image = image_;
    message.files = files_;
    message.status = MessageStatus::sendToServer;
    emit s_sendMessage(message);
    addMessage(message);
    ui->listWidget->scrollToBottom();
    files_.clear();
    ui->textEditMessage->clear();
}

void Chat::on_pushButtonImage_clicked() {

}

const QString &Chat::user() const {
    return user_;
}

void Chat::setUser(const QString &newUser) {
    user_ = newUser;
}

void Chat::on_pushButtonFile_clicked() {
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Files (*)"));
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        for (const auto &filePath: fileNames) {
            if (files_.size() >= 10) {
                QMessageBox::warning(this, tr("Внимание!"), tr("К сообщению можно прикрепить не более 10 файлов!"));
                break;
            }
            files_ << File(filePath);
        }
    }
}

