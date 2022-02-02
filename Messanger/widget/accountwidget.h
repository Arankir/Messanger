#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include "class/account.h"
#include "class/chatreceiver.h"

namespace Ui {
class AccountWidget;
}

class AccountWidget : public ChatReceiver {
    Q_OBJECT

public:
    explicit AccountWidget(const Account &account, QWidget *parent = nullptr);
    ~AccountWidget();

    const QString &nickname() const;
    void setNickname(const QString &newNickname);

    const QString &status() const;
    void setStatus(const QString &newStatus);

private:
    Ui::AccountWidget *ui;
    Account account_;

    void dataToUi();
};

#endif // ACCOUNTWIDGET_H
