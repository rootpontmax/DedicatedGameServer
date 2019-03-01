////////////////////////////////////////////////////////////////////////////////////////////////////
// Adapter class.                                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "yojimbo.h"

// two channels, one for each type that Yojimbo supports
////////////////////////////////////////////////////////////////////////////////////////////////////
enum EGameChannel
{
    GAME_CHANNEL_RELIABLE,
    GAME_CHANNEL_UNRELIABLE,
    GAME_CHANNEL_COUNT
};
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SConnectionConfig : yojimbo::ClientServerConfig
{
    SConnectionConfig()
    {
        numChannels = 2;
        channel[EGameChannel::GAME_CHANNEL_RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
        channel[EGameChannel::GAME_CHANNEL_UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
    }
};
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
