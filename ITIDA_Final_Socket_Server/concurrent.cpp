#include "concurrent.h"

Concurrent::Concurrent(QObject *parent, qintptr socketDescriptor)
    : QObject{parent},  QRunnable(), socketDescriptor(socketDescriptor)
{


}

void Concurrent::run()
{

    qInfo() << this << " run " << QThread::currentThread();

    QTcpSocket* socket = new QTcpSocket(nullptr);
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        qCritical() << socket->errorString();
        delete socket;
        return;
    }
    Requesthandler = new RequestHandler(socket);
    connect(Requesthandler, &RequestHandler::disconnected, this, &Concurrent::disconnection,Qt::DirectConnection);
    qDebug() << "Thread ID:" << QThread::currentThreadId();

    while(socket->isOpen())
    {
        socket->waitForReadyRead();

    }

    qDebug()<<"socket is closed";



}

void Concurrent::disconnection()
{
    qDebug() << "Client disconnected " ;

    // QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    // qDebug() << socketDescriptor << " Disconnected";
    // socket->close();
    // socket->deleteLater();
    // exit(0);
}
