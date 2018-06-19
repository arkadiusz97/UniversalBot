#include "programManager.h"
programManager::programManager()
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
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
int programManager::numberOfProxies()
{
    return proxiesList.size();
}
bool programManager::runRequest(QString url, bool postRequest, QByteArray postData)
{
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
        timer.start(10000);
        loop.exec();
        statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(timer.isActive())
        {
            timer.stop();
            if(reply->error() > 0)
                emit endOfOneRequest(false, i->hostName(), i->port(), statusCode);
            else
            {
                emit endOfOneRequest(true, i->hostName(), i->port(), statusCode);
            }
        }
        else//In case of timeout.
        {
            disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            emit endOfOneRequest(false, i->hostName(), i->port(), statusCode);;
            reply->abort();
        }
        delete manager;
    }
    return 0;
}
