#include "group.h"

Group::Group(): Group("", QList<Account>()) {

}

Group::Group(const QString &aTitle, const QList<Account> &aAccounts):
    Receiver(ReceiverType::group), title(aTitle), accounts(aAccounts) {

}

Group::Group(QJsonObject aObject): Group() {
    fromJson(aObject);
}

QJsonObject Group::toJson() const {
    QJsonObject obj;
    obj["title"] = title;

    QJsonArray array;
    for (const auto &account: accounts) {
        array.append(account.toJson());
    }
    obj["users"] = array;
    return obj;
}

void Group::fromJson(QJsonObject aObject) {
    title = aObject.value("title").toString();
    auto array = aObject.value("users").toArray();
    for (const auto &user: array) {
        Account acc;
        acc.fromJson(user.toObject());
        accounts << acc;
    }
}
