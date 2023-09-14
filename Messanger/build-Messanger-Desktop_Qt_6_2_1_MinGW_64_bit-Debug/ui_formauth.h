/********************************************************************************
** Form generated from reading UI file 'formauth.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMAUTH_H
#define UI_FORMAUTH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormAuth
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEditLogin;
    QLabel *label_2;
    QLineEdit *lineEditPassword;
    QPushButton *pushButtonSendAuth;

    void setupUi(QWidget *FormAuth)
    {
        if (FormAuth->objectName().isEmpty())
            FormAuth->setObjectName(QString::fromUtf8("FormAuth"));
        FormAuth->resize(174, 102);
        formLayout = new QFormLayout(FormAuth);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(FormAuth);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEditLogin = new QLineEdit(FormAuth);
        lineEditLogin->setObjectName(QString::fromUtf8("lineEditLogin"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditLogin);

        label_2 = new QLabel(FormAuth);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEditPassword = new QLineEdit(FormAuth);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

        pushButtonSendAuth = new QPushButton(FormAuth);
        pushButtonSendAuth->setObjectName(QString::fromUtf8("pushButtonSendAuth"));

        formLayout->setWidget(2, QFormLayout::SpanningRole, pushButtonSendAuth);


        retranslateUi(FormAuth);

        QMetaObject::connectSlotsByName(FormAuth);
    } // setupUi

    void retranslateUi(QWidget *FormAuth)
    {
        FormAuth->setWindowTitle(QCoreApplication::translate("FormAuth", "Form", nullptr));
        label->setText(QCoreApplication::translate("FormAuth", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        label_2->setText(QCoreApplication::translate("FormAuth", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        pushButtonSendAuth->setText(QCoreApplication::translate("FormAuth", "\320\222\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormAuth: public Ui_FormAuth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMAUTH_H
