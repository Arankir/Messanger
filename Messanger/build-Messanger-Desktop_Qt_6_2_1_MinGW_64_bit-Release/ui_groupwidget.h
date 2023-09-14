/********************************************************************************
** Form generated from reading UI file 'groupwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPWIDGET_H
#define UI_GROUPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *labelTitle;
    QLabel *labelUsers;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *GroupWidget)
    {
        if (GroupWidget->objectName().isEmpty())
            GroupWidget->setObjectName(QString::fromUtf8("GroupWidget"));
        GroupWidget->resize(73, 41);
        horizontalLayout = new QHBoxLayout(GroupWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelTitle = new QLabel(GroupWidget);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));

        horizontalLayout->addWidget(labelTitle);

        labelUsers = new QLabel(GroupWidget);
        labelUsers->setObjectName(QString::fromUtf8("labelUsers"));

        horizontalLayout->addWidget(labelUsers);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(GroupWidget);

        QMetaObject::connectSlotsByName(GroupWidget);
    } // setupUi

    void retranslateUi(QWidget *GroupWidget)
    {
        GroupWidget->setWindowTitle(QCoreApplication::translate("GroupWidget", "Form", nullptr));
        labelTitle->setText(QString());
        labelUsers->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GroupWidget: public Ui_GroupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPWIDGET_H
