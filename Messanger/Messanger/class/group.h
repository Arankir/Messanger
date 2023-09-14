#ifndef GROUP_H
#define GROUP_H

#include <QWidget>
#include <QJsonArray>
#include "class/account.h"

struct Group: public Receiver {
    Group();
    Group(const QString &aTitle, const QList<Account> &aAccounts);
    Group(QJsonObject);
    QJsonObject toJson() const;
    void fromJson(QJsonObject);
    friend QDebug operator<<(QDebug dbg, const Group &a) {
        dbg.nospace() << "Group(" << a.toJson() << ")\n";
        return dbg.space();
    }

    QString title;
    QList<Account> accounts;
};

#endif // GROUP_H
