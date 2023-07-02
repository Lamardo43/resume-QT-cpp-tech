#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "qtcpsocket.h"
#include <QByteArray>
#include <QGuiApplication>
#include <QPixmap>

void parse(QByteArray, QTcpSocket*);

QByteArray get_clients();

#endif // FUNCTIONS_H
