#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>
#include <QDateTime>
#include <QJsonObject>
#include "class/message.h"

namespace Ui {
class ChatMessage;
}

class ChatMessage : public QWidget {
    Q_OBJECT

public:
    explicit ChatMessage(const Message &aMessage, QWidget *parent = nullptr);
    ~ChatMessage();

    MessageStatus status() const;
    void setStatus(MessageStatus newStatus);

    const Message &message() const;
    void setMessage(const Message &newMessage);

private:
    Ui::ChatMessage *ui;
    Message message_;

    void dataToUi();
};

#endif // CHATMESSAGE_H
