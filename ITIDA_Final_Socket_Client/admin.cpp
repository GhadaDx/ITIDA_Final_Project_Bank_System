#include "admin.h"
Admin::Admin(): qin(stdin), qout(stdout) {
    outStream.setDevice(&m_client);
    inStream.setDevice(&m_client);
    inStream.setVersion(QDataStream::Qt_6_6);
    outStream.setVersion(QDataStream::Qt_6_6);
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
    qInfo() << "===================================================";
    qInfo() << "Please Select the method to perform: ";
    qInfo() << "1- Get Account Number";
    qInfo() << "2- View Account Balance";
    qInfo() << "3- View Transaction history";
    qInfo() << "4- View Data Base";
    qInfo() << "5- Create New User";
    qInfo() << "6- Delete User";
    qInfo() << "7- Update User";
    qInfo() << "8- Exit Program";
    qInfo() << "===================================================";
    qout << "Admin: " << Qt::endl;
    qin >> adminchoise;
    qInfo() << "===================================================";
    return adminchoise;
}

bool Admin::SystemLogIn()
{
    m_request = "login";
    outStream<<m_request<<m_role;
    std::string adminName, password;
    qout <<"login>> <username> <password>\n";
    qout << "Admin: " << Qt::endl;
    std::cin >> adminName ;
    qout << "-----------------------------------------"<< Qt::endl;
    qout << "Password: " << Qt::endl;
    std::cin >> password;
    qout << "-----------------------------------------" <<Qt::endl;
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
    std::string username;
    qout << "view Account Balance <User Name>" << Qt::endl;
    qInfo() << "User Name: " << Qt::endl;
    std::cin >> username;
    qout << "-----------------------------------------" << Qt::endl;
    QString Username=QString::fromStdString(username);
    outStream << Username;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    qInfo()<<"View Balance is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        qInfo()<<"Your Account Money is :"<<m_accountBalance;
    }
    else
    {
        qInfo()<<"Invalid User Name";

    }
}

void Admin::ViewTransactionHistory()
{
    std::string count, username;
    qout << "View Transaction History <username> <count>" << Qt::endl;
    qout << "username:" << Qt::endl;
    std::cin >> username;
    qout << "-----------------------------------------" << Qt::endl;
    qout << "count>> " << Qt::endl;
    std::cin >> count;
    qout << "-----------------------------------------" << Qt::endl;

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
    }
}

void Admin::GetAccountnumber()
{
    std::string m_usrname;
    qout << "Get Account Number: <username>";
    qout << "username: "<< Qt::endl;
    std::cin  >> m_usrname;
    qout << "-----------------------------------------" << Qt::endl;
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
    qInfo()<<"View Data Base is:"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        // Print the entire JSON content
        qDebug().noquote() << "Contents of Bank Data Base is:";
        qDebug().noquote() << m_DataBase;
        qout << "-----------------------------------------" << Qt::endl;
    }
}

bool Admin::SetField(const QString& field, QString& value, bool& ok) {
    // Send a request to the server to set the specified field
    std::string newvalue;
    outStream << m_request << m_role << ok << field << m_username;
    qout << field << " >>  " << Qt::endl;
    std::cin >> newvalue;
    qout << "-----------------------------------------" << Qt::endl;
    if(field == "password")
    {
        std::string passwordagain;
        qout << "\n-----------------------------------------" << Qt::endl;
        qout <<"Password again>> " << Qt::endl;
        std::cin >> passwordagain;
        qout << "-----------------------------------------" << Qt::endl;
        QString Passwordagain=QString::fromStdString(passwordagain);
        outStream << Passwordagain;
    }
    QString Newvalue=QString::fromStdString(newvalue);
    value = Newvalue;
    outStream << Newvalue;
    m_client.waitForBytesWritten();
    m_client.waitForReadyRead();
    qInfo() << "Your setting " << field << " is :" << m_serverrespond.toBool();

    // Return false if the server responds with an error
    if (!m_serverrespond.toBool()) {
        qDebug() << "Error setting " << field;
        return false;
    }

    return true;
}
void Admin::CreateNewUser()
{
    QString FieldToChange;
    bool ok= true;
    std::string firstName;
    // std::string secondName, password, passwordagain, balance, age;
    // QString Password, Passwordagain, SecondName, Balance, Age;
    qout << "User Name:" << Qt::endl;
    std::cin >> firstName ;
    qout << "-----------------------------------------" << Qt::endl;

    QString Username=QString::fromStdString(firstName);
    FieldToChange = "username";

    outStream << ok << FieldToChange<<Username;
    m_username = Username;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    qInfo()<<"Your transaction history is :"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true) return;

    //-----------------password------------------------
    ok = false;
    if (!SetField("password", m_Password, ok)) return;
    if (!SetField("secondName", m_SecondName, ok)) return;
    if (!SetField("balance", m_Balance, ok)) return;
    if (!SetField("age", m_Age, ok)) return;
    FieldToChange = "all";
    outStream << m_request << m_role<< ok << FieldToChange << m_username << m_Password << m_SecondName << m_Balance << m_Age;
    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    qInfo()<<"create user is :"<<m_serverrespond.toBool();
    if(m_serverrespond.toBool() == true)
    {
        qDebug() << "user created successfully";
    }
    else
    {

        qDebug() << "user not created successfully";
        CreateNewUser();
    }

}

void Admin::DeleteUser()
{
    std::string username;
    qout << "Delete user>> <username>\n"<< Qt::endl;
    qout << "User Name: "<<Qt::endl;
    std::cin >> username;
    qout << "-----------------------------------------" << Qt::endl;

    QString Username=QString::fromStdString(username);
    outStream << Username;

    m_client.waitForBytesWritten();
    //wait for the respond from the server to view it to the client
    m_client.waitForReadyRead();
    //get the responde from the server
    qInfo()<<"Delete User is:"<<m_serverrespond.toBool();
}

void Admin::UpdateUser()
{
    QString UserInput;
    // std::string Newpassword, NewpasswordAgain, Newage, Newbalance, NewsecondName;
    // QString NewPassword, NewPasswordAgain, FieldToChange, NewAge, NewBalance, NewSecondName;
    bool ok = true, check;
    std::string Username ;
    qout << "Update User>> <username>" << Qt::endl;
    qout << "User Name:" << Qt::endl;
    std::cin >> Username;
    qout << "-----------------------------------------\n" <<Qt::endl;
    QString UserName=QString::fromStdString(Username);
    m_username = UserName;
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
            qDebug() << "1.Password \n2.Age \n3.Balance \n4.Full Name";
            qin >> UserInput;
            if(UserInput.toLower() == "stop")
            {
                break;
            }
            int choice = UserInput.toInt();
            switch(choice)
            {
            case 1:
                check = SetField("password", m_Password, ok);
                if(check == true)
                {
                    qDebug() << "Password updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Password choose different username.";
                }
                break;
            case 2:
                check = SetField("Age", m_Age, ok);
                if(check == true)
                {
                    qDebug() << "Age updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Age enetr postive value between" << MAX_USR_AGE << "and" << MIN_USR_AGE ;

                }
                break;
            case 3:
                check = SetField("Balance", m_Balance, ok);
                if(check == true)
                {
                    qDebug() << "Balance updated successfuly.";
                }
                else
                {
                    qDebug() << "Invalid Balance enter Positive value.";
                }
                break;
            case 4:
                check = SetField("fullname", m_SecondName, ok);
                if(check == true)
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
            qDebug() << "Do you want to continue? (type `stop` to exit/ `y` to continue):";
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
