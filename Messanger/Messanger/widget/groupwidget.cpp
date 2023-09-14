#include "groupwidget.h"
#include "ui_groupwidget.h"

GroupWidget::GroupWidget(Group group, QWidget *parent) :
ChatReceiver(ReceiverType::group, parent), ui(new Ui::GroupWidget), group_(group) {
    ui->setupUi(this);
    dataToUi();
}

GroupWidget::~GroupWidget() {
    delete ui;
}

const QString &GroupWidget::title() const {
    return group_.title;
}

void GroupWidget::setTitle(const QString &newTitle) {
    group_.title = newTitle;
}

const QList<Account> &GroupWidget::accounts() const {
    return group_.accounts;
}

void GroupWidget::setAccounts(const QList<Account> &newAccounts) {
    group_.accounts = newAccounts;
}

void GroupWidget::dataToUi() {
    ui->labelTitle->setText(group_.title);
    QStringList users;
    for (const auto &user: group_.accounts) {
        users.append(user.nickname);
    }
    QString sUsers = users.join(",");
    if (sUsers.length() > 20) {
        sUsers = sUsers.left(17) + "...";
    }
    ui->labelUsers->setText("(" + sUsers + ")");
}
