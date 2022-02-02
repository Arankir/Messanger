#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include "class/group.h"
#include "class/chatreceiver.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public ChatReceiver {
    Q_OBJECT

public:
    explicit GroupWidget(Group group, QWidget *parent = nullptr);
    ~GroupWidget();

    const QString &title() const;
    void setTitle(const QString &newTitle);

    const QList<Account> &accounts() const;
    void setAccounts(const QList<Account> &newAccounts);

private:
    Ui::GroupWidget *ui;
    Group group_;

    void dataToUi();
};

#endif // GROUPWIDGET_H
