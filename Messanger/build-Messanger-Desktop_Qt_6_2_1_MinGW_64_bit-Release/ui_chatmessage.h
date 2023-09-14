/********************************************************************************
** Form generated from reading UI file 'chatmessage.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATMESSAGE_H
#define UI_CHATMESSAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatMessage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelName;
    QLabel *labelMessage;
    QLabel *labelDate;

    void setupUi(QWidget *ChatMessage)
    {
        if (ChatMessage->objectName().isEmpty())
            ChatMessage->setObjectName(QString::fromUtf8("ChatMessage"));
        ChatMessage->resize(400, 300);
        verticalLayout = new QVBoxLayout(ChatMessage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelName = new QLabel(ChatMessage);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        verticalLayout->addWidget(labelName);

        labelMessage = new QLabel(ChatMessage);
        labelMessage->setObjectName(QString::fromUtf8("labelMessage"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelMessage->sizePolicy().hasHeightForWidth());
        labelMessage->setSizePolicy(sizePolicy);
        labelMessage->setWordWrap(true);

        verticalLayout->addWidget(labelMessage);

        labelDate = new QLabel(ChatMessage);
        labelDate->setObjectName(QString::fromUtf8("labelDate"));
        labelDate->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(labelDate);


        retranslateUi(ChatMessage);

        QMetaObject::connectSlotsByName(ChatMessage);
    } // setupUi

    void retranslateUi(QWidget *ChatMessage)
    {
        ChatMessage->setWindowTitle(QCoreApplication::translate("ChatMessage", "Form", nullptr));
        labelName->setText(QString());
        labelMessage->setText(QString());
        labelDate->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatMessage: public Ui_ChatMessage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATMESSAGE_H
