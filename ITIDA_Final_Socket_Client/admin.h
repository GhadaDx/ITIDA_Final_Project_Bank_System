#ifndef ADMIN_H
#define ADMIN_H

#define MAX_USR_AGE 90
#define MIN_USR_AGE 18
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include <iostream>
#include "client.h"

class Admin : public Client
{
public:
    Admin();
    quint16 showOptions();
    // void GetAccountnumber ();
    void ViewBankDatabase();
    void CreateNewUser();
    void DeleteUser();
    void UpdateUser();
    bool SetField(const QString& field, QString& value, bool& ok);

    //client class
    bool SystemLogIn();
    bool CheckAccountNumber(QString accountnumber);
    void GetAccountnumber ();
    void ViewAccountBalance();
    void ViewTransactionHistory();
    void sendrequesttoserver(QString request);

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
    QTextStream qout;
    QString m_adminname;
    QString m_DataBase;
    QString m_username, m_Password, m_Passwordagain, m_SecondName, m_Balance, m_Age;


};



#endif // ADMIN_H
