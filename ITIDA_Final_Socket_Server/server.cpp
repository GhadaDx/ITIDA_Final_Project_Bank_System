#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    pool =new QThreadPool(this);
    server = new QTcpServer(this);
    pool->setMaxThreadCount(4);
    qInfo() << "Threads: " << pool->maxThreadCount();
    connect(server,&QTcpServer::newConnection,this,&Server::onNewConnection);

}
Server::~Server()
{
    quit();
    serverThread.quit();
    serverThread.wait();
}
void Server::start(quint16 port)
{
    qInfo() << this << " start " << QThread::currentThread();
    if(this->listen(QHostAddress::Any, port))
    {
        qInfo() << "Server started on " << port;
    }
    else
    {
        qCritical() << this->errorString();
    }
}


void Server::quit()
{
    this->close();
    qInfo() << "Server Stopped!";
}

void Server::incomingConnection(qintptr handle)
{
    Concurrent *client = new Concurrent(nullptr, handle);
    qInfo() << "Incomming Connection " << handle << " on " << QThread::currentThread();
    // Threads* client = new Threads(nullptr, handle);
    client->setAutoDelete(true);
    pool->start(client);
    qInfo()<<"connected to"<<socketDescriptor();

}

void Server::onNewConnection()
{
    QTcpSocket* clientSocket=server->nextPendingConnection();
    QThread* clientThread = new QThread;
    RequestHandler* Requesthandler = new RequestHandler(clientSocket, nullptr);
    connect(clientThread, &QThread::finished, Requesthandler, &RequestHandler::deleteLater);
    connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);
    Requesthandler->moveToThread(clientThread);
    clientThread->start();
    qInfo()<<"connected to"<<clientSocket;
}

void Server::disconnected()
{
    QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    qInfo()<<"Disconnected"<<socket;
    qInfo()<<"parent"<<socket->parent();
}
