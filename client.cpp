//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.cpp
// Summary  : This file includes the client functionalities of the socket
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 01/10/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "client.h"

//******************************* Local Types *********************************
using namespace std;

//********************************* exceedTime ********************************
//Purpose : Checks the current time exceeds the previous time and by a 
//          specified time difference and updates the previous time 
//          accordingly.
//Inputs  : lTimeDif
//Outputs : Nil
//Return  : Return the time difference exceeded status.
//Notes   : Nil
//*****************************************************************************
bool ClientCom::exceedTime(uint32 lTimeDif)
{
    bool blDelayTime = false;
    time_t ulCurTime = 0;

    time(&ulCurTime);

    if ((ulCurTime - ulPrevTime) >= lTimeDif)
    {
        blDelayTime = true;
        ulPrevTime = ulCurTime;
        // cout << "Unix time : " << ulCurTime << endl;
    }

    return blDelayTime;
}

//***************************** startConnection *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket creation state
//Notes   : Nil
//*****************************************************************************
bool ClientCom::startConnection(void)
{
    bool blConState = false;

    do
    {
        blConState = createSocket(); 
        if (blConState == false)
        {
            cout << "Socket creation failed\n";
            break;
        }

        cout << "Socket created\n";
        blConState = bindSocket();
        if (blConState == false)
        {
            cout << "Couldn't bind socket\n";
            break;
        }

        blConState = connectToSocket(getSocketDes());
        if (blConState == false)
        {
            cout << "Couldn't connect socket\n";
        }
        else
        {
            cout << "Connected" << endl;
        }

    } while (false);

    return blConState;
}

//****************************** connectToSocket ******************************
//Purpose : Try to connect to the server
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool ClientCom::connectToSocket(int32 lSocDes)
// static bool connectToSocket(struct sockaddr_in pstServerAdd)
{
    bool blStatus = true;
    int32 lConState = 0;

    // Connect to the server
    lConState = connect(lSocDes,
                        (struct sockaddr*)& stServerAddress,
                        sizeof(struct sockaddr));

    if(lConState < 0)
    {
        blStatus = false;
    }

    // Return connection status
    return blStatus;
}

// EOF
