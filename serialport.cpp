#include "serialport.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

SerialPort::SerialPort(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    m_serialPort(serialPort),
    m_standardOutput(stdout)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::handleReadyRead);
    //connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
    connect(&m_timer, &QTimer::timeout, this, &SerialPort::handleTimeout);
    m_timer.start(2000);
    qDebug() << "DEBUG CONECT OK";
}

void SerialPort::handleReadyRead()
{
    QJsonArray array;
    QJsonObject object;
    qDebug() << ">>>SerialPort::handleReadyRead<<<" << this;
  if (m_serialPort->bytesAvailable() > 21)
  {
    m_readData.append(m_serialPort->readAll());

    //affiche les data (pas de traitement...)
    qDebug() << m_readData;
    //delete les data au fur et a mesure quon les consome

    if (!m_timer.isActive())
        m_timer.start(100);
    int list[21];
    int i =0;
    while (m_readData[i]!=13)
    {
    // 48 etant pour convertir de l'asci en decimal '0' = 48 ascii
    list[i]=m_readData[i]-48;
    array.append(list[i]);
    i+=1;
    }
    }
   object.insert("command", "Combi");

    object.insert("List",array);

    Signal_RCon_Game(object);
    m_readData.clear();

  }

void SerialPort::handleReadyWrite(char *data)
{
    qDebug() << ">>>SerialPort::handleReadyWrite<<<" << this;

    m_serialPort->write(data);
}


void SerialPort::handleTimeout()
{

    qDebug() << "serial timeout" << endl;
    //call get_all au timeout pour affiche en boucle les valeurs
    handleReadyWrite("get_all\n");
}

void SerialPort::Slot_RCon(QJsonObject packet)
{
    qDebug() << "SerialPort::Slot_RCon";
    handleReadyWrite("get_all\n");
}

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;

    }
}


