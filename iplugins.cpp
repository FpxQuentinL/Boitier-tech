#include <QCoreApplication>
#include <iplugins.h>
#include <QDebug>

bool        IPlugins::setScore(int value)
{ _ScoreBrou = value;
  return (true); }
bool        IPlugins::setState(IPlugins::State value)
{ _state = value;
  return (true); }
bool        IPlugins::setDifficulty(IPlugins::Difficulty value)
{ _difficulty = value;
  return (true); }


int                     IPlugins::getScore()
{ return _ScoreBrou; }
IPlugins::State         IPlugins::getState()
{ return _state; }
IPlugins::Difficulty    IPlugins::getDiffilcuty()
{ return _difficulty; }

bool IPlugins::stop()
{
    qDebug() << ">IPlugins::stop<";
    _running = false;
    return (true);
}

void IPlugins::game_session_init()
{
   setScore(0);
   setState(IPlugins::State::WaitingPlayers);
   setDifficulty(IPlugins::Difficulty::normal);
}
//void IPlugins::Slot_RCon1( QJsonObject packet)
//{
//    qDebug()<<">IPlugins::Slot_RCon<"<< "\n" << packet;
//}

void IPlugins::run()
{
    _running = true;
    Setup();
    while( _running )
    {
        main_game_callback();
    }
}
