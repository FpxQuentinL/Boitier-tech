#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>

#include <QtMultimedia/QtMultimedia>

class AudioController : public QObject
{
    Q_OBJECT


public:
    AudioController(QObject *parent = 0);

public Q_SLOTS:
    void Slot_RCon(QJsonObject data);
    void onStateChanged(QMediaPlayer::MediaStatus state);


Q_SIGNALS:
    void Signal_RCon_Game(QJsonObject data);
    void Signal_RCon_Network(QJsonObject data);

protected:
    QList< QMediaPlayer* > _listplayer;
    void playSound(QJsonObject packet);
    void stopSound(QJsonObject packet);
    void pauseSound(QJsonObject packet);
    void stateSound(QJsonObject packet);
    //QMap< QString, std::function< void(*)(QJsonObject) > > _mapCommands;
};

#endif // AUDIOCONTROLLER_H

