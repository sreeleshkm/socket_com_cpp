//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : main.cpp
// Summary  : Main file of the server communication
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 01/10/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include "socket_common.h"

#if (SOCKET_COM == SOC_SER)
#include "server.h"
#elif (SOCKET_COM == SOC_CLI)
#include "client.h"
#endif

//******************************* Local Types *********************************
using namespace std;

//***************************** Local Constants *******************************
#define CLIENT_MAX_COUNT 20


//***************************** Local Variables *******************************
#if (SOCKET_COM == SOC_SER)
uint8 ucMessage[10] = {0};
uint8 ucMsgArCurPos = 0;
uint8 ucMsgArPrvPos = 0;
uint8 ucClientCount = 0;
uint8 ucClientActive[CLIENT_MAX_COUNT] = {0};
uint8 ulPreClientCount = 0;
counting_semaphore sem(1);
#endif

#if (SOCKET_COM == SOC_SER)
//******************************* threadEstCon ********************************
//Purpose : This thread is used to listen and accept the client request
//Inputs  : Server
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void threadEstCon(ServerCom *Server)
{
    bool blStatus = false; 

    while (true)
    {
        blStatus = Server->establishCon();

        if (blStatus == true)
        {
            ucClientCount++;
        }
    }
}

//****************************** threadWriteMsg *******************************
//Purpose : This thread is used to set message buffer
//Inputs  : ucClientID, Server
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void threadWriteMsg(uint8 ucClientID, ServerCom *Server)
{
    bool blStatus = false;

    while (ucClientActive[ucClientID] == 1)
    {
        blStatus = Server->exceedTime(SEND_MES_TIME_DIF);

        if (blStatus == true)
        {
            if (sem.try_acquire_for(chrono::seconds(1)))
            {
                ucMessage[ucMsgArCurPos] = ucClientID;
                ucMsgArCurPos++;

                if (ucMsgArCurPos == 10)
                {
                    ucMsgArCurPos = 0;
                }

                sem.release();
            }
        }
    }

    return;
}

//****************************** threadSendMsg ********************************
//Purpose : This thread is used to send message to the client
//Inputs  : Server
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void threadSendMsg(ServerCom *Server)
{
    int32 socDes = 0;
    uint8 pucMessage[10] = "client  \n";

    while (true)
    {
        if (ucMsgArCurPos != ucMsgArPrvPos)
        {
            if (sem.try_acquire_for(chrono::seconds(1)))
            {
                if (ucClientActive[ucMessage[ucMsgArPrvPos]] == 1)
                {
                    pucMessage[7] = ucMessage[ucMsgArPrvPos] + 48;
                    socDes = Server->getCliSoc(ucMessage[ucMsgArPrvPos]);
                    cout << "Send Message : " << ucMessage[ucMsgArPrvPos] + 48;
                    cout << " ID " << socDes + 48 << endl;
                    Server->sendMessage(&socDes, pucMessage);
                }

                ucMsgArPrvPos++;
                sem.release();
            }

            if (ucMsgArPrvPos == 10)
            {
                ucMsgArPrvPos = 0;
            }
        }
    }
}
#endif

//*********************************** main ************************************
//Purpose : Socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
int main()
{
    cout << "Program Started\n";
    bool blStatus = false;
    int32 blRcvMsgStatus = 0;
    bool blSndMsgStatus = false;
    uint8 ucRdCount = 0;
    int32 socDes = 0;

#if (SOCKET_COM == SOC_SER)
    vector <uint8> vulThreadClient {0};
    vector <thread> threads;
    ServerCom Server;
#elif (SOCKET_COM == SOC_CLI)
    uint8 pucMessage[15] = "from client\n";
    ClientCom Client;
#endif

#if (SOCKET_COM == SOC_SER)
    blStatus = Server.startConnection();
#elif (SOCKET_COM == SOC_CLI)
    blStatus = Client.startConnection();
#endif

    if (blStatus == true)
    {
#if (SOCKET_COM == SOC_SER)
        // Create and start thread 1
        thread t1(threadEstCon, &Server);
        thread t2(threadSendMsg, &Server);
        
        // Wait for thread 1 to finish
        t1.detach();
        t2.detach();
#endif

        while (true)
        {
#if (SOCKET_COM == SOC_SER)
            if (ucClientCount != ulPreClientCount)
            {
                threads.emplace_back(threadWriteMsg,
                                    vulThreadClient[ulPreClientCount],
                                    &Server);
                ucClientActive[ulPreClientCount] = 1;
                cout << "New Thread created ID : ";
                cout <<  vulThreadClient[ulPreClientCount] + 48;
                ulPreClientCount++;
                vulThreadClient.push_back(ulPreClientCount);
            }
            if (ucClientActive[ucRdCount] == 1)
            {
                socDes = Server.getCliSoc(ucRdCount);
                blRcvMsgStatus = Server.readMessage(&socDes);
                if (blRcvMsgStatus == 0)
                {
                    ucClientActive[ucRdCount] = 0;
                    cout << "Client disconnected ID : ";
                    cout << ucRdCount + 48 << endl;
                }
                else if (blRcvMsgStatus > 0)
                {
                    cout << "Message : " << Server.pucRecieveBuffer << endl;
                }
            }

            ucRdCount++;
            if (ucRdCount == Server.getClientCount())
            {
                ucRdCount = 0;
            }

#elif (SOCKET_COM == SOC_CLI)
            socDes = Client.getSocketDes();
            blRcvMsgStatus = Client.readMessage(&socDes);
            if (blRcvMsgStatus != 0)
            {
                if (blRcvMsgStatus > 0)
                {
                    cout << "Message from Server: " << Client.pucRecieveBuffer;
                }
            }
#if 0
            blSndMsgStatus = Client.exceedTime(SEND_MES_TIME_DIF);

            if (blSndMsgStatus == true)
            {
                Client.sendMessage(&socDes, pucMessage);
                cout << "Send Message" << endl;
            }
#endif
#endif
        }
    }

    return 0;
}

// EOF
