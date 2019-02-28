#include "server.h"
#include "jeu.h"
#include <thread>
#include <QTcpSocket>


Server::Server( QObject* parent )
    :QTcpServer(parent)
{

}

void Server::acceptNextConnection()
{
    QTcpSocket* socket = nextPendingConnection();
    connect( socket, SIGNAL(readyRead()), this, SLOT(readSocket()) );
    _sockets.push_back( socket );
    qDebug() << "New Client connected : ";
}

void Server::readSocket()
{
    QTcpSocket* socket = dynamic_cast< QTcpSocket* >( sender() );
    if( !socket )
        return;
    qDebug() << "socket received : " << socket->readAll();
    //test_signal();
}

void Server::writeSocket(QJsonObject packet)
{
  //  qDebug() << QString("not coded send data with socket but arg funct is :\n\t [%1] \n").arg( packet);
//    _sockets.last()->write(data.toStdString());
    QString tmp;
    for (int i = 0; i < packet.length(); i++)
    {
        tmp += QString('{') + QString('"');
        tmp += packet.keys()[i];
        tmp += QString('"');
        tmp += QString(":");
        tmp += QString('"');
        tmp += packet.value(packet.keys()[i]).toString();
        tmp += QString('"') + QString('}');
        if (i < packet.length() - 1)
            tmp += ',' + '\n';
    }
    _sockets.last()->write(tmp.toStdString().c_str());
    _sockets.last()->write("sending data");
    qDebug() << tmp;
}

void Server::Slot_RCon(QJsonObject packet)
{

    qDebug("TU GERE :D");
 //   qDebug() << packet;
//    if (packet.contains("hostname")) {
//        if (packet.value("hostname") == "props_stage_2")
//            writeSocket(packet);
  //  }
}

void Server::initialize( int tcpPort )
{
     qDebug() << "mouai" << tcpPort << "]";
    if( !listen(QHostAddress::Any, tcpPort ) )
    {
        qDebug() << "Error : Unable to start listen";
    }
    qDebug() << "Start listening on port [" << tcpPort << "]";
    //connect( this, SIGNAL(newConnection() ), this, SLOT( acceptNextConnection() ) );
    qDebug() << "Ready for new connection" ;
}
