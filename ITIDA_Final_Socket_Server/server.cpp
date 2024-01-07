#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    m_path = "E:/IMT/ITIDA_Final_Project_Bank_System/ITIDA_Final_Socket_Server/";
    m_historyfile = "History.json";
    m_usersfile = "Users.json";
    m_adminsfile = "Admins.json";
    connect(&server,&QTcpServer::newConnection,this,&Server::newConnection);
}
quint32 Server::counter = 1;

quint32 Server::generateUniqueNumber(const QString& uniqueString)
{
    return qHash(uniqueString);
}
void Server::start()
{
    if(!server.listen(QHostAddress::Any,1234))
    {
        qWarning()<<server.errorString();
        return;
    }
    qInfo()<<"Listening";
}

void Server::quit()
{
    server.close();
}

void Server::newConnection()
{
    QTcpSocket* socket=server.nextPendingConnection();
    connect(socket,&QTcpSocket::disconnected,this,&Server::disconnected);
    connect(socket,&QTcpSocket::readyRead,this,&Server::readyRead);
    qInfo()<<"connected to"<<socket;
}

void Server::disconnected()
{
    QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    qInfo()<<"Disconnected"<<socket;
    qInfo()<<"parent"<<socket->parent();
}

void Server::readyRead()
{
    //create pointer to the socket connected to the object send the request
    QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    //create an in datasream to take the needed data from the client
    QDataStream inStream(socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    //take the request message and the role of our client to be able to handle the request
    QString request,role ;
    inStream>>request>>role;
    qDebug()<<request<<role;
    //call this method to handle the request according to client role
    Handlerequest(request,role);
}

void Server::Handlerequest(QString request,QString role)
{
    //create pointer to the socket connected to the object send the request
    QTcpSocket* socket=qobject_cast<QTcpSocket*>(sender());
    //create an in datasream to take the needed data from the client
    QDataStream inStream(socket);
    //create an out datastream to send the respond to the client
    QDataStream outStream(socket);
    // QTextStream stream(socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    outStream.setVersion(QDataStream::Qt_6_6);
    //check if oour client is user or admin to handle his request
    if(role.toUpper()=="USER")
    {
        if(request=="TransferAmount")
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
                socket->waitForBytesWritten();
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
        //Done
        else if(request=="ViewAccount")
        {
            QString username;
            inStream >> username;
            QString file = m_path + m_usersfile;
            QString respond =  JsonHandler::ViewUserField(file, username);

            //sending respond to the server using the socket
            outStream<<respond;
        }
        //Done
        else if(request=="login")
        {
            bool ok = true;
            QString username,password;
            inStream >> username >> password ;
            QString file = m_path + m_usersfile;
            ok = JsonHandler::authenticateUser(file, username, password);
            qDebug() << "authenticayed" << ok;
            qDebug() << "Received from client:" << username<<" "<<password ;
            //sending respond to the server using the socket
            outStream<<ok;

        }
        //Done
        else if(request=="GetAccountNumber")
        {
            bool ok;
            QString username, respond;
            inStream >> username ;
            QString file = m_path + m_usersfile;
            // ok =printObjectFields(file, username);
            ok = JsonHandler::printObjectFields(file, username, "Account Number", respond);
            outStream << ok << respond;

        }
        //Done
        else if(request=="AccountBalance")
        {
            bool ok;
            QString username, respond;
            inStream >> username ;
            QString file = m_path + m_usersfile;
            // ok =printObjectFields(file, username);
            ok = JsonHandler::printObjectFields(file, username, "Balance", respond);
            outStream << ok << respond;

        }
        //hnkmlha
        else if(request == "MakeTransaction")
        {
            QString amount;
            QString username;
            QString accountnumber;
            inStream >> amount>> accountnumber>> username;

            QString file = m_path + m_usersfile;
            QString fileName = m_path + m_historyfile;
            bool ok = JsonHandler::updateOrCreateJsonFile(fileName, file,username,  accountnumber, amount);
            outStream<<ok;

        }
        //lesa
        else if(request == "TransactionHistory")
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
    }
    else if(role.toUpper() == "ADMIN")
    {
        if(request=="login")
        {
            bool ok = true;
            QString username,password;
            inStream >> username >> password ;
            QString file = m_path + m_adminsfile;
            ok = JsonHandler::authenticateUser(file, username, password);
            qDebug() << "authenticated" << ok;
            qDebug() << "Received from client:" << username<<" "<<password ;
            //sending respond to the server using the socket
            outStream<<ok;

        }
        else if(request=="AccountBalance")
        {
            bool ok;
            QString username, respond;
            inStream >> username ;
            QString file = m_path + m_usersfile;
            // ok =printObjectFields(file, username);
            ok = JsonHandler::printObjectFields(file, username, "Balance", respond);
            outStream << ok << respond;

        }
        else if(request=="GetAccountNumber")
        {
            bool ok;
            QString adminname, username, respond;
            inStream >> adminname >> username ;
            QString file = m_path + m_usersfile;
            ok = JsonHandler::printObjectFields(file, username, "Account Number", respond);
            outStream << ok << respond;

        }
        else if(request == "ViewBankDatabase")
        {
            QString DataBase;

            QString file = m_path + m_usersfile;
            bool ok = JsonHandler::PrintAllDb(file, DataBase);
            outStream << ok << DataBase;
        }
        else if(request == "UpdateUser")
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
                socket->waitForBytesWritten();
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
        else if(request == "DeleteUser")
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
        else if(request == "CreateNewUser")
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
                    socket->waitForBytesWritten();
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
                    socket->waitForBytesWritten();
                    inStream >> username >>secondName;
                    ok =true;
                    // outStream << ok;
                }
                else if(data == "balance")
                {
                    socket->waitForBytesWritten();
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
                    socket->waitForBytesWritten();
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
                    counter ++;
                }

            }
            outStream << ok;
        }
        else if(request == "ViewTransactionHistory")
        {
            QString username;
            QString count;
            inStream >> username >> count ;
            quint16 recieved_count = count.toInt();
            qDebug() << "username" << username << "recieved_count" << recieved_count ;
            QString fileName =m_path+ m_historyfile;

            QString respond = JsonHandler::ViewTransactionHistory(fileName, username, recieved_count);
            qDebug() << respond;
            outStream << respond;
        }
    }
}
