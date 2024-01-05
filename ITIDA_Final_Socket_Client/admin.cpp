#include "admin.h"
#define MAX_USR_AGE 90
#define MIN_USR_AGE 18
Admin::Admin(): qin(stdin), qout(stdout) {
    //client = new Client_socket();
    m_role="admin";
    connect(&m_client,&QTcpSocket::connected,this,&Admin::connected);
    connect(&m_client,&QTcpSocket::disconnected,this,&Admin::disconnected);
    connect(&m_client,&QTcpSocket::stateChanged,this,&Admin::stateChanged);
    connect(&m_client,&QTcpSocket::readyRead,this,&Admin::readyRead);
    connect(&m_client,&QTcpSocket::errorOccurred,this,&Admin::error);

}

quint16 Admin::showOptions()
{
    quint16 adminchoise;
    qInfo() << "Please Select the method to perform: ";
    qInfo() << "1- Get Account Number";
    qInfo() << "2- View Account Balance";
    qInfo() << "3- View Transaction history";
    qInfo() << "4- View Data Base";
    qInfo() << "5- Create New User";
    qInfo() << "6- Delete User";
    qInfo() << "7- Update User";
    qInfo() << "8- Exit Program";
    qout << "Admin: ";
    qout.flush();
    qin >> adminchoise;
    return adminchoise;

}

bool Admin::SystemLogIn()
{
    m_request = "login";
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    outStream<<m_request<<m_role;
    std::string adminName, password;
    qout << "login>> <username> <password>\n";
    qout << "login>> " << Qt::endl;
    std::cin >> adminName >> password;
    qInfo() << "Hello: " << adminName;
    quint8 counter = 0;
    bool ok = false;
    QString AdminName=QString::fromStdString(adminName);
    QString Password=QString::fromStdString(password);
    m_adminname = AdminName;


    while (counter < 3)
    {
        counter++;

        if (!AdminName.isEmpty() && !Password.isEmpty())
        {
            ok = true;
            break;
        }


    }
    if (ok)
    {
        //send the data to the server to handle that
        outStream<<AdminName<<Password;
        m_client.waitForReadyRead();
        qInfo()<<"login is :"<<m_serverrespond.toBool();
        ok = m_serverrespond.toBool();
    }
    else
    {
        qInfo() << "User not found, please try again later..";
    }

    return ok;
}



bool Admin::CheckAccountNumber(QString accountnumber)
{
    if(accountnumber <= '0')
    {
        qInfo() << "Invalid Account Number!";
        return false;
    }
    else
    {
        return true;
    }
}

void Admin::ViewAccountBalance()
{
    //quint32 responde;
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    std::string username;
    qInfo() << "view Account Balance <accountnumber>";
    qInfo() << "Account number>> ";
    std::cin >> username;
    QString Username=QString::fromStdString(username);
    outStream << Username;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    qInfo()<<"View Balance is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        qInfo()<<"Your account money is :"<<m_accountBalance;
    }
    else
    {
        qInfo()<<"Invalid user name";

    }



}

void Admin::ViewTransactionHistory()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    // QString accountnumber;
    std::string count, username;
    qInfo() << "View Transaction History <username> <count> ";
    qInfo() << "username>> ";
    std::cin >> username;
    qInfo() << "count>> ";
    std::cin >> count;
    QString Username=QString::fromStdString(username);
    QString Count=QString::fromStdString(count);


    if(Count.toInt() <= 0)
    {
        qInfo() << "Please enter a valid count.";
        ViewTransactionHistory();
    }
    else
    {
        outStream << Username << Count;
        m_client.waitForBytesWritten();
        //wait for the respond from the server to view it to the client
        m_client.waitForReadyRead();
        qDebug().noquote() <<"Your transaction history is :"<<m_serverrespond.toString();

        // QByteArray History = "ViewTransactionHistory " + accountnumber + " " + count;
        // client->send_Request(History);
        //qDebug() << "Account Balance is: " << response;
    }


}


void Admin::GetAccountnumber()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    std::string m_usrname;
    qout << "Get Account Number: <username>";
    qout << "username: "<< Qt::endl;
    std::cin  >> m_usrname;
    QString Username=QString::fromStdString(m_usrname);
    //send request to server to get data
    outStream << m_adminname << Username;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    //get the responde from the server
    qInfo()<<"View Account Number is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        qInfo()<<"Account Number of User "<<Username << " is: " << m_accountnumber;

    }
    else
    {
        qInfo() << "Invalid Username";
    }
}

void Admin::ViewBankDatabase()
{
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    //get the responde from the server
    qInfo()<<"View Account Number is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        // Print the entire JSON content
        qDebug().noquote() << "Contents of JSON file:";
        qDebug().noquote() << m_DataBase;
        // qInfo()<<m_DataBase;
    }
}

void Admin::CreateNewUser()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    QString FieldToChange;
    bool ok= true;
    std::string firstName, secondName, password, passwordagain, balance, age;
    QString Password, Passwordagain, SecondName, Balance, Age;

    qout << "user or admin: " << Qt::endl;
    qout << "username>>  " << Qt::endl;
    std::cin >> firstName ;
    QString Username=QString::fromStdString(firstName);
    FieldToChange = "username";

    outStream << ok << FieldToChange<<Username;
    m_username = Username;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();

    if(m_serverrespond.toBool() == false)
    {
        FieldToChange ="password";
        ok = false;
        outStream << m_request << m_role<< ok << FieldToChange;
        qout << "password>>  " << Qt::endl;
        std::cin >> password ;
        qout << "passwordagain>>  " << Qt::endl;
        std::cin >> passwordagain ;
        Password=QString::fromStdString(password);
        Passwordagain=QString::fromStdString(passwordagain);
        m_Password=Password;
        outStream << Password << Passwordagain;
        m_client.waitForBytesWritten();
        //wait for the respond from the server to view it to the client
        m_client.waitForReadyRead();
        qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();
        if(m_serverrespond.toBool() == true)
        {
            FieldToChange= "secondName";
            outStream << m_request << m_role<< ok << FieldToChange;
            qout << "secondName>>  " << Qt::endl;
            std::cin >> secondName ;
            SecondName=QString::fromStdString(secondName);
            m_SecondName =SecondName;
            outStream << SecondName;
            m_client.waitForBytesWritten();
            //wait for the respond from the server to view it to the client
            m_client.waitForReadyRead();
            qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();
            if(m_serverrespond.toBool() == true)
            {
                FieldToChange = "balance";
                outStream << m_request << m_role<< ok << FieldToChange;
                qout << "balance>>  " << Qt::endl;
                std::cin >> balance ;
                Balance=QString::fromStdString(balance);
                m_Balance=Balance;
                outStream << Balance;
                m_client.waitForBytesWritten();
                //wait for the respond from the server to view it to the client
                m_client.waitForReadyRead();
                qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();
                if(m_serverrespond.toBool() == true)
                {
                    FieldToChange = "age";
                    outStream << m_request << m_role<< ok << FieldToChange << m_username << m_Password << m_SecondName << m_Balance;
                    qout << "age>>  " << Qt::endl;
                    std::cin >> age;
                    Age=QString::fromStdString(age);
                    m_Age = Age;
                    outStream << m_Age;
                    m_client.waitForBytesWritten();
                    //wait for the respond from the server to view it to the client
                    m_client.waitForReadyRead();
                    qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();
                    if(m_serverrespond.toBool() == true)
                    {
                        qDebug() << "user created successfully";
                    }
                    else
                    {
                        qDebug() << "User age must be from " << MIN_USR_AGE << "to " << MAX_USR_AGE;
                        qDebug() << "user not created successfully";

                    }
                }
                else
                {
                    qDebug() << "invalid balance";
                }
            }
            else
            {
                qDebug() << "invalid secondname";

            }
        }
        else
        {
            qDebug() << "password must be identical";

        }

    }
    else
    {
        qDebug() << "username existed choose another name";
        CreateNewUser();
    }
    // if(password == passwordagain)
    // {
    //     qout << "Your age>> ";
    //     qout.flush();
    //     qin >> age;
    //     if(age >= MIN_USR_AGE && age <= MAX_USR_AGE )
    //     {
    //         qout << "your balance>> ";
    //         qout.flush();
    //         qin >> balance;
    //         QTextStream stream, stream2;
    //         stream << balance;
    //         stream2 << age;
    //         QString balanceString = stream.readAll();
    //         QString ageString = stream2.readAll();
    //         // QString fullname = firstName + " " + secondName;
    //         // QByteArray UserData = "CreateNewUser " + firstName + " " + fullname + " " + password + " " + balanceString + " " + ageString;
    //         //client->send_Request(UserData);
    //     }
    //     else
    //     {
    //         qDebug() << "User age must be from " << MIN_USR_AGE << "to " << MAX_USR_AGE;
    //     }
    // }
    // else
    // {
    //     qDebug() << "password must be identical";
    // }

}

void Admin::DeleteUser()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    std::string username;
    qout << "Delete user>> <username>\n"<< Qt::endl;
    qout << "Delete user>> "<<Qt::endl;
    std::cin >> username;
    QString Username=QString::fromStdString(username);
    outStream << Username;

    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    //get the responde from the server
    qInfo()<<"View Account Number is:"<<m_serverrespond.toBool();

}

void Admin::UpdateUser()
{
    QString UserInput;
    std::string Newpassword, NewpasswordAgain, Newage, Newbalance, NewsecondName;
    QString NewPassword, NewPasswordAgain, FieldToChange, NewAge, NewBalance, NewSecondName;
    bool ok = true;
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    std::string Username ;
    qout << "\n\n-----------------------------------------" <<Qt::endl;
    qout << "Update User>> <username>" << Qt::endl;
    qout << "Update user>> " << Qt::endl;
    std::cin >> Username;
    qout << "-----------------------------------------\n" <<Qt::endl;
    QString UserName=QString::fromStdString(Username);
    outStream << ok << UserName;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    //get the responde from the server
    qInfo()<<"View Account Number is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        ok = false;
        while(true)
        {
            qDebug() << "Choise what you want to Update For `exit` type Stop\n";
            qDebug() << "1.Password \n2.Age \n3.Balance \n6.Full Name";
            qin >> UserInput;
            if(UserInput.toLower() == "stop")
            {
                break;
            }
            int choice = UserInput.toInt();
            switch(choice)
            {
            case 1:
                outStream << m_request << m_role<< ok;
                FieldToChange = "password";
                qDebug() << "Please Enter the New User password";
                std::cin >>Newpassword;
                qDebug() << "Please Enter the New User password again";
                std::cin >>NewpasswordAgain;
                NewPassword=QString::fromStdString(Newpassword);
                NewPasswordAgain=QString::fromStdString(NewpasswordAgain);
                outStream << UserName<< FieldToChange<< NewPassword << NewPasswordAgain;
                qDebug() << UserName<< FieldToChange << NewPassword << NewPasswordAgain;

                m_client.waitForBytesWritten();
                //wait for the respond from the server to view it to the client
                m_client.waitForReadyRead();
                //get the responde from the server
                qInfo()<<"Change User Password is:"<<m_serverrespond.toBool();
                if(m_serverrespond.toBool() == true)
                {
                    qDebug() << "Password updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Password choose different username.";
                }
                break;
            case 2:
                outStream << m_request << m_role<< ok;
                FieldToChange = "Age";
                qDebug() << "Please Enter the New User age";
                std::cin >>Newage;
                NewAge=QString::fromStdString(Newage);
                outStream << UserName<< FieldToChange<< NewAge ;
                qDebug() << UserName<< FieldToChange << NewAge ;

                m_client.waitForBytesWritten();
                //wait for the respond from the server to view it to the client
                m_client.waitForReadyRead();
                //get the responde from the server
                qInfo()<<"Change User Age is:"<<m_serverrespond.toBool();
                if(m_serverrespond.toBool() == true)
                {
                    qDebug() << "Age updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Age enetr postive value between" << MAX_USR_AGE << "and" << MIN_USR_AGE ;

                }
                break;
            case 3:
                outStream << m_request << m_role<< ok;
                FieldToChange = "Balance";
                qDebug() << "Please Enter the New User balace";
                std::cin >>Newbalance;
                NewBalance=QString::fromStdString(Newbalance);
                outStream << UserName<< FieldToChange<< NewBalance ;
                qDebug() << UserName<< FieldToChange << NewBalance ;

                m_client.waitForBytesWritten();
                //wait for the respond from the server to view it to the client
                m_client.waitForReadyRead();
                //get the responde from the server
                qInfo()<<"Change User Balance is:"<<m_serverrespond.toBool();
                if(m_serverrespond.toBool() == true)
                {
                    qDebug() << "Balance updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Balance enter Positive value.";
                }
                break;
            case 4:
                outStream << m_request << m_role<< ok;
                FieldToChange = "fullname";
                qDebug() << "Please Enter the New User second name";
                std::cin >>NewsecondName;
                NewSecondName=QString::fromStdString(NewsecondName);
                outStream << UserName<< FieldToChange<< NewSecondName ;
                qDebug() << UserName<< FieldToChange << NewSecondName ;
                m_client.waitForBytesWritten();
                //wait for the respond from the server to view it to the client
                m_client.waitForReadyRead();
                //get the responde from the server
                qInfo()<<"Change User Age is:"<<m_serverrespond.toBool();
                if(m_serverrespond.toBool() == true)
                {
                    qDebug() << "Full name updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid entry";
                }
                break;
            default:
                qDebug()<< "Invalid choise";
                break;

            }
            qDebug() << "Do you want to continue? type `stop` to exit:";
            qin >> UserInput;
            if(UserInput.toLower() == "stop")
            {
                break;
            }
        }
    }
    else
    {
        qDebug() << "Please Enter a valid acount number.";
    }

}
void Admin::sendrequesttoserver(QString request)
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    outStream<<request<<m_role;
    m_request=request;

    if(request == "login")
    {
        bool ok = SystemLogIn();
        qDebug() << "login successfully" << ok;
    }
    else if(request == "GetAccountNumber")
    {
        GetAccountnumber();
    }
    else if(request == "AccountBalance")
    {
        ViewAccountBalance();
    }
    else if( request == "ViewTransactionHistory")
    {
        ViewTransactionHistory();

    }
    else if(request == "ViewBankDatabase")
    {
        ViewBankDatabase();
    }
    else if(request=="UpdateUser")
    {
        //call the method Transfer Account to handle this request
        UpdateUser();
    }
    else if(request=="CreateNewUser")
    {
        //call the method view Account to handle this request
        CreateNewUser();
    }
    else if(request == "DeleteUser")
    {
        DeleteUser();
    }
    else
    {
        qDebug() << "No Info recieved.";
    }

}

void Admin::connectToHost(QHostAddress host, quint16 port)
{
    if(m_client.isOpen()) disconnect();
    //qInfo()<<"connecting to host"<<host<<"on port:"<<port;
    m_client.connectToHost(host,port);
}

void Admin::disconnect()
{
    m_client.close();
    m_client.waitForDisconnected();
}

void Admin::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    m_client.write(userInput.toUtf8());
}

void Admin::disconnected()
{
    qInfo()<<"Disconnected";
}

void Admin::error(QAbstractSocket::SocketError socketerror)
{
    qInfo()<<"Error:"<<socketerror<<m_client.errorString();
}

void Admin::stateChanged(QAbstractSocket::SocketState socketstate)
{
    QMetaEnum metaenum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str= metaenum.valueToKey(socketstate);
}

void Admin::readyRead()
{
    QDataStream inStream(&m_client);
    inStream.setVersion(QDataStream::Qt_6_6);
    if(m_request=="login")
    {
        bool respond;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
    }
    else if ( m_request =="ViewAccount")
    {
        QString Account;
        //take the money in the account from the server and save it in the server respond
        inStream>>Account;
        m_serverrespond.setValue(Account);
    }
    else if(m_request=="DeleteUser")
    {
        bool respond;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
    }

    else if(m_request == "ViewTransactionHistory")
    {
        QString respond;
        // QString DataBase;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
    }
    else if(m_request == "AccountBalance")
    {
        bool respond;
        QString balance ;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond >> balance;
        m_serverrespond.setValue(respond);
        m_accountBalance = balance.toInt();
    }
    else if(m_request == "GetAccountNumber")
    {
        bool respond;
        QString account_number;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond >> account_number;
        m_serverrespond.setValue(respond);
        m_accountnumber = account_number;
    }
    else if(m_request == "ViewBankDatabase")
    {
        bool respond;
        QString DataBase;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond >> DataBase;
        m_serverrespond.setValue(respond);
        m_DataBase = DataBase;
    }
    else if(m_request == "UpdateUser")
    {
        bool respond;
        // QString DataBase;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
        // m_DataBase = DataBase;
    }
    else if(m_request == "CreateNewUser")
    {
        bool respond;
        // QString DataBase;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
        // m_DataBase = DataBase;
    }


}
