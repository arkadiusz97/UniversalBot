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
#include<QDateTime>
#include<QObject>
#include<QJsonDocument>
#include<QJsonObject>
#include<QDataStream>

class programManager : public QObject
{
    Q_OBJECT
public:
    programManager();
    ~programManager();
    bool runRequest(QString url, bool postRequest, QByteArray postData);
    bool loadProxies(QString fileName);
    bool getSettingsFromFile(QString fileName);
    int numberOfProxies();
    int getNumberOfCurrentRequestsSucces();
    int getNumberOfCurrentRequestsError();
    QString getDuration();
    QJsonObject getLoadedSettings();
public slots:
    void setTimeout(int timeoutToSet);
    void saveSettingsToFile(QString fileName);
signals:
    void endOfOneRequest(bool succes, QString proxy, quint16 port, int status, int numberOfCurrentRequestsSucces, int numberOfCurrentRequestsError, QString dateTime);
private:
    QNetworkAccessManager *manager;
    QEventLoop loop;
    QList<QNetworkProxy> proxiesList;
    QTimer timer;
    QNetworkReply *reply;
    QNetworkRequest request;
    int numberOfCurrentRequestsSucces, numberOfCurrentRequestsError;
    bool succes;
    QDateTime firstDateTime, lastDateTime;
    QJsonObject jsonSettings;
};
