#include <QCoreApplication>
#include "function.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    readConfigFile();
    replaceFiles();
    return a.exec();
}
