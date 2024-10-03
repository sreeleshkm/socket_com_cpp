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
#include "server.h"

using namespace std;

//****************************** startClientCon *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool ServerCom::serverStartCon(void)
{
    bool blConState = false;
    // Structure to represent the address
    struct sockaddr_in stServerAddress = {0};

    blConState = createSocket();

    if (blConState == true)
    {
        cout << "Socket created\n";

        blConState = setOption();

        if (blConState == true)
        {
            blConState = BindSocket();

            if (blConState == true)
            {
                blConState = listenSocket();

                if (blConState == true)
                {
                    blConState = acceptConnection();

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

bool ServerCom::setOption(void)
{
    bool blStatus = true;
    int32 lSocOptState = 0;
    // Option value for respective option_name
    uint32 ulOptionValue = 1;

    // Set options for the socket
    lSocOptState = setsockopt(glSocketDescriptor, SOL_SOCKET, SO_REUSEADDR,
                    &ulOptionValue, sizeof(ulOptionValue));

    // Check if options are set successfully
    if(lSocOptState < 0)
    {
        blStatus = false;
    }

    return blStatus;
}

bool ServerCom::listenSocket(void)
{
    bool blStatus = true;
    int32 lLisState = 0;

    // Listen on specified port with a maximum of 4 requests
    lLisState = listen(glSocketDescriptor, 4);

    // Check if the socket is listening successfully
    if(lLisState < 0)
    {
        blStatus = false;
    }

    return blStatus;
}

bool ServerCom::acceptConnection(void)
{
    bool blStatus = true;
    uint32 ulLenOfAddress = 0;

    ulLenOfAddress = sizeof(struct sockaddr);

    // Accept connection signals from the client
    glClientSocket = accept(glSocketDescriptor, 
                    (struct sockaddr*)&stServerAddress, &ulLenOfAddress);

    // Check if the socket is listening successfully
    if(glClientSocket < 0)
    {
        blStatus = false;
    }

    return blStatus;
}
