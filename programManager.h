#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QNetworkProxy>
#include<QEventLoop>
#include<QList>
#include<QFile>
#include<QRegularExpression>
#include<QTextStream>
#include<QTimer>
#include<QObject>
class programManager : public QObject
{
    Q_OBJECT
public:
    programManager();
    ~programManager();
    bool runRequest(QString url, bool postRequest, QByteArray postData);
    bool loadProxies(QString fileName);
    int numberOfProxies();
signals:
    void endOfOneRequest(bool succes, QString proxy, quint16 port, int status);
private:
    QNetworkAccessManager *manager;
    QEventLoop loop;
    QList<QNetworkProxy> proxiesList;
    QTimer timer;
    QNetworkReply *reply;
    QNetworkRequest request;
};
