#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTcpSocket>
#include "jeu.h"

class Server : public QTcpServer

{
    Q_OBJECT

public:
    Server( QObject* parent = nullptr );
    void initialize( int tcpPort );

public Q_SLOTS:
    void writeSocket(QJsonObject packet);

protected Q_SLOTS:
    void acceptNextConnection();
    void readSocket();


protected:
    QList< QTcpSocket* > _sockets;

//signals
Q_SIGNALS:
    void    test_signal();
    void    Signal_RCon_Game(QJsonObject);
    void    Signal_RCon_AudioControler(QJsonObject);

public Q_SLOTS:
    void    Slot_RCon(QJsonObject packet);


};

#endif // SERVER_H
