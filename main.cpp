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
#include "socket_common.h"

#if (SOCKET_COM == SOC_SER)
#include "server.h"
#elif (SOCKET_COM == SOC_CLI)
#include "client.h"
#endif

//******************************* Local Types *********************************
using namespace std;

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
    blStatus = Server.startConnection();
#elif (SOCKET_COM == SOC_CLI)
    blStatus = Client.startConnection();
#endif

    if (blStatus == true)
    {
        while (true)
        {
#if (SOCKET_COM == SOC_SER)
            blRcvMsgStatus = Server.readMessage(Server.getCliSoc());
#elif (SOCKET_COM == SOC_CLI)
            blRcvMsgStatus = Client.readMessage(Client.getSocketDes());
#endif
            if (blRcvMsgStatus == true)
            {
#if (SOCKET_COM == SOC_SER)
                cout << "Message : " << Server.pucRecieveBuffer << endl;
                blMesState = Server.sendMessage(Server.getCliSoc(), pucMessage);
            
                if (blMesState == false)
                {
                    printf("Message not send\n");
                }
#elif (SOCKET_COM == SOC_CLI)
                cout << "Message : " << Client.pucRecieveBuffer << endl;
#endif
            }

#if (SOCKET_COM == SOC_CLI)
            blSndMsgStatus = Client.exceedTime(SEND_MES_TIME_DIF);

            if (blSndMsgStatus == true)
            {
                Client.sendMessage(Client.getSocketDes(), pucMessage);
            }
#endif
        }
    }

    return 0;
}

// EOF
