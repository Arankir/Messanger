#include "formauth.h"
#include "ui_formauth.h"
#include <QMessageBox>

const QString c_serverIP = "10.10.12.96";
const int c_port = 9999;

FormAuth::FormAuth(QWidget *parent) :
QWidget(parent),
ui(new Ui::FormAuth) {
    ui->setupUi(this);
    initClient();
}

FormAuth::~FormAuth() {
    delete ui;
}

void FormAuth::initClient() {
    client_ = new MyClient();
    QObject::connect(client_, &QTcpSocket::hostFound, this, [&]() {
        qDebug() << "host finded";
    });
    QObject::connect(client_, &QTcpSocket::connected, this, [&]() {
        qDebug() << "connect finished";
    });
    QObject::connect(client_, &QTcpSocket::disconnected, this, [&]() {
        qDebug() << "disconnect";
    });
    connect(client_, &MyClient::s_read, this, &FormAuth::clientRead);
    client_->connect(QHostAddress(c_serverIP), c_port);
}

void FormAuth::keyPressEvent(QKeyEvent *event) {
//    qDebug() << event->key() << "\t" << Qt::Key_Enter << "\t" << QKeyEvent::Enter;
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
        on_pushButtonSendAuth_clicked();
    else
        QWidget::keyPressEvent(event);
}

void FormAuth::on_pushButtonSendAuth_clicked() {
    NetworkData data("logOn");
    QJsonObject obj;
    obj["login"] = ui->lineEditLogin->text();
    obj["password"] = ui->lineEditPassword->text();
    data.value = obj;
    client_->writeData(data);
}

void FormAuth::clientRead(NetworkData aReply) {
    qDebug() << "client read" << aReply.requestType << aReply.value;
    if (aReply.requestType == "logOn") {
        auto obj = aReply.value.toObject();
        if (obj.value("status").toString() == "error") {
            QMessageBox::warning(this, tr("Ошибка"), obj.value("error").toString());
        } else if (obj.value("status").toString() == "success") {
            auto mainWindow = new MainWindow(client_, obj.value("token").toString(), obj.value("nickname").toString());
            mainWindow->show();
//            disconnect(client_, &MyClient::s_read, this, &FormAuth::clientRead);
            connect (mainWindow, &MainWindow::s_relogin, this, [&]() {
                show();
//                mainWindow->close();
            });
            hide();
        }
    }
}
