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

void SingletonClient::send_msg_to_server(QByteArray query) {
    mTcpSocket->write(query + SPLIT_SYMBOL + END_SYMBOL);
}

QByteArray message = "";

void SingletonClient::slotServerRead(){

    QByteArray mes = "";

    while(mTcpSocket->bytesAvailable()>0)
    {
        mes.append(mTcpSocket->readAll());
    }

    message.append(mes);

    QList<QByteArray> parts = message.split(SPLIT_SYMBOL.at(0));

    qDebug()<< message;

    if (parts.last() == END_SYMBOL)
    {
        //qDebug() << parts.first() + " " + parts.last();

        parts.removeLast();

        if (parts[0] == "get_screenshot_to") {
            emit get_scr(parts[1]);
            message.clear();
        }
        else if (parts[0] == "send_screenshot") {
            emit set_scr(parts.mid(1, parts.size()).join(""));
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
