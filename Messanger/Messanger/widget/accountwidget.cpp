#include "accountwidget.h"
#include "ui_accountwidget.h"

//const QColor c_border = QColor("333333");
//const QColor c_hoverBorder = QColor("ff0000");

AccountWidget::AccountWidget(const Account &account, QWidget *parent):
    ChatReceiver(ReceiverType::account, parent), ui(new Ui::AccountWidget), account_(account) {
    ui->setupUi(this);
    dataToUi();
//    setAttribute(Qt::WA_StyledBackground, true);
//    setStyleSheet("User { "
//                        "border: 2 solid " + c_border.name() + "; "
//                        "background-color: rgb(204, 231, 47); "
//                        "color: white; "
//                        "border-radius: 10px; "
//                    "} "
//                 "User:hover { "
//                        "border: 2 solid " + c_hoverBorder.name() + "; "
//                    "} ");
}

AccountWidget::~AccountWidget() {
    delete ui;
}

const QString &AccountWidget::nickname() const {
    return account_.nickname;
}

void AccountWidget::setNickname(const QString &newNickname) {
    account_.nickname = newNickname;
    dataToUi();
}

const QString &AccountWidget::status() const {
    return account_.status;
}

void AccountWidget::setStatus(const QString &newStatus) {
    account_.status = newStatus;
    dataToUi();
}

void AccountWidget::dataToUi() {
    ui->labelName->setText(account_.nickname);
    if (account_.status == "online") {
        ui->labelStatus->setStyleSheet("color: #007700;");
        ui->labelStatus->setText(tr("В сети"));
    } else if (account_.status == "offline") {
        ui->labelStatus->setStyleSheet("color: #770000;");
        ui->labelStatus->setText(tr("Не в сети"));
    } else if (account_.status == "away") {
        ui->labelStatus->setStyleSheet("color: #333333;");
        ui->labelStatus->setText(tr("Отошел"));
    } else {
        ui->labelStatus->setText(tr("Неизвестно"));
    }
}
