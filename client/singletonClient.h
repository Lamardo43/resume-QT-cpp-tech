#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H
#include<QDebug>
#include<QObject>
#include<QtNetwork>
#include<QTcpSocket>
#include<QByteArray>
#include<QString>

class SingletonClient;

class SingletonDestroyer
{
    private:
        SingletonClient * p_instance;
    public:
        ~SingletonDestroyer() { delete p_instance; }

        void initialize(SingletonClient * p){p_instance = p;};
};


class SingletonClient : public QObject
{
    Q_OBJECT
    private:
//      int access_level = 0;

        static SingletonClient * p_instance;
        static SingletonDestroyer destroyer;
        QTcpSocket * mTcpSocket;
    protected:
        explicit SingletonClient(QObject *parent = nullptr);
        SingletonClient(const SingletonClient& ) = delete;
        SingletonClient& operator = (SingletonClient &) = delete;
        ~SingletonClient();
        friend class SingletonDestroyer;
    public:

        static SingletonClient* getInstance();

        void send_msg_to_server(QByteArray query);
    signals:
       void message_from_server(QString msg);
       void get_client(QString message);
       void get_scr(QByteArray);
       void set_scr(QByteArray);

    private slots:
       void slotServerRead();
};

#endif // SINGLETONCLIENT_H
