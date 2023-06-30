#include "singletonClient.h"

SingletonClient::SingletonClient(QObject *parent) : QObject(parent){
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket -> connectToHost("127.0.0.1",33333);
    connect(mTcpSocket, &QTcpSocket::readyRead,
            this, &SingletonClient::slotServerRead);
}
SingletonClient* SingletonClient::getInstance(){
    if (!p_instance)
        {
            p_instance = new SingletonClient();
            destroyer.initialize(p_instance);
        }
    return p_instance;
}

void SingletonClient::send_msg_to_server(QString query){
    mTcpSocket->write(query.toUtf8());
}

void SingletonClient::slotServerRead(){
    QString message = "";

    while(mTcpSocket->bytesAvailable()>0)
    {
        message.append(mTcpSocket->readAll());
    }

    qDebug()<<message;

    QList<QString>parts = message.split(" ");

    if (parts[0] == "get_screenshot_to") {
        SingletonClient::getInstance()->send_msg_to_server("send_screenshot_to " + parts[1] + " " + emit get_scr());
    }
    else if (parts[0] == "send_screenshot") {
        emit set_scr(parts[1].toUtf8());
    }
    else if (parts[0] == "send_clients") {
        emit get_client(message);
    }
}

SingletonClient::~SingletonClient(){
    mTcpSocket->close();
};



SingletonClient* SingletonClient::p_instance;
SingletonDestroyer SingletonClient::destroyer;
