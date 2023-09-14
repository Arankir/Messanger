#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QListWidgetItem>
#include "class/client.h"
#include "widget/chat.h"
#include "widget/popup.h"
#include "widget/accountwidget.h"
#include "widget/groupwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(MyClient *aClient, const QString &aToken, const QString &nickname, QWidget *parent = nullptr);
    ~MainWindow();

    void sendMessage(const Message &aMessage);
private slots:
    void clientRead(NetworkData aReply);

    void on_pushButtonReconnect_clicked();

    void on_pushButtonRelogin_clicked();

    void on_pushButtonTest_clicked();

    void on_pushButton_clicked();

signals:
    void s_relogin();

private:
    Ui::MainWindow *ui;
    MyClient *client_;
    QString token_;
    QString nickname_;

    QList<Receiver*> receivers_;

    void userClickedSlot(QListWidgetItem *aItem);
    void addUserToList(ChatReceiver *aChatReceiver);
    int findReceiver(const QString &aReceiver);
    void setItemToTop(const QString &aReceiver);
    void receiversToUi();
    void messageNotification(const Message &aMessage);
    QJsonObject jsonWithToken();
};
#endif // MAINWINDOW_H
