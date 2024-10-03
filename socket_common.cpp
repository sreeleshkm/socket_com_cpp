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
#include "socket_common.h"

bool SocketCom::createSocket(void)
{
    bool blStatus = true;

    glSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(glSocketDescriptor < 0)
    {
        blStatus = false;
    }

    return blStatus;
}

bool SocketCom::BindSocket(void)
{
    bool blStatus = true;
    int8 cBindState = 0;
    // Initializing structure elements for address
    stServerAddress.sin_family = AF_INET;
    // Convert port to network byte order using htons
    stServerAddress.sin_port = htons(CONNECTION_PORT);
    // Any address available
    stServerAddress.sin_addr.s_addr = INADDR_ANY;
    // Assigning null character to the last index of the character array
    stServerAddress.sin_zero[8] = NULL_CHARACTER;
#if (SOCKET_COM == SOC_SER)
    // Bind socket with values address and port from the sockaddr_in structure
    cBindState = bind(glSocketDescriptor, (struct sockaddr*)&stServerAddress, 
                sizeof(struct sockaddr));

    // Check if the binding was successful
    if(cBindState < 0)
    {
        blStatus = false;
    }
#endif

    return blStatus;
}

void SocketCom::setSocketDescriptor(uint32 ulSctDes)
{
    glSocketDescriptor = ulSctDes;
}

uint32 SocketCom::getSocketDescriptor(void)
{
    return glSocketDescriptor;
}

void SocketCom::setClientSocket(uint32 ulCliSct)
{
    glClientSocket = ulCliSct;
}

uint32 SocketCom::getClientSocket(void)
{
    return glClientSocket;
}

//******************************** readMessage ********************************
//Purpose : Read message from the socket
//Inputs  : lSocket
//Outputs : Nil
//Return  : Return read message status
//Notes   : Nil
//*****************************************************************************
bool SocketCom::readMessage(int32 lSocket)
{
    bool blStatus = false;
    int32 lRecMsgLen = 0;

    // Receive data sent by the client
    lRecMsgLen = recv(lSocket, pucRecieveBuffer, REC_MSG_BUF_LEN, MSG_DONTWAIT);

    if (lRecMsgLen > 0)
    {
        blStatus = true;
    }

    return blStatus;
}

//******************************** sendMessage ********************************
//Purpose : Send message to the client
//Inputs  : pucMessage
//Outputs : Nil
//Return  : Return send message status
//Notes   : Nil
//*****************************************************************************
bool SocketCom::sendMessage(int32 lSocket, uint8* pucMessage)
{
    bool blStatus = true;
    uint32 ulTransferMsgLen = 0;
    uint32 ulMsgLen = 0;

    ulMsgLen = strlen((const char *)pucMessage);

    // Send data to the client
    ulTransferMsgLen = send(lSocket, pucMessage, ulMsgLen, 0);

    if (ulTransferMsgLen != ulMsgLen)
    {
        blStatus = false;
    }

    return blStatus;
}
