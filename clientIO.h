#ifndef CLIENTIO_H
#define CLIENTIO_H

#include <QObject>
#include <sio_client.h>
#include <sio_message.h>
#include <sio_socket.h>

#include "application.h"
#include "jeu.h"

using namespace sio;

class clientIO : public QObject
{
    Q_OBJECT


public:
    clientIO(QObject *parent = 0);
    client _io2;
    void    Initialize(QJsonObject config);

Q_SIGNALS:
    void    Signal_RCon_Game(QJsonObject packet);

public Q_SLOTS:
    void    Slot_RCon(QJsonObject packet);

protected:
    void    writeSocket(QJsonObject packet);

private:
    //std::unique_ptr<client> _io;
};

#endif // CLIENTIO_H
