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
#ifndef _SERVER_H_
#define _SERVER_H_

//******************************* Include Files *******************************
#include <iostream>
#include "socket_common.h"
#include "types.h"

class ServerCom : public SocketCom
{
public:
    bool serverStartCon(void);

private:
    bool setOption(void);
    bool listenSocket(void);
    bool acceptConnection(void);
};

#endif // _SERVER_H_ 

// EOF