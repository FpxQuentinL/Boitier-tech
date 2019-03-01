#include <QJsonObject>
#include <QString>
#include <QObject>
#include <QDebug>

#include "clientIO.h"
#include <sio_client.h>
#include <functional>
#include <cstring>
#include <cstdlib>

using namespace sio;
using namespace std;

clientIO::clientIO(QObject *parent) :
    QObject(parent)
    //_io2(new client())
{

}

/*
void Server::acceptNextConnection()
{
    QTcpSocket* socket = nextPendingConnection();
    connect( socket, SIGNAL(readyRead()), this, SLOT(readSocket()) );
    _sockets.push_back( socket );
    qDebug() << "New Client connected : ";
}

void ClientIO::readSocket()
{
    QTcpSocket* socket = dynamic_cast< QTcpSocket* >( sender() );
    if( !socket )
        return;
    qDebug() << "socket received : " << socket->readAll();
    test_signal();
}
*/

void clientIO::Slot_RCon(QJsonObject packet)
{
    qDebug("TU IO SOCKET !!! :D");
    if (packet.contains("io_socket") && packet.contains("command"))
    {
            writeSocket(packet);
    }
}


void clientIO::writeSocket(QJsonObject packet)
{
    QString m_data;
    QString m_socket;
    m_data = packet.value("command").toString();
    m_socket = packet.value("io_socket").toString();
    QByteArray bytes2 = m_socket.toUtf8();
    QByteArray bytes = m_data.toUtf8();
    std::string data(bytes2.data(),bytes2.length());
    std::string socket(bytes.data(),bytes.length());
    _io2.socket()->emit(socket.c_str() , data);
}


void clientIO::Initialize(QJsonObject config)
{
//    qDebug() << "Conection to :"  << config.value("adress").toString() << ":" << config.value("adress").toString();
//    //_io2.connect("http://localhost:3000");
//    _io2.connect("http://localhost:3000");

}
