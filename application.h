#ifndef APPLICATION_H
#define APPLICATION_H

#include <QJsonDocument>

#include <QCoreApplication>

#include <QProcess>
#include <QList>

#include "iplugins.h"
#include "clientIO.h"
#include "serialport.h"

class AudioController;
class SerialPort;
class clientIO;
class Server;
class Application :
        public QCoreApplication,
        public IPluginsReport
{
    Q_OBJECT    //Macro for QT_slot and meta-object

public:
    Application(int &argc, char **argv, int flags = ApplicationFlags);

    void startProcess( QString path );

    virtual void sendScore() override;
    virtual void gameFinished() override;
    virtual bool loadConfig( QString config_file );
    virtual bool checkConfig();

protected:
    AudioController* _audioController = nullptr;
    int _tcpPort = 33333; //default build in port
    Server*             _server;
    clientIO*           _clientio;
    SerialPort*         _serial;
    QList< QProcess* >  _listProcess;
    QList< IPlugins* >  _listPlugins;

public Q_SLOTS:
    void initialize(int &argc, char **argv, int flags);
    void test_slot();

    void Initialize_Game();
    void Initialize_Serial();
Q_SIGNALS:
    void toto_signal();

};

#endif // APPLICATION_H
