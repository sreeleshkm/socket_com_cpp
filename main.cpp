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
#include <mutex>
#include "socket_common.h"

#if (SOCKET_COM == SOC_SER)
#include "server.h"
#elif (SOCKET_COM == SOC_CLI)
#include "client.h"
#endif


//***************************** Local Constants *******************************
#define CLI_MAX_CNT 10

//******************************* Local Types *********************************
using namespace std;

#if (SOCKET_COM == SOC_SER)
uint8 ucMessage[10] = {0};
uint8 ucMsgArCurPos = 0;
uint8 ucMsgArPrvPos = 0;
uint8 ulClientCount = 0;
uint8 ucClientActive[CLI_MAX_CNT] = {0};
uint8 ulPreClientCount = 0;
counting_semaphore sem(1);
mutex mtx;
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
            ulClientCount++;
        }
    }
}

//****************************** threadWriteMsg *******************************
//Purpose : This thread is used to set message buffer
//Inputs  : ucID, Server
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void threadWriteMsg(uint8 ucID, ServerCom *Server)
{
    bool blStatus = false;

    while (ucClientActive[ucID] == 1)
    {
        blStatus = Server->exceedTime(SEND_MES_TIME_DIF);

        if (blStatus == true)
        {
            // if (sem.try_acquire())
            // {
                ucMessage[ucMsgArCurPos] = ucID;
                ucMsgArCurPos++;
                if (ucMsgArCurPos == 10)
                {
                    ucMsgArCurPos = 0;
                }
                cout << "set message ID : " << ucID + 48 << endl;

                // sem.release();
            // }
        }
    }

    return;
}

//****************************** threadSendMsg ********************************
//Purpose : This thread is used to send message to the client
//Inputs  : Nil
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void threadSendMsg(ServerCom *Server)
{
    bool blStatus = false;
    int32 socDes = 0;
    uint8 pucMessage[10] = "clientN\n";

    while (true)
    {
        if (ucMsgArCurPos != ucMsgArPrvPos)
        {
            // if (sem.try_acquire())
            // {
            if (ucClientActive[ucMessage[ucMsgArPrvPos]] == 1)
            {
                pucMessage[6] = ucMessage[ucMsgArPrvPos] + 48;
                // cout << "ID : " << ucID + 48 << endl;
                socDes = Server->getCliSoc(ucMessage[ucMsgArPrvPos]);
                cout << "Send Message : " << ucMessage[ucMsgArPrvPos] + 48 << " ID " << socDes + 48 << endl;
                Server->sendMessage(&socDes, pucMessage);
                // cout << "Send Message" << endl;
            }
            //     sem.release();
            // }

            ucMsgArPrvPos++;

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
    uint8 ucWrCount = 0;
    int32 socDes = 0;

#if (SOCKET_COM == SOC_SER)
    vector <uint8> vulThreadClient {0};
    vector <thread> threads;
    bool blMesState = false;
    uint8 pucMessage[10] = "Recieved!";
    ServerCom Server;
#elif (SOCKET_COM == SOC_CLI)
    uint8 pucMessage[10] = "Hii\n";
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
            if (ulClientCount != ulPreClientCount)
            {
                threads.emplace_back(threadWriteMsg, vulThreadClient[ulPreClientCount], &Server);
                ucClientActive[ulPreClientCount] = 1;   
                cout << "New Thread created ID : " <<  vulThreadClient[ulPreClientCount] + 48 
                    << "act state" << ucClientActive[ulPreClientCount]  + 48 << endl;
                ulPreClientCount++;
                vulThreadClient.push_back(ulPreClientCount);
            }
            socDes = Server.getCliSoc(ucRdCount);
            // cout << "Soc des : " << socDes << endl;
            if (ucClientActive[ucRdCount] == 1)
            {
                blRcvMsgStatus = Server.readMessage(&socDes);
#elif (SOCKET_COM == SOC_CLI)
                socDes = Client.getSocketDes();
                blRcvMsgStatus = Client.readMessage(&socDes);
#endif
                if (blRcvMsgStatus != 0)
                {
                    if (blRcvMsgStatus > 0)
                    {
#if (SOCKET_COM == SOC_SER)
                        socDes = Server.getCliSoc(ucWrCount);
                        ucWrCount++;
                        if (ucWrCount == Server.getClientCount())
                        {
                            ucWrCount = 0;
                        }
                        cout << "Message from " << ucRdCount + 48 << " : " << Server.pucRecieveBuffer;
                        blMesState = Server.sendMessage(&socDes, pucMessage);
                    
                        if (blMesState == false)
                        {
                            printf("Message not send\n");
                        }
#elif (SOCKET_COM == SOC_CLI)
                    cout << "Message from Server : " << Client.pucRecieveBuffer;
#endif
                    }
                }
#if (SOCKET_COM == SOC_SER)
                else
                {
                    ucClientActive[ucRdCount] = 0;
                    cout << "Client disconnected ID : " << ucRdCount + 48 << endl;
                }
            }
            ucRdCount++;
            if (ucRdCount == Server.getClientCount())
            {
                ucRdCount = 0;
            }
#endif


#if (SOCKET_COM == SOC_CLI)
            // blSndMsgStatus = Client.exceedTime(SEND_MES_TIME_DIF);

            // if (blSndMsgStatus == true)
            // {
            //     socDes = Client.getSocketDes();
            //     Client.sendMessage(&socDes, pucMessage);
            //     cout << "Send Message" << endl;
            // }
#endif
        }
    }

    return 0;
}

// EOF
