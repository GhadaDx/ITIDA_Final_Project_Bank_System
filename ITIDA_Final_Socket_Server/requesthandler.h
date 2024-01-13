#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QTcpSocket>
#include "jsonhandler.h"


class RequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QTcpSocket* socket, QObject *parent = nullptr);
    void Handlerequest(QString request, QString role);
    quint32 generateUniqueNumber(const QString& uniqueString);

    //handle
    void TransferAmount();
    void ViewAccount();
    void Login(const QString& fileName);
    void GetAccountNumber();
    void AccountBalance();
    void MakeTransaction();
    void TransactionHistory();
    void ViewBankDatabase();
    void UpdateUser();
    void DeleteUser();
    void CreateNewUser();

signals:
    void disconnected();
public slots:
    void Disconnected();
    void readyRead();

private:
    QDataStream outStream;
    QDataStream inStream;
    QString m_path;
    QString m_historyfile;
    QString m_usersfile, m_adminsfile;
};

#endif // REQUESTHANDLER_H
