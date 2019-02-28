#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QByteArray>
#include <QSerialPort>
#include <QTextStream>
#include <QJsonObject>
#include <QTimer>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    SerialPort(QSerialPort *serialPort, QObject *parent = nullptr);

Q_SIGNALS:
    void    Signal_RCon_Game(QJsonObject);

public Q_SLOTS:
    void    Slot_RCon(QJsonObject packet);

private Q_SLOTS:
    void handleReadyWrite(char *data);
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QTextStream m_standardOutput;
    QTimer m_timer;
};
#endif // SERIALPORT_H
