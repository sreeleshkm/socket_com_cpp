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
#ifndef _SOCKET_COMMON_H_
#define _SOCKET_COMMON_H_

//******************************* Include Files *******************************
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "types.h"

//***************************** Global Constants *******************************
#define SOC_CLI                 (0)
#define SOC_SER                 (1)
#define CONNECTION_PORT         (3500)
#define NULL_CHARACTER          ('\0')
#define REC_MSG_BUF_LEN         (200)
#define SEND_MES_TIME_DIF       (5)

//******************************** SocketCom **********************************
// Description : Includes the common functions and variable of the socket
//               communication
// Usage       : This is the base class of the server and client derived class.
// Note        : Nil
//*****************************************************************************
class SocketCom
{
public:
    int32 glSocketDescriptor = 0;
    int32 glClientSocket = 0;
    uint8 pucRecieveBuffer[REC_MSG_BUF_LEN] = {0};
    // Structure to represent the address
    struct sockaddr_in stServerAddress = {0};

    bool createSocket(void);
    bool bindSocket(void);
    bool readMessage(int32 lSocket);
    bool sendMessage(int32 lSocket, uint8* pucMessage);
};

#endif // _SOCKET_COMMON_H_

// EOF
