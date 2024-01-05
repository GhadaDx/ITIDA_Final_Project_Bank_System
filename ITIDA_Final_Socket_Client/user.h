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
    //bool userLogin(QString role);
    void Get_Account_number();
    void Make_Transaction ();
    void Transfer_Amount();
    void ViewAccount();
    //login class
    bool SystemLogIn();
    bool CheckAccountNumber(QString accountnumber);
    void ViewAccountBalance();
    void ViewTransactionHistory();

    //client_socket class
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
    // QString m_accountnumber;
    // quint32 m_balance;






};

#endif // USER_H
