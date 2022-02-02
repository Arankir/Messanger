#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QFileDialog>
#include <QMessageBox>
#include "chatmessage.h"

namespace Ui {
class Chat;
}

class Chat : public QWidget {
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    void addMessage(const Message&);
    void setMessages(const QList<Message>&);
    void changeMessageStatus(const Message &aMessage, MessageStatus aStatus);
    void setReceiver(const QString&, ReceiverType);
    void clear();

    const QString &user() const;
    void setUser(const QString &newUser);

signals:
    void s_sendMessage(const Message&);

private slots:
    void on_pushButtonSend_clicked();

    void on_pushButtonImage_clicked();

    void on_pushButtonFile_clicked();

private:
    Ui::Chat *ui;
    QString user_;
    ReceiverType type_;
    QImage image_;
    QList<File> files_;
};

#endif // CHAT_H
