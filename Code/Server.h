////////////////////////////////////////////////////////////////////////////////////////////////////
// Server class.                                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Adapter.h"
#include "yojimbo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
class CGameServer
{
public:
    CGameServer( const yojimbo::Address& address, const SConnectionConfig& config );
    void Run();
    
    void ClientConnected( const int clientIndex );
    void ClientDisconnected( const int clientIndex );
    
private:
    
    void Update( double fixedDeltaTime );
    void ProcessMessages();
    void ProcessMessage( const int clientIndex, const yojimbo::Message* pMessage ); 

    SConnectionConfig   m_connectionConfig;   
    CAdapter            m_adapter;
    yojimbo::Server     m_server;
    double              m_time;
    bool                m_bRunning;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
