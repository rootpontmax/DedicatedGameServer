////////////////////////////////////////////////////////////////////////////////////////////////////
// Adapter class.                                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "yojimbo.h"
#include "Common.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
class CGameServer;
////////////////////////////////////////////////////////////////////////////////////////////////////
class CAdapter : public yojimbo::Adapter
{
public:
    explicit CAdapter( CGameServer* server );
    
    // Overriden methods of Adapter class
    virtual yojimbo::MessageFactory *CreateMessageFactory( yojimbo::Allocator& allocator ) override;
    virtual void OnServerClientConnected( int clientIndex ) override;
    virtual void OnServerClientDisconnected( int clientIndex ) override;

private:
    CGameServer    *m_pServer;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
