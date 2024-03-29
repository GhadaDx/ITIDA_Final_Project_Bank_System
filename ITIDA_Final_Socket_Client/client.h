#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDebug>
#include <QAbstractSocket>
#include <QMetaEnum>
#include <QNetworkProxy>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual void sendrequesttoserver(QString request)=0;
    virtual bool SystemLogIn()=0;
    virtual bool CheckAccountNumber(QString accountnumber)=0;
    virtual void ViewAccountBalance()=0;
    virtual void ViewTransactionHistory()=0;
    virtual void GetAccountnumber()=0;

signals:
protected:
    QDataStream outStream;
    QDataStream inStream;
    QString m_role;
    QString m_request;
    QVariant m_serverrespond;
    quint32 m_accountBalance;
    QString m_accountnumber;
};

#endif // CLIENT_H
