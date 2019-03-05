#include "Server.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };
static const int MAX_PLAYERS = 64;
////////////////////////////////////////////////////////////////////////////////////////////////////
CGameServer::CGameServer( const yojimbo::Address& address, const SConnectionConfig& config ) :
    m_adapter( this ),
    m_server( yojimbo::GetDefaultAllocator(), DEFAULT_PRIVATE_KEY, address, config, m_adapter, 0.0 ),
    m_time( 0.0 ),
    m_bRunning( false )
{
    std::cout << "\tStarting...\n";
    m_server.Start( MAX_PLAYERS );
    
    if( !m_server.IsRunning() )
    {
        throw std::runtime_error("Could not start server at port " + std::to_string(address.GetPort()));
    }
    
    // This is my code
    m_bRunning = true;

    // print the port we got in case we used port 0
    char buffer[256];
    m_server.GetAddress().ToString(buffer, sizeof(buffer));
    std::cout << "\tServer address is " << buffer << std::endl;

    // ... load game ...
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::Run()
{
    float fixedDt = 1.0f / 60.0f;
    while( m_bRunning )
    {
        // Information and control
        m_menu.Input();
        m_menu.Draw();
        if( m_menu.ShouldExit() )
        {
            m_server.Stop();
            break;
        }
        
        double currentTime = yojimbo_time();
        if( m_time <= currentTime )
        {
            Update( fixedDt );
            m_time += fixedDt;
        }
        else
        {
            yojimbo_sleep( m_time - currentTime );
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::ClientConnected( const int clientIndex )
{
    std::cout << "client " << clientIndex << " connected" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::ClientDisconnected( const int clientIndex )
{
    std::cout << "client " << clientIndex << " disconnected" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::Update( double fixedDeltaTime )
{
    // Stop if server is not running
    if( !m_server.IsRunning() )
    {
        m_bRunning = false;
        return;
    }

    // Update server and process messages
    m_server.AdvanceTime(m_time);
    m_server.ReceivePackets();
    ProcessMessages();

    // ... process client inputs ...
    // ... update game ...
    // ... send game state to clients ...

    m_server.SendPackets();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::ProcessMessages()
{
    for( int i = 0; i < MAX_PLAYERS; ++i )
    {
        if( m_server.IsClientConnected( i ) )
        {
            for( int j = 0; j < m_connectionConfig.numChannels; ++j )
            {
                yojimbo::Message *pMessage = m_server.ReceiveMessage( i, j );
                while( pMessage )
                {
                    ProcessMessage( i, pMessage );
                    m_server.ReleaseMessage( i, pMessage );
                    pMessage = m_server.ReceiveMessage( i, j );
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameServer::ProcessMessage( const int clientIndex, const yojimbo::Message* pMessage )
{
    switch( pMessage->GetType() )
    {
        case EGameMessage::GAME_MESSAGE_TEST:
            //ProcessTestMessage(clientIndex, (TestMessage*)message);
            break;
            
        default:
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
