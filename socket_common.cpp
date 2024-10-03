//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : socket_common.cpp
// Summary  : This file includes the common functionalities of the server and 
//            client
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 03/10/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "socket_common.h"

//****************************** createSocket *********************************
//Purpose : Create socket
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket creation state
//Notes   : Nil
//*****************************************************************************
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

//******************************* bindSocket **********************************
//Purpose : Assign an address to the socket
//Inputs  : Nil
//Outputs : Nil
//Return  : Return the status of the binding state
//Notes   : Nil
//*****************************************************************************
bool SocketCom::bindSocket(void)
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

//******************************** readMessage ********************************
//Purpose : Read message from the socket
//Inputs  : lSocket
//Outputs : Nil
//Return  : Return the read message status
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
//Purpose : Send message to the socket
//Inputs  : lSocket, pucMessage
//Outputs : Nil
//Return  : Return the send message status
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
