#include "account.h"

Account::Account(): Account("", "") {

}

Account::Account(const QString &aNickname, const QString &aStatus)
    : Receiver(ReceiverType::account), nickname(aNickname), status(aStatus) {

}

Account::Account(QJsonObject aObject): Account() {
    fromJson(aObject);
}

QJsonObject Account::toJson() const {
    QJsonObject obj;
    obj["nickname"] = nickname;
    obj["status"] = status;
    return obj;
}

void Account::fromJson(QJsonObject aObject) {
    nickname = aObject.value("nickname").toString();
    status = aObject.value("status").toString();
}
