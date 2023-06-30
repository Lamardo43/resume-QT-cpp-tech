#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
    static QHash<QTcpSocket*, QString> get_mTcpSocket();
    static QHash<QString, QString> history;
public slots:
    /// \brief slotNewConnection отвечает за подключение клиента к серверу
    /// \param void
    /// \return void
    ///
    void slotNewConnection();

    /// \brief slotClientDisconnected отвечает за отключение клиента от сервера
    /// \param void
    /// \return void
    ///
    void slotClientDisconnected();

    /// \brief slotServerRead отвечает за отслеживание информации от клиента
    /// \param void
    /// \return void
    ///
    void slotServerRead();
//    void slotReadClient();
private:
    QTcpServer * mTcpServer;
    static QHash<QTcpSocket*, QString> mTcpSocket;
    int server_status;
};
#endif // MYTCPSERVER_H







