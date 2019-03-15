////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  DedicatedGameServer
//  Client game class
//
//  Created by Mikhail Scherbakov on 03/10/2018.
//  Copyright © 2018 sh. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "yojimbo.h"
#include "Messages.h"
#include "Common.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
class ClientAdapter : public yojimbo::Adapter
{
public:

    yojimbo::MessageFactory * CreateMessageFactory( yojimbo::Allocator & allocator )
    {
        return YOJIMBO_NEW( allocator, CommonMessageFactory, allocator );
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////
static ClientAdapter clientAdapter;
static volatile int quit = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////
void interrupt_handler( int /*dummy*/ )
{
    quit = 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
    std::cout << "Init Yojimbo...\n";
    InitializeYojimbo();
    
    std::cout << "Сlient init" << std::endl;
    
    double time = 100.0;
    
    uint64_t clientId = 0;
    yojimbo::random_bytes( (uint8_t*) &clientId, 8 );
    printf( "client id is %.16" PRIx64 "\n", clientId );
    
    
    SConnectionConfig config;
    yojimbo::Client client( yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), config, clientAdapter, time );
    yojimbo::Address serverAddress( "127.0.0.1", g_serverPort );
    
    uint8_t privateKey[yojimbo::KeyBytes];
    memset( privateKey, 0, yojimbo::KeyBytes );

    client.InsecureConnect( privateKey, clientId, serverAddress );

    char addressString[256];
    client.GetAddress().ToString( addressString, sizeof( addressString ) );
    printf( "client address is %s\n", addressString );
    
    
    
    const double deltaTime = 0.01f;

    signal( SIGINT, interrupt_handler );

    while ( !quit )
    {
        // Create messages and send it to the server if we have connection
        const yojimbo::ClientState clientState = client.GetClientState();
        if( clientState == yojimbo::CLIENT_STATE_CONNECTED )
        {
            yojimbo::Message *pMessage = client.CreateMessage( MESSAGE_TYPE_INPUT );
            client.SendMessage( GAME_CHANNEL_UNRELIABLE, pMessage );
            //client.ReleaseMessage( pMessage );
        }
        
        // Client network code
        client.SendPackets();
        client.ReceivePackets();

        if ( client.IsDisconnected() )
        {
            printf( "Client disconnected\n" );
            break;
        }
     
        time += deltaTime;

        client.AdvanceTime( time );

        if ( client.ConnectionFailed() )
        {
            printf( "Connection failed\n" );
            break;
        }

        yojimbo_sleep( deltaTime );
    }

    client.Disconnect();

    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
