#include "functions.h"
#include "mytcpserver.h"

QByteArray parse(QString message, QTcpSocket* cTcpSocket) {

    QList<QString> parts = message.split(' ');

    if (parts[0] == "get_screenshot") {
        QHash<QTcpSocket*, QString> sockets = MyTcpServer::get_mTcpSocket();

        foreach (QTcpSocket* socket, sockets.keys()) {
            if (parts[1] == sockets[socket]) {
                socket->write("get_screenshot_to " + QHostAddress(cTcpSocket->peerAddress().toIPv4Address()).toString().toUtf8() + " ");
                return "_";
            }
        }
    }
    else if (parts[0] == "send_screenshot_to") {
        QHash<QTcpSocket*, QString> sockets = MyTcpServer::get_mTcpSocket();

        foreach (QTcpSocket* socket, sockets.keys()) {
            if (parts[1] == sockets[socket]) {
                QString part = "";

                for (int i = 2; i < parts.size(); i++)
                    part.append(parts[i]);

                socket->write("send_screenshot " + part.toUtf8() + " ");
                return "_";
            }
        }
    }
    else if (parts[0] == "get_clients") {
        return "send_clients" + get_clients();
    }

    return "Non_valid";
}

QByteArray get_clients() {
    QByteArray list = "";

    foreach (QString address, MyTcpServer::history.keys()) {
        list += " "  +  address.toUtf8() + "&" + MyTcpServer::history[address].toUtf8();
    }

    return list;
}
