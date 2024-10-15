//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : server.h
// Summary  : Includes the function/class declarations
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 03/10/2024
//
//*****************************************************************************
#ifndef _SERVER_H_
#define _SERVER_H_

//******************************* Include Files *******************************
#include <iostream>
#include "socket_common.h"
#include "types.h"

//******************************** ServerCom **********************************
// Description : Derived class from the SocketCom it includes the 
//               functionalities of the server side socket communication.
// Usage       : It includes the functionalities of the server side 
//               socket communication.
// Note        : Nil
//*****************************************************************************
class ServerCom : public SocketCom
{
public:
    bool startConnection(void);
    bool setOption(int32 lSocDes);
    bool listenSocket(int32 lSocDes);
    bool acceptConnection(int32 lSocDes, int32* lClientSoc);
    bool establishCon();
    bool exceedTime(uint32 lTimeDif);

private:
    uint32 ulPrevTime = 0;
};

#endif // _SERVER_H_ 

// EOF
