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
    //void updateJsonHistoryFile(const QList<QPair<QString, int>>& newData, const QString& filename);
    static bool updateOrCreateJsonFile(const QString& filename, const QString& Usersfilename, const QString& username, const QString& accountnumber, const QString& amount);
    //bool searchKeyInJsonDocument(const QString& fileName, const QString& targetKey);
    static bool EditSpecifcField(const QString &fileName, const QString &ObjectKey, const QString &FieldToChange, const QString &NewValue);
    // bool GetField(const QString &filename, const QString &Username, const QString &field, QString &fieldValue);
    static bool Transfer_Amount(const QString &fileName,const QString &fromtransferaccount,const QString & totransferaccount, const QString &transferamount);
    static bool checkOnObject(const QString& filename, const QString& keyValue);
    static bool EditKeyValue(const QString &fileName, const QString &ObjectKey, const QString &NewValue);
    static bool deleteObject(const QString &filePath, const QString &objectKey);
    static bool createEmptyJsonObject(const QString& filename, const QString& key);
    static bool SaveTransaction(QString fileName, QString username,const QString &amount);
    static QString ViewTransactionHistory(const QString &fileName, const QString &username,quint16 count);
    static bool AddNewObject(const QString &fileName, QVariantMap newObject,const QString& NewObjectName);




};

#endif // JSONHANDLER_H
