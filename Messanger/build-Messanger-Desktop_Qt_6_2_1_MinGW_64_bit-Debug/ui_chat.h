/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *labelReceiver;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *labelImage;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEditMessage;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonSend;
    QPushButton *pushButtonImage;

    void setupUi(QWidget *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(515, 585);
        verticalLayout_2 = new QVBoxLayout(Chat);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelReceiver = new QLabel(Chat);
        labelReceiver->setObjectName(QString::fromUtf8("labelReceiver"));

        verticalLayout_2->addWidget(labelReceiver);

        listWidget = new QListWidget(Chat);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_2->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelImage = new QLabel(Chat);
        labelImage->setObjectName(QString::fromUtf8("labelImage"));

        horizontalLayout->addWidget(labelImage);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEditMessage = new QTextEdit(Chat);
        textEditMessage->setObjectName(QString::fromUtf8("textEditMessage"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEditMessage->sizePolicy().hasHeightForWidth());
        textEditMessage->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(textEditMessage);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButtonSend = new QPushButton(Chat);
        pushButtonSend->setObjectName(QString::fromUtf8("pushButtonSend"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButtonSend->sizePolicy().hasHeightForWidth());
        pushButtonSend->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(pushButtonSend);

        pushButtonImage = new QPushButton(Chat);
        pushButtonImage->setObjectName(QString::fromUtf8("pushButtonImage"));

        verticalLayout->addWidget(pushButtonImage);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QWidget *Chat)
    {
        Chat->setWindowTitle(QCoreApplication::translate("Chat", "Form", nullptr));
        labelReceiver->setText(QString());
        labelImage->setText(QString());
        pushButtonSend->setText(QCoreApplication::translate("Chat", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", nullptr));
        pushButtonImage->setText(QCoreApplication::translate("Chat", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
