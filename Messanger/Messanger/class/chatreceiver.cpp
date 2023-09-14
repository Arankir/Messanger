#include "chatreceiver.h"

ChatReceiver::ChatReceiver(ReceiverType type, QWidget *parent)
: QWidget{parent}, type_(type) {

}

ReceiverType ChatReceiver::type() const {
    return type_;
}
