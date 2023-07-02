#include "mytcpserver.h"
#include "functions.h"
#include "../symbols.h"
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

QHash<QTcpSocket*, QString> MyTcpServer::mTcpSocket;
QHash<QString, QString> MyTcpServer::history;

MyTcpServer::~MyTcpServer()
{
    mTcpSocket.clear();
    mTcpServer->close();
    server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){

    if(server_status==1){
        QTcpSocket* cTcpSocket;

        cTcpSocket = mTcpServer->nextPendingConnection();

        connect(cTcpSocket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(cTcpSocket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);

        history[QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString()] = "Адрес_подключен_в_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        mTcpSocket[cTcpSocket] = QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString();
    }
}

void MyTcpServer::slotServerRead(){
    QByteArray array;
    QTcpSocket* cTcpSocket = (QTcpSocket*)sender();

    while(cTcpSocket->bytesAvailable()>0)
    {
        array.append(cTcpSocket->readAll());
    }

    //qDebug() << array;

    parse(array, cTcpSocket);
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* cTcpSocket = (QTcpSocket*)sender();

    cTcpSocket->close();

    mTcpSocket.remove(cTcpSocket);

    history[QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString()] = "Адрес_отключен_в_" + QDateTime::currentDateTime().toString("yyyy-MM-dd-_hh:mm:ss");
}

QHash<QTcpSocket*, QString> MyTcpServer::get_mTcpSocket(){
    return MyTcpServer::mTcpSocket;
}
