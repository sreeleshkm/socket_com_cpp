//********************** Socket communication - Client ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.h
// Summary  : Includes the function declarations 
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 13/09/2024
//
//*****************************************************************************
#ifndef _CLIENT_H_
#define _CLIENT_H_

//******************************* Include Files *******************************
#include <iostream>
#include <time.h>
#include "socket_common.h"
#include "types.h"

// 
class ClientCom : public SocketCom
{
public:
    bool checkTime(void);
    bool clientStartCon(void);
    bool connectToSocket(void);

private:
    uint32 ulPrevTime = 0;
    uint32 exceedTime(uint32 lTimeDif);
};

#endif // _CLIENT_H_ 

// EOF