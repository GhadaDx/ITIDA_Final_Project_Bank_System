#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include "jsonhandler.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void Handlerequest(QString,QString);

    // QString printAccountnumber(const QString &fileName, const QString &objectName);


signals:
public slots:
    void start();
    void quit();
    void newConnection();
    void disconnected();
    void readyRead();
private:
    QTcpServer server;
    static quint32 counter;


};

#endif // SERVER_H
