#include "audiocontroller.h"
#include <unistd.h>
#include <time.h>

#include <QDebug>
#include <QString>
#include <QJsonObject>

#include "jeu.h"

AudioController::AudioController(QObject *parent)
    : QObject(parent)
{
//    _listplayer.append( new QMediaPlayer() );
 //   _mapCommands.insert( "play", std::bind( &AudioController::playSound(QJsonObject *), this ) );
}

void AudioController::playSound(QJsonObject data)
{
    for (int i = 0; i < _listplayer.length(); i++) {
        if (_listplayer[i]->state() == QMediaPlayer::StoppedState) {
            _listplayer[i]->setMedia(QMediaContent(QUrl(data.value("path").toString())));
            _listplayer[i]->setVolume(10);
            if(data.contains("volume"))
            {
                _listplayer[i]->setVolume(data.value("volume").toInt());
            }

            _listplayer[i]->play();
            qDebug() << QString("Reuse instance sound \t Playing sound %1").arg( data.value("path").toString() );
            if (data.contains("recast") && data.value("recast").toBool() ) {
               qDebug("Enebale Permanently Recast");
               connect(_listplayer[i], SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), _listplayer[i] , SLOT(play()));            }
            return;
        }
    }
    qDebug() << "creation d'une nouvelle instance de QmediaPlayer";
    _listplayer.append( new QMediaPlayer() );
    connect( _listplayer.last(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onStateChanged(QMediaPlayer::MediaStatus) ));
    _listplayer.last()->setMedia(QMediaContent(QUrl(data.value("path").toString())));
    _listplayer.last()->setVolume(10);
    if(data.contains("volume"))
    {
        _listplayer.last()->setVolume(data.value("volume").toInt());
    }
    _listplayer.last()->play();
    qDebug() << QString("Create new instancee \t Playing sound %1").arg( data.value("path").toString() );
    if (data.contains("recast") && data.value("recast").toBool() ) {
        qDebug("Enebale Permanently Recast");
        connect(_listplayer.last(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), _listplayer.last() , SLOT(play()));
    }
}

void AudioController::pauseSound(QJsonObject data)
{
    if (data.value("id") == "all")
        for (int i; i < _listplayer.length(); i++) {
            _listplayer[i]->pause();
        }
    return;
}

void AudioController::stopSound(QJsonObject data)
{
    for (int i = 0; i < _listplayer.length(); i++) {
       if (data.contains("id") &&  ( QMediaContent(data.value("id").toString()) == _listplayer[i]->currentMedia() || data.value("id") == "all"))
{
           if (data.contains("force") && data.value("force") == true) {
                disconnect(_listplayer[i], SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), _listplayer[i], 0);
            }
            }
           _listplayer[i]->stop();
        }
    return;
}

void AudioController::stateSound(QJsonObject data)
{
    QJsonObject state_report;

    //state_report.initialize();
    for (int i = 0; i < _listplayer.length(); i++) {
        if (!data.contains("id"))
            state_report.insert(_listplayer[i]->currentMedia().canonicalUrl().toString(), _listplayer[i]->state());
    }
    Signal_RCon_Game(state_report);
}

void AudioController::onStateChanged(QMediaPlayer::MediaStatus state)
{
    QJsonObject data;

    switch( state )
    {
    case QMediaPlayer::LoadedMedia:
        ;
        break;
    case QMediaPlayer::StoppedState:
        ;
        break;
    case QMediaPlayer::PlayingState:
        ;
        break;
    case QMediaPlayer::PausedState:
        ;
        break;
    case QMediaPlayer::StalledMedia:
        ;
        break;
    case QMediaPlayer::BufferingMedia:
        ;
        break;
    case QMediaPlayer::BufferedMedia:
        ;
        break;
    case QMediaPlayer::EndOfMedia:
        ;
        break;
    case QMediaPlayer::InvalidMedia:
        ;
        break;
    }
    qDebug() << QString( "Audio state changed to : %1" ).arg(state);
}


void AudioController::Slot_RCon(QJsonObject data) //a definir si recup ou scpecifie l'instance.
{
    qDebug()<<">Slot_RCon_AudioController<";

    if (data.contains("command")) {
        if (data.value("command") == "play")
            playSound(data);
        if (data.value("command") == "stop")
            stopSound(data);
        if (data.value("command") == "pause")
            pauseSound(data);
        if (data.value("command") == "state")
            stateSound(data);
    }
}
