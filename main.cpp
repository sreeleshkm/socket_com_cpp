//********************** Socket communication ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : main.c
// Summary  : Main file of the server communication
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 20/09/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <iostream>
#include "socket_common.h"
#include "client.h"
#include "server.h"

using namespace std;

int main()
{
    cout << "Program Started\n";
    bool blStatus = false;
    bool blRcvMsgStatus = false;
    bool blSndMsgStatus = false;

#if (SOCKET_COM == SOC_SER)
    bool blMesState = false;
    uint8 pucMessage[10] = "Recieved!";
    ServerCom Server;
#elif (SOCKET_COM == SOC_CLI)
    uint8 pucMessage[4] = "Hii";
    ClientCom Client;
#endif

#if (SOCKET_COM == SOC_SER)
    blStatus = Server.serverStartCon();
#elif (SOCKET_COM == SOC_CLI)
    blStatus = Client.clientStartCon();
#endif

    if (blStatus == true)
    {
        while (true)
        {
#if (SOCKET_COM == SOC_SER)
            blRcvMsgStatus = Server.readMessage(Server.glClientSocket);
#elif (SOCKET_COM == SOC_CLI)
            blRcvMsgStatus = Client.readMessage(Client.glSocketDescriptor);
#endif
            if (blRcvMsgStatus == true)
            {
#if (SOCKET_COM == SOC_SER)
                cout << "Message : " << Server.pucRecieveBuffer << endl;
                blMesState = Server.sendMessage(Server.glClientSocket, pucMessage);
            
                if (blMesState == false)
                {
                    printf("Message not send\n");
                }
#elif (SOCKET_COM == SOC_CLI)
                cout << "Message : " << Client.pucRecieveBuffer << endl;
#endif
            }
#if (SOCKET_COM == SOC_CLI)
            blSndMsgStatus = Client.checkTime();

            if (blSndMsgStatus == true)
            {
                Client.sendMessage(Client.glSocketDescriptor, pucMessage);
            }
#endif
        }
    }

    return 0;
}