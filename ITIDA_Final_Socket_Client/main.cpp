#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include "user.h"
#include "admin.h"


#if defined(Q_OS_WIN)
#include <windows.h>
#elif defined(Q_OS_UNIX)
#include <unistd.h>
#endif

void clearScreen() {
#if defined(Q_OS_WIN)
    system("cls");
#elif defined(Q_OS_UNIX)
    system("clear");
#endif
}
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
            clearScreen();

            User usr;
            usr.connectToHost(QHostAddress::LocalHost, 2020);
            isLoggedIn = usr.SystemLogIn();
            while(isLoggedIn == true)
            {
                //show the valid commands for user to choose one
                Choise = usr.showOptions();
                clearScreen();
                switch(Choise)
                {
                case 1:
                    request = "GetAccountNumber";
                    usr.sendrequesttoserver( request);
                    break;
                case 2:
                    request = "AccountBalance";
                    usr.sendrequesttoserver( request);
                    break;
                case 3:
                    request = "TransactionHistory";
                    usr.sendrequesttoserver( request);
                    break;
                case 4:
                    request = "MakeTransaction";
                    usr.sendrequesttoserver( request);
                    break;
                case 5:
                    request = "TransferAmount";
                    usr.sendrequesttoserver( request);
                    break;
                case 6:
                    request = "ViewAccount";
                    usr.sendrequesttoserver( request);
                    break;
                case 7:
                    qInfo() <<  " Exit Program";
                    isLoggedIn = false;
                    break;
                default:
                    qDebug() << "Invalid choise please try again.";
                    break;
                }
                qInfo() << "===================================================";
                qDebug() << "Do you want to continue? (type `exit` to quit/ `y` to continue):";
                qInfo() << "User:";
                qin >> userInput;
                if(userInput.toLower() == "exit")
                {
                    break;
                }
            }
        }
        else if(userInput.toLower() == "admin")
        {
            clearScreen();
            Admin admin;
            admin.connectToHost(QHostAddress::LocalHost, 2020);
            isLoggedIn = admin.SystemLogIn();
            while(isLoggedIn == true)
            {
                Choise = admin.showOptions();
                clearScreen();
                switch(Choise)
                {
                case 1:
                    request = "GetAccountNumber";
                    admin.sendrequesttoserver( request);
                    break;
                case 2:
                    request = "AccountBalance";
                    admin.sendrequesttoserver( request);
                    break;
                case 3:
                    request = "ViewTransactionHistory";
                    admin.sendrequesttoserver( request);
                    break;
                case 4:
                    request = "ViewBankDatabase";
                    admin.sendrequesttoserver( request);
                    break;
                case 5:
                    request = "CreateNewUser";
                    admin.sendrequesttoserver( request);
                    break;
                case 6:
                    request = "DeleteUser";
                    admin.sendrequesttoserver( request);
                    break;
                case 7:
                    request = "UpdateUser";
                    admin.sendrequesttoserver( request);
                    break;
                case 8:
                    qInfo() <<  " Exit Program";
                    isLoggedIn = false;
                    break;
                default:
                    qDebug() << "Invalid choise please try again.";
                    break;
                }
                qInfo() << "===================================================\n";
                qDebug() << "Do you want to continue? (type `exit` to quit/ `y` to continue):";
                qInfo() << "Admin:";
                qin >> userInput;
                if(userInput.toLower() == "exit")
                {
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
