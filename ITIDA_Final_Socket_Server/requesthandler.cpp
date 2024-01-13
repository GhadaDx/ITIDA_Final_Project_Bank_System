#include "requesthandler.h"

RequestHandler::RequestHandler(QTcpSocket *socket, QObject *parent)
    : QObject{parent}
{
    outStream.setDevice(socket);
    outStream.setVersion(QDataStream::Qt_6_6);
    inStream.setDevice(socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    m_path = "E:/IMT/ITIDA_Final_Project_Bank_System/ITIDA_Final_Socket_Server/";
    m_historyfile = "History.json";
    m_usersfile = "Users.json";
    m_adminsfile = "Admins.json";
    connect(socket, &QTcpSocket::readyRead, this, &RequestHandler::readyRead,Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &RequestHandler::Disconnected,Qt::DirectConnection);

}
quint32 RequestHandler::generateUniqueNumber(const QString& uniqueString)
{
    return qHash(uniqueString);
}
void RequestHandler::Handlerequest(QString request, QString role)
{

    if(role.toUpper()=="USER")
    {
        if(request=="TransferAmount")
        {
            TransferAmount();
        }
        else if(request=="ViewAccount")
        {
            ViewAccount();
        }
        else if(request=="login")
        {
            QString file = m_path + m_usersfile;
            Login(file);
        }
        else if(request=="GetAccountNumber")
        {
            GetAccountNumber();
        }
        else if(request=="AccountBalance")
        {
            AccountBalance();
        }
        else if(request == "MakeTransaction")
        {
            MakeTransaction();

        }
        else if(request == "TransactionHistory")
        {
            TransactionHistory();

        }
    }
    else if(role.toUpper() == "ADMIN")
    {
        if(request=="login")
        {
            QString file = m_path + m_adminsfile;

            Login(file);

        }
        else if(request=="AccountBalance")
        {
            AccountBalance();

        }
        else if(request=="GetAccountNumber")
        {
            GetAccountNumber();

        }
        else if(request == "ViewBankDatabase")
        {
            ViewBankDatabase();
        }
        else if(request == "UpdateUser")
        {
            UpdateUser();
        }
        else if(request == "DeleteUser")
        {
            DeleteUser();

        }
        else if(request == "CreateNewUser")
        {
            CreateNewUser();
        }
        else if(request == "ViewTransactionHistory")
        {
            TransactionHistory();
        }
    }
}



void RequestHandler::Disconnected()
{
    qDebug() << "Disconnected ";
    emit disconnected();

}

void RequestHandler::readyRead()
{
    // //create pointer to the socket connected to the object send the request
    // QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    // //create an in datasream to take the needed data from the client
    // QDataStream inStream(socket);
    // inStream.setVersion(QDataStream::Qt_6_6);
    //take the request message and the role of our client to be able to handle the request

    QString request,role ;

    inStream>>request>>role;
    if (inStream.status() != QDataStream::Ok)
    {
        // Handle error
        qCritical() << "Error reading from the data stream";
        return;
    }
    qDebug()<<request<<role;
    //call this method to handle the request according to client role
    Handlerequest(request,role);
}

void RequestHandler::TransferAmount()
{
    QString file = m_path + m_usersfile;
    bool ok;
    QString fromUsername, toUsername,  transferamount;
    inStream >> ok;
    if(ok ==true)
    {
        inStream >> fromUsername >>toUsername ;
        ok = JsonHandler::checkOnObject(file, toUsername);
        outStream<<ok;
        qDebug() << "Received from client:" << fromUsername << " " << toUsername;
    }
    else if(ok == false)
    {
        // socket->waitForBytesWritten();
        inStream >> fromUsername >> toUsername >> transferamount;
        //send data to the method transfer acount to check and process on them using json methods
        qint32 Transferamount = transferamount.toInt(&ok);
        if(ok == true)
        {
            if(Transferamount >=0)
            {
                ok = JsonHandler::Transfer_Amount(file, fromUsername, toUsername, transferamount);
            }
            else
            {
                ok = false;
                qInfo() << "Transfer amount is too small enter positive number";
            }
        }
        else
        {
            ok = false;
            qInfo() << "Invalid Transfer amount";
        }
    }
    else
    {
        ok = false;
    }
    //sending respond to the server using the socket
    outStream<<ok;
}

void RequestHandler::ViewAccount()
{
    QString username;
    inStream >> username;
    QString file = m_path + m_usersfile;
    QString respond =  JsonHandler::ViewUserField(file, username);

    //sending respond to the server using the socket
    outStream<<respond;
}

void RequestHandler::Login(const QString& fileName)
{
    bool ok = true;
    QString username,password;
    inStream >> username >> password ;
    ok = JsonHandler::authenticateUser(fileName, username, password);
    qDebug() << "authenticayed" << ok;
    qDebug() << "Received from client:" << username<<" "<<password ;
    //sending respond to the server using the socket
    outStream<<ok;

}

void RequestHandler::GetAccountNumber()
{
    bool ok;
    QString username, respond;
    inStream >> username ;
    QString file = m_path + m_usersfile;
    ok = JsonHandler::printObjectFields(file, username, "Account Number", respond);
    outStream << ok << respond;

}

void RequestHandler::AccountBalance()
{
    bool ok;
    QString username, respond;
    inStream >> username ;
    QString file = m_path + m_usersfile;
    ok = JsonHandler::printObjectFields(file, username, "Balance", respond);
    outStream << ok << respond;

}

void RequestHandler::MakeTransaction()
{
    QString amount;
    QString username;
    QString accountnumber;
    inStream >> amount>> accountnumber>> username;

    QString file = m_path + m_usersfile;
    QString fileName = m_path + m_historyfile;
    bool ok = JsonHandler::updateOrCreateJsonFile(fileName, file, username, amount);
    outStream<<ok;

}

void RequestHandler::TransactionHistory()
{
    QString username;
    QString count;
    inStream >> username >> count ;
    quint16 recieved_count = count.toInt();
    qDebug() << "username" << username << "recieved_count" << recieved_count ;

    QString fileName = m_path + m_historyfile;

    QString respond = JsonHandler::ViewTransactionHistory(fileName, username, recieved_count);
    qDebug() << respond;
    outStream << respond;

}

void RequestHandler::ViewBankDatabase()
{
    QString DataBase;

    QString file = m_path + m_usersfile;
    bool ok = JsonHandler::PrintAllDb(file, DataBase);
    outStream << ok << DataBase;
}

void RequestHandler::UpdateUser()
{
    QString file = m_path + m_usersfile;
    QString username;
    QString FieldToChange, newValue, repassword;
    bool ok;
    inStream >> ok;
    qDebug() << "ok" << ok ;
    if(ok == true)
    {
        inStream >> username;
        qDebug() << username << " " <<username;
        ok = JsonHandler::checkOnObject(file, username);
        outStream << ok;
    }
    if(ok == false)
    {
        // socket->waitForBytesWritten();
        inStream >> FieldToChange >> username >>newValue;
        qDebug() << "username"  <<username;
        qDebug() << "FieldToChange"  <<FieldToChange;


        if(FieldToChange == "password")
        {
            inStream  >> repassword;
            qDebug() << newValue << " " <<repassword;
            if(newValue == repassword)
            {
                ok = JsonHandler::EditSpecifcField(file, username, FieldToChange, newValue);
            }
            else
            {
                ok = false;
            }

        }
        else if(FieldToChange == "Age")
        {
            qint32 age = newValue.toInt(&ok);
            if(ok == true)
            {
                if(age >= 18 && age <= 90)
                {
                    qDebug() << newValue ;
                    ok = JsonHandler::EditSpecifcField(file, username, FieldToChange, newValue);
                }
                else
                {
                    ok = false;

                }
            }
            else
            {
                ok = false;
            }

        }
        else if(FieldToChange == "Balance")
        {
            qint32 balance = newValue.toInt(&ok);
            if(ok == true)
            {
                if( balance > 0 )
                {
                    qDebug() << newValue ;
                    ok = JsonHandler::EditSpecifcField(file, username, FieldToChange, newValue);
                }
                else
                {
                    ok = false;
                }
            }
            else
            {
                ok = false;
            }
        }
        else if(FieldToChange == "fullname")
        {
            qDebug() << newValue ;
            QString Newfullname = username + " " + newValue;
            qDebug() << Newfullname ;

            ok = JsonHandler::EditSpecifcField(file, username, FieldToChange, Newfullname);
        }
    }
    outStream << ok;
}

void RequestHandler::DeleteUser()
{
    bool ok;
    QString file = m_path + m_usersfile;
    QString FileHistory = m_path + m_historyfile;

    QString username;
    inStream >> username;
    ok = JsonHandler::deleteObject(file, username);
    if(ok == true)
    {
        bool deleteHistory = JsonHandler::deleteObject(FileHistory, username);
        qDebug() << "history deleted" << deleteHistory;

    }
    inStream << ok;
}

void RequestHandler::CreateNewUser()
{
    QString file = m_path + m_usersfile;
    QString username, data, password ,repassword, secondName, balance, age;
    bool ok;
    inStream >> ok;
    if(ok == true)
    {
        inStream >> data>> username;
        ok = JsonHandler::checkOnObject(file, username);
        // outStream << ok;
    }
    else
    {
        inStream >> data;
        if(data == "password")
        {
            // socket->waitForBytesWritten();
            inStream >> username >> repassword >> password ;
            qInfo() << "password is " << password << "password again is " << repassword;
            if(password == repassword)
            {
                ok =true;

            }
            else
            {
                ok =false;
            }
            // outStream << ok;
        }
        else if(data == "secondName")
        {
            // socket->waitForBytesWritten();
            inStream >> username >>secondName;
            ok =true;
            // outStream << ok;
        }
        else if(data == "balance")
        {
            // socket->waitForBytesWritten();
            inStream  >> username >> balance;
            qint32 Balance = balance.toInt(&ok);
            if(ok == true)
            {
                if(Balance >= 0)
                {
                    ok =true;
                }
                else
                {
                    ok =false;
                }
            }
            else
            {
                ok =false;
            }
        }
        else if(data == "age")
        {
            // socket->waitForBytesWritten();
            inStream >>username>>  age;
            qint32 Age = age.toInt(&ok);
            qDebug() << "Age = " << Age;
            if(ok == true)
            {
                qDebug() << "Age is " << ok;
                if(Age >= 18 && Age <= 90 )
                {
                    ok = true;
                    qDebug() << "Age is " << Age;

                }
                else
                {
                    ok = false;
                }
            }
        }
        else if(data == "all")
        {
            outStream >> username>> password >> secondName >> balance >>age;
            QVariantMap newObject;
            QString NewObjectName = username;
            quint32 accountnumber = generateUniqueNumber(username);

            QString Accountnumber = QString::number(accountnumber);
            newObject["Account Number"]= Accountnumber;
            newObject["Age"] = age;
            newObject["Balance"] = balance,
                newObject["fullname"] = username + " "+ secondName;
            newObject["password"]= password;
            ok = JsonHandler::AddNewObject(file, newObject, NewObjectName);
        }

    }
    outStream << ok;
}
