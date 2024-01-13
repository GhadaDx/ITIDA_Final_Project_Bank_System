#ifndef USER_H
#define USER_H

#include <QTextStream>
#include <QDataStream>
#include <iostream>
#include "client.h"

class User: public Client
{
public:
    User();
    quint16 showOptions();
    // void Get_Account_number();
    void Make_Transaction ();
    void Transfer_Amount();
    void ViewAccount();

    //client class
    bool SystemLogIn();
    bool CheckAccountNumber(QString accountnumber);
    void GetAccountnumber();
    void ViewAccountBalance();
    void ViewTransactionHistory();
    void sendrequesttoserver(QString request);
signals:
public slots:
    void connectToHost(QHostAddress host,quint16 port);
    void disconnect();
protected slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketerror);
    void stateChanged(QAbstractSocket::SocketState socketstate);
    void readyRead();
private:
    QTcpSocket m_client;
    QTextStream qin;
    QTextStream cout;
    QString m_username;







};

#endif // USER_H
