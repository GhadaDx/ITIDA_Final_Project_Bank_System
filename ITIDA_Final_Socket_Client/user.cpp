#include "user.h"

User::User(): qin(stdin), cout(stdout) {
    //client = new Client_socket();
    m_role="user";
    connect(&m_client,&QTcpSocket::connected,this,&User::connected);
    connect(&m_client,&QTcpSocket::disconnected,this,&User::disconnected);
    connect(&m_client,&QTcpSocket::stateChanged,this,&User::stateChanged);
    connect(&m_client,&QTcpSocket::readyRead,this,&User::readyRead);
    connect(&m_client,&QTcpSocket::errorOccurred,this,&User::error);
}

quint16 User::showOptions()
{
    quint16 userchoise = 0;
    qInfo() << "===================================================";
    qInfo() << "Please Select the method to perform: ";
    qInfo() << "1- Get Account Number";
    qInfo() << "2- View Account Balance";
    qInfo() << "3- View Transaction history";
    qInfo() << "4- Make Transaction";
    qInfo() << "5- Transfer Amount";
    qInfo() << "6- View Account";
    qInfo() << "7- Exit Program";
    qInfo() << "===================================================";

    cout << "User: ";
    cout.flush();
    qin >> userchoise;
    return userchoise;
}

bool User::SystemLogIn()
{
    m_request = "login";
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    outStream<<m_request<<m_role;
    std::string username, password;
    // QString username, password;

    cout << "login>> <username> <password>" << Qt::endl;
    cout << "Username: " << Qt::endl;
    // cout.flush();
    std::cin >> username >> password;
    // username = qin.readLine();
    // cout << "Username: " << Qt::endl;
    // password = qin.readLine();

    quint8 counter = 0;
    bool ok = false;
    QString Username=QString::fromStdString(username);
    QString Password=QString::fromStdString(password);
    cout << "Hello: " << Username << Qt::endl;


    while (counter < 3)
    {
        counter++;

        if (!Username.isEmpty() && !Password.isEmpty())
        {
            ok = true;
            break;
        }


    }
    if (ok)
    {
        //send the data to the server to handle that
        outStream<<Username<<Password;
        m_client.waitForReadyRead();
        qInfo()<<"login is :"<<m_serverrespond.toBool();
        ok = m_serverrespond.toBool();
        if(m_serverrespond.toBool() == true)
        {
            m_username = Username;
            sendrequesttoserver("GetAccountNumber");

        }
    }
    else
    {
        qInfo() << "User not found, please try again later..";
    }

    return ok;
}



bool User::CheckAccountNumber(QString accountnumber)
{
    // bool ok;
    // quint32 AccountNumber = accountnumber.toInt(&ok);
    if( accountnumber.toInt() <= 0)
    {
        qInfo() << "Invalid Account Number!";
        return false;
    }
    else
    {
        return true;
    }
}

void User::ViewAccountBalance()
{
    //quint32 response;
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    // cout << "view Account Balance <accountnumber>" << Qt::endl;
    // cout << "Account number>> " << Qt::endl;
    // std::cin >> accountnumber;
    // QString Accountnumber=QString::fromStdString(accountnumber);

    // qin >> accountnumber;

    //bool isValid= CheckAccountNumber(accountnumber);
    if(CheckAccountNumber(m_accountnumber))
    {
        outStream << m_username;
        m_client.waitForBytesWritten();
        //wait for the respond from the server to view it to the client
        m_client.waitForReadyRead();
        if(m_serverrespond.toBool() == true)
        {
            qInfo()<<"Your account balance is :"<<m_accountBalance;
        }
        else
        {
            qInfo()<<"Your account balance is not found";
        }
    }
    else
    {
        qInfo() << "Invalid account number please try again";
    }


}

void User::ViewTransactionHistory()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    // QString accountnumber;
    std::string count;
    qInfo() << "View Transaction History <accountnumber> <count> ";
    qInfo() << "Transaction History>> ";
    std::cin >> count;
    QString Count=QString::fromStdString(count);

    if(Count.toInt() <= 0)
    {
        qInfo() << "Please enter a valid count.";
        ViewTransactionHistory();
    }
    else
    {
        outStream << m_username << Count;
        m_client.waitForBytesWritten();
        //wait for the respond from the server to view it to the client
        m_client.waitForReadyRead();
        qDebug().noquote() <<"Your transaction history is :"<<m_serverrespond.toString();

        // QByteArray History = "ViewTransactionHistory " + accountnumber + " " + count;
        // client->send_Request(History);
        //qDebug() << "Account Balance is: " << response;
    }

}


void User::Get_Account_number()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);

    cout << "Get Account Number <username>" << Qt::endl;
    qInfo() << "getting account number";


    //send request to server to get data
    outStream << m_username;
    m_client.waitForBytesWritten();
    m_client.waitForReadyRead();

    //get the response from the server & check on it

    if(m_serverrespond.toBool() == true)
    {
        qInfo()<<"Your Account number is :"<<m_accountnumber;

    }
    else
    {
        qInfo()<<"Your Account number is not found please try again";

    }

}

void User::Make_Transaction()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);

    std::string transaction_amount, transactiontype;
    cout << "for in transaktion type in for out type out" << Qt::endl;
    std::cin >> transactiontype;
    cout << "Make Transaction>> <transaction amount>" << Qt::endl;
    cout << "Transaction Amount>> " << Qt::endl;

    std::cin  >> transaction_amount;

    QString TransactionAmount=QString::fromStdString(transaction_amount);
    QString Transactiontype=QString::fromStdString(transactiontype);

    QString Transaction;
    if(Transactiontype.toLower() == "out")
    {
        Transaction = "-" + TransactionAmount;
    }
    else if(Transactiontype.toLower() == "in")
    {
        Transaction = TransactionAmount;
    }
    else
    {
        cout << "Invalid option. Please try again" << Qt::endl;
        Make_Transaction();
    }
    qInfo() << "transaction = " << TransactionAmount;
    outStream <<Transaction << m_accountnumber << m_username;
    m_client.waitForBytesWritten();
    m_client.waitForReadyRead();
    qInfo()<<"make transaction is :"<<m_serverrespond.toBool();
}


void User::Transfer_Amount()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    std::string to_Username, transfer_amount;
    // quint32 transfer_amount;
    // cout << "Transfer Amount>> <to_Username> <transfer amount>" << Qt::endl;
    cout << "Transfer Amount>> <to_Username> <transfer amount>" << Qt::endl;
    cout << "to_Username>> " << Qt::endl;
    std::cin >> to_Username;
    bool ok = true;
    QString To_accountnumber=QString::fromStdString(to_Username);
    outStream <<ok << m_username << To_accountnumber;
    m_client.waitForBytesWritten();
    m_client.waitForReadyRead();
    //qInfo()<<"transfer mony is :"<<m_serverrespond.toBool();

    if(m_serverrespond.toBool() == true)
    {
        bool check = false;
        outStream<<m_request<<m_role<< check;
        cout << "Transfer amount>> " << Qt::endl;
        std::cin >> transfer_amount;
        QString Transfer_amount=QString::fromStdString(transfer_amount);
        outStream << m_username <<To_accountnumber <<Transfer_amount;
        m_client.waitForBytesWritten();
        m_client.waitForReadyRead();
        if(m_serverrespond.toBool() == true)
        {
            qInfo()<<"Transfer Mony is :"<<m_serverrespond.toBool();

        }
        else
        {
            qInfo() << "Transfer Amount is not valid please try again";
        }
    }
    else
    {
        qInfo() << "Wrong  From User name try again";
        Transfer_Amount();

    }
}

void User::ViewAccount()
{
    QDataStream outStream(&m_client);
    outStream.setVersion(QDataStream::Qt_6_6);
    cout << "View account>>" << Qt::endl;
    outStream << m_username;
    m_client.waitForBytesWritten();
    m_client.waitForReadyRead();
    cout <<"ViewAccount:\n"<< "{\n" << m_serverrespond.toString() << "\n}" << Qt::endl;
}

void User::sendrequesttoserver(QString request)
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
        Get_Account_number();
    }
    else if(request == "AccountBalance")
    {
        ViewAccountBalance();
    }
    else if( request == "TransactionHistory")
    {
        ViewTransactionHistory();

    }
    else if(request == "MakeTransaction")
    {
        Make_Transaction();
    }
    else if(request=="TransferAmount")
    {
        //call the method Transfer Account to handle this request
        Transfer_Amount();
    }
    else if(request=="ViewAccount")
    {
        //call the method view Account to handle this request
        ViewAccount();
    }
    else
    {
        qDebug() << "No Info recieved.";
    }


}


void User::connectToHost(QHostAddress host, quint16 port)
{
    if(m_client.isOpen()) disconnect();
    //qInfo()<<"connecting to host"<<host<<"on port:"<<port;
    m_client.connectToHost(host,port);
}

void User::disconnect()
{
    m_client.close();
    m_client.waitForDisconnected();
}

void User::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    m_client.write(userInput.toUtf8());
}

void User::disconnected()
{
    qInfo()<<"Thank you!...system is closing";
    qInfo()<<"Disconnected";
}

void User::error(QAbstractSocket::SocketError socketerror)
{
    qInfo()<<"Error:"<<socketerror<<m_client.errorString();
}

void User::stateChanged(QAbstractSocket::SocketState socketstate)
{
    QMetaEnum metaenum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str= metaenum.valueToKey(socketstate);
}

void User::readyRead()
{
    QDataStream inStream(&m_client);
    inStream.setVersion(QDataStream::Qt_6_6);

    if ( m_request =="ViewAccount")
    {
        QString Account;
        //take the money in the account from the server and save it in the server respond
        inStream>>Account;
        m_serverrespond.setValue(Account);
    }
    else if(m_request=="TransferAmount"||m_request=="MakeTransaction")
    {
        bool respond;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
    }
    else if(m_request=="login")
    {
        bool respond;
        //know if the transaction succeed or not from the server and save it in the server respond
        inStream>>respond;
        m_serverrespond.setValue(respond);
    }
    else if(m_request == "TransactionHistory")
    {
        QString respond;
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

}


