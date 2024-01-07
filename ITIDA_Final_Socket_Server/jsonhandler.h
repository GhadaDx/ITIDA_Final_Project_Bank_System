#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QDebug>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

class JsonHandler
{
public:
    JsonHandler();
    static bool authenticateUser(const QString &filename, const QString& username, const QString& password);
    static bool printObjectFields(const QString &fileName, const QString &objectName, const QString &request, QString &respond);
    static QString ViewUserField(const QString &fileName, const QString &objectName);
    static bool PrintAllDb(const QString &fileName, QString &DataBase);
    static bool updateOrCreateJsonFile(const QString& filename, const QString& Usersfilename, const QString& username, const QString& accountnumber, const QString& amount);
    static bool EditSpecifcField(const QString &fileName, const QString &ObjectKey, const QString &FieldToChange, const QString &NewValue);
    static bool Transfer_Amount(const QString &fileName,const QString &fromtransferaccount,const QString & totransferaccount, const QString &transferamount);
    static bool checkOnObject(const QString& filename, const QString& keyValue);
    static bool deleteObject(const QString &filePath, const QString &objectKey);
    static bool SetHistory(QString fileName, QString username,const QString &amount);
    static QString ViewTransactionHistory(const QString &fileName, const QString &username,quint16 count);
    static bool AddNewObject(const QString &fileName, QVariantMap newObject,const QString& NewObjectName);




};

#endif // JSONHANDLER_H
