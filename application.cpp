#include "application.h"

#include "audiocontroller.h"
#include "clientIO.h"
#include "server.h"
#include "jeu.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QDir>
#include <iostream>
#include <unistd.h>
#include <QSerialPort>
#include <QStringList>
#include <QTextStream>
#include "serialport.h"


Application::Application(int &argc, char **argv, int flags)
:QCoreApplication(argc,argv,flags)
{
    initialize(argc, argv, flags);
}

void Application::startProcess(QString /*path*/)
{

}


void Application::sendScore()
{
    QJsonObject packet;
    packet.insert("hostname","server");
    packet.insert("push_score",_listPlugins.first()->getScore());
    _server->writeSocket(packet);
}

void Application::gameFinished()
{
    _listPlugins.first()->stop();
}

bool Application::loadConfig(QString config_file)
{
    QJsonDocument doc;
    QDir            dir;
    qDebug() << dir.path();
    QFile file(config_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return(false);
    qDebug() << "Opening config file" << config_file;
    doc = QJsonDocument::fromJson( file.readAll() );
    QJsonObject object = doc.object();
    qDebug("\n\n###\t start json file\t###");
    for( int i=0; i<object.keys().size(); i++ )
    {
        qDebug() << object.keys()[i] << "\t" << object.value(object.keys()[i]).toString();
    }
    qDebug("###\t end json file###\n\n");

    //search key "port"
    int port = object.value("port").toString().toInt();
    if( 1024 < port && port < 65535 )
    {
        _tcpPort = port;
        std::cout << QString("Set listening port from config file : [%1] \n").arg(port).toStdString();

    }
    else
    {
        std::cout << QString("No default port define in config.json : setting default value [%1] \n").arg(_tcpPort).toStdString();
    }
    return (true);
}

bool Application::checkConfig()
{
    qDebug() << "Checking Configureation";
    if( 1024 < _tcpPort && _tcpPort < 65535 )
        if(42)
            return (true);
    qDebug() << "Error in settings !!!";
    return (false);
}

void Application::test_slot()
{
    qDebug("YOUOUOUUOOUOUOU");
    toto_signal();
}
void Application::Initialize_Serial()
{
    QSerialPort serialPortR;
    serialPortR.setPortName("/dev/ttyUSB0");
    serialPortR.setBaudRate(115200);

    if (!serialPortR.open(QIODevice::ReadWrite)) {
        qDebug() << QObject::tr("Failed to open port %1, error: %2")
                          .arg("/dev/ttyUSB0")
                          .arg(serialPortR.errorString())
                       << endl;

    }
    SerialPort serialPort(&serialPortR);
    qDebug() << "PLOP";
}

void Application::Initialize_Game()
{
    if (_listPlugins.isEmpty())
    {
        qDebug() << "JINSTANCIIII";
        _listPlugins.append( new Jeu(this) );


        //Connect for Audio comunication/callback
        connect(_listPlugins.first(),   SIGNAL(Signal_RCon_AudioControler(QJsonObject)),
                _audioController,       SLOT(Slot_RCon(QJsonObject)));
        connect(_audioController,       SIGNAL(Signal_RCon_Game(QJsonObject)),
                _listPlugins.first(),   SLOT(Slot_RCon(QJsonObject)));

        //connect for Network communication/callback
        connect(_listPlugins.first(),   SIGNAL(Signal_RCon_Network(QJsonObject)),
                _server,                SLOT(Slot_RCon(QJsonObject)));
        connect(_server,                SIGNAL(Signal_RCon_Game(QJsonObject)),
                _listPlugins.first(),   SLOT(Slot_RCon(QJsonObject)));

//        //connect for SocketIO communication/callback
        connect(_listPlugins.first(),   SIGNAL(Signal_RCon_NetworkIO(QJsonObject)),
                _clientio,              SLOT(Slot_RCon(QJsonObject)));
        connect(_clientio,              SIGNAL(Signal_RCon_Game(QJsonObject)),
                _listPlugins.first(),   SLOT(Slot_RCon(QJsonObject)));

        connect(_listPlugins.first(),   SIGNAL(Signal_RCon_Serial(QJsonObject)),
                _serial,                SLOT(Slot_RCon(QJsonObject)));
        connect(_serial,                SIGNAL(Signal_RCon_Game(QJsonObject)),
                _listPlugins.first(),   SLOT(Slot_RCon(QJsonObject)));

        //Connect for reinstance when thread goes DIE !!!
        connect(_listPlugins.first(),   SIGNAL(finished()),
                this,                   SLOT(Initialize_Game()));
    }
    _listPlugins.first()->start();
}


void Application::initialize(int &argc, char **argv, int flags)
{
    flags = flags; //use for remove warning
    QString config_file_url = "./test.json";

    qDebug() << "MasterApp start";
    if (argc > 1)
        config_file_url = argv[1];
    qDebug() << "Load config file : " << config_file_url;
    if (!loadConfig(config_file_url))
        return;
    if (!checkConfig())
        return;

    if( !_audioController )
    {
        _audioController = new AudioController(this);
    }
        QJsonObject clientio_setup_packet;

    _clientio = new clientIO(this);
        clientio_setup_packet.insert("adress", "127.0.0.1:4242");
       // clientio_setup_packet.insert("port", "4242";
    _clientio->Initialize(  clientio_setup_packet );


        //good instance
        QSerialPort *serialPortR;

        serialPortR = new QSerialPort();
        serialPortR->setPortName("/dev/ttyACM0");
        serialPortR->setBaudRate(115200);

        if (!serialPortR->open(QIODevice::ReadWrite)) {
            qDebug() << QObject::tr("Failed to open port %1, error: %2")
                              .arg("/dev/ttyUSB0")
                              .arg(serialPortR->errorString())
                           << endl;
        }
        else
        {
            qDebug()<<"OK";

        }
        //SerialPort serialPort(&serialPortR);
        _serial = new SerialPort(serialPortR,this);

    _server = new Server(this);
    _server->initialize( _tcpPort );
    Initialize_Game();
}
