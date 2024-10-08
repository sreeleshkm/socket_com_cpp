//*************************** Socket communication ****************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : server.cpp
// Summary  : This file includes the client functionalities of the socket
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 03/10/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "server.h"

//******************************* Local Types *********************************
using namespace std;

//***************************** startConnection *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket creation state
//Notes   : Nil
//*****************************************************************************
bool ServerCom::startConnection(void)
{
    bool blConState = false;

    blConState = createSocket();

    if (blConState == true)
    {
        cout << "Socket created\n";

        blConState = setOption(getSocketDes());

        if (blConState == true)
        {
            blConState = bindSocket();

            if (blConState == true)
            {
                blConState = listenSocket(getSocketDes());

                if (blConState == true)
                {
                    blConState = acceptConnection(getSocketDes());

                    if (blConState == true)
                    {
                        cout << "Server connected to client\n";
                    }
                    else
                    {
                        cout << "Couldn't establish connection with client\n";
                    }
                }
                else
                {
                    cout << "Couldn't listen for connections\n";
                }
            }
            else
            {
                cout << "Couldn't bind socket\n";
            }
        }
        else
        {
            cout << "Couldn't set options\n";
        }
    }
    else
    {
        cout << "Socket creation failed\n";
    }

    return blConState;
}

//******************************** setOption **********************************
//Purpose : Socket options control the behaviour of the socket.
//Inputs  : Nil
//Outputs : Nil
//Return  : Return status of the option setting.
//Notes   : Nil
//*****************************************************************************
bool ServerCom::setOption(int32 lSocDes)
{
    bool blStatus = true;
    int32 lSocOptState = 0;
    // Option value for respective option_name
    uint32 ulOptionValue = 1;

    // Set options for the socket
    lSocOptState = setsockopt(lSocDes, SOL_SOCKET, SO_REUSEADDR,
                    &ulOptionValue, sizeof(ulOptionValue));

    // Check if options are set successfully
    if(lSocOptState < 0)
    {
        blStatus = false;
    }

    return blStatus;
}

//****************************** listenSocket *********************************
//Purpose : Listen for connections from the client.
//Inputs  : lSocDes
//Outputs : Nil
//Return  : Return status of the socket listening
//Notes   : Nil
//*****************************************************************************
bool ServerCom::listenSocket(int32 lSocDes)
{
    bool blStatus = true;
    int32 lLisState = 0;

    // Listen on specified port with a maximum of 4 requests
    lLisState = listen(lSocDes, 4);

    // Check if the socket is listening successfully
    if(lLisState < 0)
    {
        blStatus = false;
    }

    return blStatus;
}

//**************************** acceptConnection *******************************
//Purpose : Accept the connection signal from the client
//Inputs  : lSocDes
//Outputs : Nil
//Return  : Return status of the socket connection
//Notes   : Nil
//*****************************************************************************
bool ServerCom::acceptConnection(int32 lSocDes)
{
    bool blStatus = true;
    uint32 ulLenOfAddress = 0;
    int32 lClientSoc = 0;

    ulLenOfAddress = sizeof(struct sockaddr);

    // Accept connection signals from the client
    lClientSoc = accept(lSocDes, 
                    (struct sockaddr*)&stServerAddress, &ulLenOfAddress);

    setCliSoc(lClientSoc);
    // Check if the server is accepting the signals from the client
    if(lClientSoc < 0)
    {
        blStatus = false;
    }

    return blStatus;
}
