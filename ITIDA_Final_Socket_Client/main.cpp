#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include "user.h"
#include "admin.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qin(stdin);
    QTextStream qout(stdout);
    QString request, userInput;
    quint16 Choise;
    bool isLoggedIn;
    qout<< "Welcome to the Banking App!" << Qt::endl;
    qout << "Type 'user' to user mode." << Qt::endl;
    qout << "Type 'admin' to admin mode."<< Qt::endl;
    qout << "Type 'exit' to quit."<< Qt::endl;

    while(true)
    {

        qout << "bank>> " << Qt::endl;
        userInput = qin.readLine();

        if(userInput.toLower() == "exit")
        {
            qout << "Exiting the Banking App." << Qt::endl;
            exit(0);
        }
        else if(userInput.toLower() == "user")
        {
            User usr;
            usr.connectToHost(QHostAddress::LocalHost, 1234);
            isLoggedIn = usr.SystemLogIn();
            while(isLoggedIn == true)
            {
                //show the valid commands for user to choose one
                Choise = usr.showOptions();
                switch(Choise)
                {
                case 1:
                    request = "GetAccountNumber";
                    usr.sendrequesttoserver( request);
                    //usr.Get_Account_number();
                    break;
                case 2:
                    request = "AccountBalance";
                    usr.sendrequesttoserver( request);
                    //usr.ViewAccountBalance();
                    break;
                case 3:
                    request = "TransactionHistory";
                    usr.sendrequesttoserver( request);
                    // usr.ViewTransactionHistory();
                    break;
                case 4:
                    request = "MakeTransaction";
                    usr.sendrequesttoserver( request);
                    // usr.Make_Transaction();
                    break;
                case 5:
                    request = "TransferAmount";
                    usr.sendrequesttoserver( request);
                    //usr.Transfer_Amount();
                    break;
                case 6:
                    request = "ViewAccount";
                    usr.sendrequesttoserver( request);
                    // usr.ViewAccount();
                    break;
                case 7:
                    qInfo() <<  " Exit Program";
                    isLoggedIn = false;
                    break;
                default:
                    qDebug() << "Invalid choise please try again.";
                    break;
                }
            }
        }
        else if(userInput.toLower() == "admin")
        {
            Admin admin;
            admin.connectToHost(QHostAddress::LocalHost, 1234);

            isLoggedIn = admin.SystemLogIn();
            while(isLoggedIn == true)
            {

                Choise = admin.showOptions();
                switch(Choise)
                {
                case 1:
                    request = "GetAccountNumber";
                    admin.sendrequesttoserver( request);
                    // admin.GetAccountnumber();
                    break;
                case 2:
                    request = "AccountBalance";
                    admin.sendrequesttoserver( request);
                    //admin.ViewAccountBalance();
                    break;
                case 3:
                    request = "ViewTransactionHistory";
                    admin.sendrequesttoserver( request);
                    //admin.ViewTransactionHistory();
                    break;
                case 4:
                    request = "ViewBankDatabase";
                    admin.sendrequesttoserver( request);
                    //admin.ViewBankDatabase();
                    break;
                case 5:
                    request = "CreateNewUser";
                    admin.sendrequesttoserver( request);
                    // admin.CreateNewUser();
                    break;
                case 6:
                    request = "DeleteUser";
                    admin.sendrequesttoserver( request);
                    // admin.DeleteUser();
                    break;
                case 7:
                    request = "UpdateUser";
                    admin.sendrequesttoserver( request);
                    // admin.UpdateUser();
                    break;
                case 8:
                    qInfo() <<  " Exit Program";
                    isLoggedIn = false;
                    break;
                default:
                    qDebug() << "Invalid choise please try again.";
                    break;
                }
            }
        }
        else
        {

            qout << "Invalid command. Type 'user' to user mode, `admin` to admin mode or 'exit' to quit.";
            qout.flush();
        }

    }
    return a.exec();
}
