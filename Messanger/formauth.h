#ifndef FORMAUTH_H
#define FORMAUTH_H

#include <QWidget>
#include <QKeyEvent>
#include "mainwindow.h"

namespace Ui {
class FormAuth;
}

class FormAuth : public QWidget {
    Q_OBJECT

public:
    explicit FormAuth(QWidget *parent = nullptr);
    ~FormAuth();

protected:
    void initClient();
    void clientRead(NetworkData aReply);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void on_pushButtonSendAuth_clicked();

private:
    Ui::FormAuth *ui;
    MyClient *client_;
};

#endif // FORMAUTH_H
