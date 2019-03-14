#include "Adapter.h"
#include "Server.h"
#include "Messages.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CAdapter::CAdapter( CGameServer *pServer ) :
    m_pServer( pServer )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
yojimbo::MessageFactory *CAdapter::CreateMessageFactory( yojimbo::Allocator& allocator )
{
    //return nullptr;
    return YOJIMBO_NEW( allocator, ServerMessageFactory, allocator );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CAdapter::OnServerClientConnected( int clientIndex )
{
    if( m_pServer )
        m_pServer->ClientConnected( clientIndex );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CAdapter::OnServerClientDisconnected( int clientIndex )
{
    if( m_pServer )
        m_pServer->ClientDisconnected( clientIndex );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
