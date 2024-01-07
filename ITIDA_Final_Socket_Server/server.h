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
#include "jsonhandler.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void Handlerequest(QString,QString);
    quint32 generateUniqueNumber(const QString& uniqueString);


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
    QString m_path;
    QString m_historyfile;
    QString m_usersfile, m_adminsfile;


};

#endif // SERVER_H
