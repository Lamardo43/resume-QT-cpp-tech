#include "functions.h"
#include "mytcpserver.h"

QByteArray parse(QByteArray message) {

    QList<QByteArray> parts = message.split(' ');

    if (parts[0] == "get_clients") {
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
