#include "functions.h"
#include "mytcpserver.h"
#include "../symbols.h"

QString message = "";

QByteArray parse(QString mes, QTcpSocket* cTcpSocket) {

    message.append(mes);

    QList<QString> parts = message.split(SPLIT_SYMBOL);

    if (parts.last() == END_SYMBOL)
    {

        qDebug() << message;

        parts.removeLast();

        if (parts[0] == "get_screenshot") {
            message.clear();
            QHash<QTcpSocket*, QString> sockets = MyTcpServer::get_mTcpSocket();

            foreach (QTcpSocket* socket, sockets.keys()) {
                if (parts[1] == sockets[socket]) {
                    socket->write("get_screenshot_to" + SPLIT_SYMBOL + QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString().toUtf8() + SPLIT_SYMBOL + END_SYMBOL);
                    return "_";
                }
            }
        }
        else if (parts[0] == "send_screenshot_to") {
            message.clear();
            QHash<QTcpSocket*, QString> sockets = MyTcpServer::get_mTcpSocket();

            foreach (QTcpSocket* socket, sockets.keys()) {
                if (parts[1] == sockets[socket]) {
                    QString part = "";

                    part = parts.mid(2, parts.size()).join("");

                    socket->write("send_screenshot" + SPLIT_SYMBOL + part.toUtf8()  + SPLIT_SYMBOL + END_SYMBOL);
                    return "_";
                }
            }
        }
        else if (parts[0] == "get_clients") {
            message.clear();
            return "send_clients" + get_clients();
        }
    }

    return "";
}

QByteArray get_clients() {
    QByteArray list = "";

    foreach (QString address, MyTcpServer::history.keys()) {
        list += SPLIT_SYMBOL  +  address.toUtf8() + "&" + MyTcpServer::history[address].toUtf8();
    }

    return list;
}
