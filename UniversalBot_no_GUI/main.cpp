#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "programManager.h"

unsigned numberOfCurrentRequestsSucces = 0, numberOfCurrentRequestsError = 0;
int numberOfProxies = 0;

void endOfOneRequest(bool succes, QString proxy, quint16 port, int status)
{
    QString statusString;
    if(succes)
    {
        statusString = "succes";
        ++numberOfCurrentRequestsSucces;
    }
    else
    {
        statusString = "error";
        ++numberOfCurrentRequestsError;
    }
    qDebug().noquote()<<"Proxy: " + proxy + " Port: " +
    QString::number(port) + " " + statusString + "(" +
    QString::number(status) + ") " + QString::number(numberOfCurrentRequestsSucces + numberOfCurrentRequestsError)
    + "/" + QString::number(numberOfProxies);
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    programManager manager;
    QString proxiesFileName = "proxies.txt";
    if(argc == 1)
    {
        qDebug().noquote()<<"UniversalBot 1.0\nWritten by Arkadiusz97.\n"
        "Usage:\n"
        "GET requests: UniversalBot Url\n"
        "POST requests without parameters: UniversalBot Url \"\"\n"
        "POST requests with parameters: UniversalBot Url parameters";
        return 0;
    }
    QObject::connect(&manager, &programManager::endOfOneRequest, endOfOneRequest);
    qDebug().noquote()<<"STARTED";
    if(manager.loadProxies(proxiesFileName))
    {
        qDebug().noquote()<<"Couldn't find file " + proxiesFileName + ".";
        return 1;
    }
    numberOfProxies = manager.numberOfProxies();
    if(argc == 2)
    {
        manager.runRequest(argv[1], false, "");
        qDebug().noquote()<<"DONE. Succesful requests: " + QString::number(numberOfCurrentRequestsSucces)
        + " Failed requests: " + QString::number(numberOfCurrentRequestsError);;
        return 0;
    }
    if(argc == 3)
    {
        manager.runRequest(argv[1], true, argv[2]);
        qDebug().noquote()<<"DONE. Succesful requests: " + QString::number(numberOfCurrentRequestsSucces)
        + " Failed requests: " + QString::number(numberOfCurrentRequestsError);;
        return 0;
    }
    return a.exec();
}
