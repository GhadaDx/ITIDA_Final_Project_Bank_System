#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include <QThread>
#include "concurrent.h"
#include "requesthandler.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

signals:

public slots:
    void start(quint16 port);
    void quit();
    void onNewConnection();
    void disconnected();

private:
    QThreadPool* pool;
    QTcpServer* server;
    QThread serverThread;
    RequestHandler* Requesthandler;



    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
};

#endif // SERVER_H
