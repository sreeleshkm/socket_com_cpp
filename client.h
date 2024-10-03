//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.h
// Summary  : Includes the function declarations 
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 01/10/2024
//
//*****************************************************************************
#ifndef _CLIENT_H_
#define _CLIENT_H_

//******************************* Include Files *******************************
#include <iostream>
#include <time.h>
#include "socket_common.h"
#include "types.h"

//******************************** ClientCom **********************************
// Description : Derived class from the SocketCom it includes the 
//               functionalities of the client side socket communication.
// Usage       : It includes the functionalities of the client side 
//               socket communication.
// Note        : Nil
//*****************************************************************************
class ClientCom : public SocketCom
{
public:
    bool exceedTime(uint32 lTimeDif);
    bool startConnection(void);
    bool connectToSocket(void);

private:
    uint32 ulPrevTime = 0;
};

#endif // _CLIENT_H_

// EOF