#include <QCoreApplication>
#include "function.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(readConfigFile())
    {
       replaceFiles();
    }
    return a.exec();
}
