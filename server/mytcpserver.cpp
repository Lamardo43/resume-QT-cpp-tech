#include "mytcpserver.h"
#include "functions.h"
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

QList<QTcpSocket*> MyTcpServer::mTcpSocket;
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

        cTcpSocket->write("You are connected.");

        connect(cTcpSocket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(cTcpSocket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);

        history[QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString()] = "Адрес_подключен_в_" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
        mTcpSocket.append(cTcpSocket);
    }
}

void MyTcpServer::slotServerRead(){
    QByteArray array;
    QTcpSocket* cTcpSocket = (QTcpSocket*)sender();
    while(cTcpSocket->bytesAvailable()>0)
    {
        array.append(cTcpSocket->readAll());
    }

    qDebug() << array.replace("\r", "").replace("\n", "");
    cTcpSocket->write(parse(array));
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* cTcpSocket = (QTcpSocket*)sender();

    cTcpSocket->close();

    mTcpSocket.removeOne(cTcpSocket);

    history[QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString()] = "Адрес_отключен_в_" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
}

QList<QTcpSocket*> MyTcpServer::get_mTcpSocket(){
    return MyTcpServer::mTcpSocket;
}
