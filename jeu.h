#ifndef JEU_H
#define JEU_H

#include "iplugins.h"
#include <QJsonObject>
#include <QString>
#include <QtSerialPort/QSerialPort>

class Jeu : public IPlugins
{
    Q_OBJECT

public:
    Jeu(IPluginsReport* report);

    // QThread interface
    //bool checkVictoryCondition();
    bool Game_Success();
    bool Game_Fail();
    bool Game_Socketio();
    void Setup();
    static void InteruptGeneral();
    QString _AudioPathInterupt;
    QString _AudioPathAmbiance;
    QSerialPort _serialPort;
    void loadConfigGameTech(QString config_file);
    void loadConfigGameFonct(QString config_file);
public Q_SLOTS:
    void Slot_RCon(QJsonObject packet);

protected:
    virtual void main_game_callback() override;
    int _score;

Q_SIGNALS:
    void Signal_RCon_AudioControler(QJsonObject);
    void Signal_RCon_NetworkIO(QJsonObject);
    void Signal_RCon_Network(QJsonObject);
    void Signal_RCon_Serial(QJsonObject);
};

#endif // JEU_H
