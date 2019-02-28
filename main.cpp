#include <application.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    //instense Qt
    Application a(argc, argv);

    return a.exec();
}
