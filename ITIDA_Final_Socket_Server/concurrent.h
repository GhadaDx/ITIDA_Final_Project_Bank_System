#ifndef CONCURRENT_H
#define CONCURRENT_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include "requesthandler.h"

class Concurrent : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Concurrent(QObject *parent = nullptr, qintptr socketDescriptor = 0);

signals:

    // QRunnable interface
public:
    void run() override;

public slots:
    void disconnection();
private:
    qintptr socketDescriptor;
    RequestHandler* Requesthandler;

};

#endif // CONCURRENT_H
