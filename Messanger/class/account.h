#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QJsonObject>

enum class ReceiverType {
    account,
    group,
    unknown
};

struct Receiver {
    Receiver(ReceiverType type = ReceiverType::unknown): type_(type) {}

    ReceiverType type_;
};

struct Account: public Receiver {
    Account();
    Account(const QString &aNickname, const QString &aStatus);
    Account(QJsonObject);
    friend QDebug operator<<(QDebug dbg, const Account &a) {
        dbg.nospace() << "Account(" << a.toJson() << ")\n";
        return dbg.space();
    }
    QJsonObject toJson() const;
    void fromJson(QJsonObject);
    QString nickname;
    QString status;
};

#endif // ACCOUNT_H
