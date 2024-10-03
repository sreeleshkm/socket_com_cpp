//********************** Socket communication *********************************
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
#include "client.h"

using namespace std;

//***************************** Local Constants *******************************
#define SEND_MES_TIME_DIF   (5)

uint32 ClientCom::exceedTime(uint32 lTimeDif)
{
    bool blDelayTime = false;
    time_t ulCurTime = 0;

    time(&ulCurTime);

    if ((ulCurTime - ulPrevTime) >= lTimeDif)
    {
        blDelayTime = true;
        ulPrevTime = ulCurTime;
    }

    return blDelayTime;
}

bool ClientCom::checkTime(void)
{
    bool blStatus = false;

    blStatus = exceedTime(SEND_MES_TIME_DIF);

    if (blStatus == true)
    {
        cout << "Time : " << ulPrevTime << endl;
    }

    return blStatus;
}

//////////////////////////////////////////////////

//****************************** startClientCon *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool ClientCom::clientStartCon(void)
{
    bool blConState = false;
    // Structure to represent the address
    struct sockaddr_in stServerAddress = {0};

    blConState = createSocket();

    if (blConState == false)
    {
        cout << "Socket creation failed\n";
    }
    else
    {
        cout << "Socket created\n";
        blConState = BindSocket();

        if (blConState == false)
        {
            cout << "Couldn't bind socket\n";
        }
        else
        {
            blConState = connectToSocket();

            if (blConState == false)
            {
                cout << "Couldn't connect socket\n";
            }
        }
    }

    return blConState;
}

//****************************** connectToSocket ******************************
//Purpose : Try to connect to the server
//Inputs  : pstServerAdd
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool ClientCom::connectToSocket(void)
// static bool connectToSocket(struct sockaddr_in pstServerAdd)
{
    bool blStatus = true;
    int32 lConState = 0;

    // Connect to the server
    lConState = connect(glSocketDescriptor,
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
