#ifndef IPLUGINS_H
#define IPLUGINS_H

#include <QThread>
#include <QJsonObject>
#include <memory>

class IPluginsReport
{
public:
    IPluginsReport() = default;

    virtual void sendScore() = 0;
    virtual void gameFinished() = 0;
};

class IPlugins : public QThread
{

public:
    enum State
    {
        Initialisation,
        Stopped = 0,
        Running,
        Success,

        WaitingPlayers,
        InRunning,
        Error
    };
    Q_ENUM(State)
    enum Difficulty
    {
        debug = 0,
        easy,
        normal,
        hard
    };

    IPlugins( IPluginsReport* report ) { _report = report; }

    int                  getScore(void);
    IPlugins::State      getState(void);
    IPlugins::Difficulty getDiffilcuty(void);

    bool setScore(int);
    bool setState(IPlugins::State);
    bool setDifficulty(IPlugins::Difficulty);
    bool stop();


protected:
    virtual void main_game_callback() = 0;
    virtual void Setup() = 0;
    void game_session_init();
    void run();

    State               _state = Stopped;
    std::atomic_bool    _running;
    IPluginsReport*     _report = nullptr;
    Difficulty          _difficulty = easy;
    int                 _ScoreBrou;

public Q_SLOTS:
  // void Slot_RCon(QJsonObject packet);

Q_SIGNALS:
 // void Signal_RCon_AudioControler(QJsonObject);
  // void Signal_RCon_NetworkIO(QJsonObject);
 // void Signal_RCon_Network(QJsonObject);
};
#endif // IPLUGINS_H
