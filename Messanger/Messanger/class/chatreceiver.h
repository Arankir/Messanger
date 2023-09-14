#ifndef CHATRECEIVER_H
#define CHATRECEIVER_H

#include <QWidget>
#include "class/account.h"

class ChatReceiver : public QWidget {
    Q_OBJECT
public:
    explicit ChatReceiver(ReceiverType type, QWidget *parent = nullptr);

    ReceiverType type() const;

signals:

private:
    ReceiverType type_ = ReceiverType::unknown;
};

#endif // CHATRECEIVER_H
