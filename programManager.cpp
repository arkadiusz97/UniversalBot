#include "programManager.h"
programManager::programManager()
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    jsonSettings.insert("timeout", QJsonValue(10000));
}
programManager::~programManager()
{
    //Nothing to do yet.
}
bool programManager::loadProxies(QString fileName)
{
    proxiesList.clear();
    QRegularExpression re1("(.*):(.*):(.*):(.*)"), re2("(.*):(.*)");
    QFile file(fileName);
    if(!file.exists())
    {
        return 1;
    }
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        auto match = re1.match(line);
        if(match.hasMatch())
        {
            proxiesList.push_back(QNetworkProxy(QNetworkProxy::HttpProxy, match.captured(1), match.captured(2).toInt(), match.captured(3), match.captured(4)));
        }
        else
        {
            auto match2 = re2.match(line);
            if(match2.hasMatch())
            {
                proxiesList.push_back(QNetworkProxy(QNetworkProxy::HttpProxy, match2.captured(1), match2.captured(2).toInt()));
            }
        }
    }
    file.close();
    return 0;
}
bool programManager::getSettingsFromFile(QString fileName)
{//At the moment loads only timeout setup.
    QFile settingsFile(fileName);
    if(!settingsFile.open(QIODevice::ReadOnly))
        return 1;
    QByteArray fileContent = settingsFile.readAll();
    settingsFile.close();
    QJsonDocument jsonSettingsFile(QJsonDocument::fromJson(fileContent));
    if(!jsonSettingsFile.isObject())
        return 1;
    QJsonObject jsonObject = jsonSettingsFile.object();
    if(!jsonObject.contains("timeout"))
    {
        return 1;
    }
    else
    {
        jsonSettings = jsonObject;
        return 0;
    }
}
void programManager::saveSettingsToFile(QString fileName)
{
    QJsonDocument settingDoc(jsonSettings);
    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    outputFile.write(settingDoc.toJson(QJsonDocument::Indented));
    outputFile.close();
}
int programManager::numberOfProxies()
{
    return proxiesList.size();
}
int programManager::getNumberOfCurrentRequestsSucces()
{
    return numberOfCurrentRequestsSucces;
}
int programManager::getNumberOfCurrentRequestsError()
{
    return numberOfCurrentRequestsError;
}
QString programManager::getDuration()
{
    qint64 millisecondsDiff = firstDateTime.msecsTo(lastDateTime);
    qint64 milliseconds = millisecondsDiff % 1000;
    qint64 seconds = (millisecondsDiff / 1000) % 60 ;
    qint64 minutes = ((millisecondsDiff / (1000*60)) % 60);
    qint64 hours = (millisecondsDiff / (1000*60*60));
    QString millisecondsString, secondsString, minutesString;
    if(milliseconds < 10)
        millisecondsString = "00" + QString::number(milliseconds);
    else if(milliseconds < 100)
        millisecondsString = "0" + QString::number(milliseconds);
    else
        millisecondsString = QString::number(milliseconds);
    if(seconds < 10)
        secondsString = "0" + QString::number(seconds);
    else
        secondsString = QString::number(seconds);
    if(minutes < 10)
        minutesString = "0" + QString::number(minutes);
    else
        minutesString = QString::number(minutes);
    QString durationString = QString::number(hours) + ":" + minutesString + ":" +
    secondsString + "." + millisecondsString;
    return durationString;
}
void programManager::setTimeout(int timeoutToSet)
{
    if(timeoutToSet < 1)
        jsonSettings.insert("timeout", QJsonValue(10000));
    else
        jsonSettings.insert("timeout", QJsonValue(timeoutToSet));

}
QJsonObject programManager::getLoadedSettings()
{
    return jsonSettings;
}
bool programManager::runRequest(QString url, bool postRequest, QByteArray postData)
{
    firstDateTime = QDateTime::currentDateTime();
    numberOfCurrentRequestsSucces = 0;
    numberOfCurrentRequestsError = 0;
    int statusCode = 0;
    if(proxiesList.isEmpty())
        return 1;
    for(auto i = proxiesList.begin(); i != proxiesList.end(); ++i)
    {
        manager = new QNetworkAccessManager;
        manager->setProxy(*i);
        if(postRequest)
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            request.setUrl(QUrl(url));
            reply = manager->post(request, postData);
        }
        else
            reply = manager->get(QNetworkRequest(QUrl(url)));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        timer.start(jsonSettings["timeout"].toInt());
        loop.exec();
        statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        succes = true;
        if(timer.isActive())
        {
            timer.stop();
            if(reply->error() > 0)
            {
                succes = false;
                ++numberOfCurrentRequestsError;
            }
            else
            {
                ++numberOfCurrentRequestsSucces;
            }
        }
        else//In case of timeout.
        {
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            succes = false;
            ++numberOfCurrentRequestsError;
            reply->abort();
        }
        emit endOfOneRequest(succes, i->hostName(), i->port(), statusCode, numberOfCurrentRequestsSucces, numberOfCurrentRequestsError, QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz"));
        delete manager;
    }
    lastDateTime = QDateTime::currentDateTime();
    return 0;
}
