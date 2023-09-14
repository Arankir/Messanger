/********************************************************************************
** Form generated from reading UI file 'accountwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACCOUNTWIDGET_H
#define UI_ACCOUNTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AccountWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *labelName;
    QSpacerItem *horizontalSpacer;
    QLabel *labelStatus;

    void setupUi(QWidget *AccountWidget)
    {
        if (AccountWidget->objectName().isEmpty())
            AccountWidget->setObjectName(QString::fromUtf8("AccountWidget"));
        AccountWidget->resize(73, 41);
        horizontalLayout = new QHBoxLayout(AccountWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelName = new QLabel(AccountWidget);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        horizontalLayout->addWidget(labelName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelStatus = new QLabel(AccountWidget);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));

        horizontalLayout->addWidget(labelStatus);


        retranslateUi(AccountWidget);

        QMetaObject::connectSlotsByName(AccountWidget);
    } // setupUi

    void retranslateUi(QWidget *AccountWidget)
    {
        AccountWidget->setWindowTitle(QCoreApplication::translate("AccountWidget", "Form", nullptr));
        labelName->setText(QString());
        labelStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AccountWidget: public Ui_AccountWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACCOUNTWIDGET_H
