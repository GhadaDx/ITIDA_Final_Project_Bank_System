## ITIDA_Final_Project_Bank_System

# Description

The applications uses Qt Frame Work With C++ 
It consests of two applications one for the client and the other for the server to create the client-server model.

# Client_Application
client application done based on QTcpSockets with three classes 
1. Client Class
   It is an Abstract QObject class created to be the base for the other two classes and for starting the client socket.
2. Admin Class
   This class inherit from the Client base class and implement its pure virtual functions to start the connection with the server.
it is also handel the admin request and serialize the data to the server using datastream and recive the server response using the same method.
it allow the admin to have the control all over the database for the user.
3. User Calss
this class inherit from the Client base class and implement its pure virtual functions to start the connection with the server.
it is also handel the user request and serialize the data to the server using datastream and recive the server response using the same method.

# System Inputs 
The input data requested from the user to inter it through the terminal as the two applications
the server and the client are console applications and the interface with the user done using CLI.

# Server_Application
Server application done based on QTcpSockets and QTcpServer with two classes 
1. Server Class
   The server class starts the and be ready for any connection from the client and it uses pool threading to put every client in separated thread.
   once the client connected to the server it starts to read the data using datastreams and process over it to retrieve or handel the data in the database class.
2. JsonHandler Class
   Json class used to handel the data base and work with the json files to get or add data to them.


# Finite State Machine for the Project
![](images/Image1.png)

# Demo

