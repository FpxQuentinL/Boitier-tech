#include "jeu.h"
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonObject>
#include <audiocontroller.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <wiringPi.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>



int G_OutputDiodeV;
int G_OutputDiodeR;
bool G_DeclenchementInterupt;
int G_Interupt;
QTime G_TimerBypass;
std::atomic_int G_CombiValid[21];
std::atomic_int G_CombiReset[21];
std::atomic_int G_CombiCurrent[21];

Jeu::Jeu(IPluginsReport *report) : IPlugins(report)
{

}



bool Jeu::Game_Fail()
{
    QJsonObject play_sound
    {
        {"command", "stop"},
        {"id", "file:///home/pi/Downloads/test.mp3"}
    };

    std::cout << "YOU LOOSE !!!" << std::endl;
    Signal_RCon_AudioControler(play_sound);
    return (true);
}

void Jeu::loadConfigGameTech(QString config_file)
{
    QJsonDocument docGameTech;
    QDir dirGameTech;
    qDebug() << dirGameTech.path();
    QFile fileGameTech(config_file);
    if (!fileGameTech.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;
    docGameTech = QJsonDocument::fromJson(fileGameTech.readAll());
    QJsonObject object = docGameTech.object();
    qDebug("\n\n###\t start json file\t###");

    G_OutputDiodeV = object.value("ControlerLedV").toString().toInt();
    G_OutputDiodeR = object.value("ControlerLedR").toString().toInt();

    G_Interupt = object.value("Interupt").toString().toInt();
    qDebug("###\t end json file###\n\n");

}
void Jeu::loadConfigGameFonct(QString config_file)
{
    QJsonDocument docGame;
    QDir dirGame;
    qDebug() << dirGame.path();
    QFile fileGame(config_file);
    if (!fileGame.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;

    docGame = QJsonDocument::fromJson(fileGame.readAll());
    qDebug()<<docGame;
    QJsonObject object = docGame.object();
    qDebug("\n\n###\t start json file\t###");


    QJsonArray array = object["List_ID"].toArray();
    qDebug("\n\n###\t start json file\t###");
    int i=0;
    for(i=0;i<array.size();i++)
    {
       G_CombiValid[i]=array.at(i).toString().toInt();
    }
    QJsonArray arrayReset=object["List_Reset"].toArray();
    for(i=0;i<arrayReset.size();i++)
    {
       G_CombiReset[i]=arrayReset.at(i).toString().toInt();
    }
    _AudioPathInterupt = object.value("AudioPathI").toString();
    _AudioPathAmbiance = object.value("AudioPathA").toString();



    qDebug("###\t end json file###\n\n");

}
void Jeu::InteruptGeneral()
{
    QTime test=QTime::currentTime().addMSecs(250);

    while (QTime::currentTime()<test){

    }
    if (!digitalRead(G_Interupt)){
        qDebug()<<"je suis de l'electromagnetisme";
        return;
    }

    G_DeclenchementInterupt=true;
    G_TimerBypass=QTime::currentTime().addSecs(15);

}


void Jeu::Setup()
{
    wiringPiSetupGpio();
    G_DeclenchementInterupt=false;
    pinMode(G_OutputDiodeV,OUTPUT);
    pinMode(G_OutputDiodeR,OUTPUT);
    loadConfigGameFonct("./GameFonct.json");
    loadConfigGameTech("./GameTech.json");
    if(wiringPiISR(G_Interupt,INT_EDGE_RISING, &Jeu::InteruptGeneral)<0)
        qDebug("ERRRROOOOORRRRRR");
    G_TimerBypass=QTime(25,0,0,0);




}

bool Jeu::Game_Socketio()
{
    QJsonObject packet
    {
        {"io_socket", "iterate_and_pulse"},
        {"command", "{\"color\":\"0xFAFAFA\",\"delay\":\"1\",\"brightness_max\":\"255\",\"brightness_min\":\"32\",\"brightness_scale\":\"5\"pos_start\":\"0\",\"pos_end\":\"144\",\"scale\":\"i\"}"}
    };

    std::cout << "YOU Socket iO !!!" << std::endl;
    Signal_RCon_NetworkIO(packet);
    return (true);
}

void Jeu::Slot_RCon( QJsonObject packet )
{
   qDebug()<<">IPlugins::Slot_RCon<"<< "\n" << packet;

        if (packet.contains("command"))
        {
            if (packet.value("command")=="Combi")
            {
               QJsonArray array =packet["List"].toArray();
               int i = 0;
               for (i=0;i<array.size();i++)
               {
                G_CombiCurrent[i]=array.at(i).toString().toInt();
               }
            }

    }
}


//GameFlow
void Jeu::main_game_callback()
{
    bool valid = true;
    bool validreset = true;
    int i = 0;
    for (i=0;i<21;i++)
    {
        if(G_CombiCurrent[i]!=G_CombiValid[i])
        {
            valid = false;
        }
        if(G_CombiCurrent[i]!=G_CombiReset[1])
        {
            validreset = false;
        }
     }
        if (valid)
        {
            if (_First)
            {
            _First=false;
            QJsonObject AudioInterupt;
            AudioInterupt.insert("command","play");
            AudioInterupt.insert("path",_AudioPathInterupt);
            Signal_RCon_AudioControler(AudioInterupt);
            }
            digitalWrite(G_OutputDiodeV, HIGH);
            digitalWrite(G_OutputDiodeR, LOW);
        }
        else if (validreset)
        {
           digitalWrite(G_OutputDiodeR,HIGH);
           digitalWrite(G_OutputDiodeV, LOW);
        }
        else
        {
            _First=true;
            digitalWrite(G_OutputDiodeR, HIGH);
            digitalWrite(G_OutputDiodeV, LOW);
        }
        if(G_DeclenchementInterupt){
            G_DeclenchementInterupt=false;
            QJsonObject AudioFinale;
            AudioFinale.insert("command","play");
            AudioFinale.insert("path",_AudioPathAmbiance);
            if (G_TimerBypass.isValid())
            {
                if(G_TimerBypass<QTime::currentTime())
                {
                        if (!digitalRead(G_Interupt))
                        {
                            G_TimerBypass=QTime(25,0,0,0);
                            valid =true;

                        }
                        else
                        {
                            valid=true;
                        }
                }
            }
            if (valid)
            {
                Signal_RCon_AudioControler(AudioFinale);
                digitalWrite(G_OutputDiodeV, HIGH);
                digitalWrite(G_OutputDiodeR, LOW);
            }
            else if (validreset&&digitalRead(G_OutputDiodeV))
            {
               digitalWrite(G_OutputDiodeR,HIGH);
               digitalWrite(G_OutputDiodeV, LOW);
               G_TimerBypass=QTime(25,0,0,0);
            }

    }



}
