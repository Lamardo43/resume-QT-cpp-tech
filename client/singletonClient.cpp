#include "singletonClient.h"
#include "../symbols.h"

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

void SingletonClient::send_msg_to_server(QString query) {
    mTcpSocket->write(query.toUtf8() + SPLIT_SYMBOL + END_SYMBOL);
}

QString message = "";

void SingletonClient::slotServerRead(){

    QString mes = "";

    while(mTcpSocket->bytesAvailable()>0)
    {
        mes.append(mTcpSocket->readAll());
    }

    message.append(mes);

    QList<QString> parts = message.split(SPLIT_SYMBOL);

    qDebug()<< message;

    //qDebug() << parts.first() + " " + parts.last();

    if (parts.last() == END_SYMBOL)
    {
        parts.removeLast();

        if (parts[0] == "get_screenshot_to") {
            emit get_scr(parts[1]);
            message.clear();
        }
        else if (parts[0] == "send_screenshot_to") {
            emit set_scr(message.toUtf8());
            message.clear();
        }
        else if (parts[0] == "send_clients") {
            emit get_client(message);
            message.clear();
        }
    }
}

SingletonClient::~SingletonClient(){
    mTcpSocket->close();
};



SingletonClient* SingletonClient::p_instance;
SingletonDestroyer SingletonClient::destroyer;
