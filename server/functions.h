#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "qtcpsocket.h"
#include <QByteArray>
#include <QGuiApplication>
#include <QPixmap>

QByteArray parse(QString, QTcpSocket*);

QByteArray get_clients();

#endif // FUNCTIONS_H
