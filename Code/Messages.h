////////////////////////////////////////////////////////////////////////////////////////////////////
// Class for message with player's input.                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "yojimbo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
static const int MAX_PLAYER_INPUT = 8;
static const int MAX_PLAYER_COUNT = 64;
////////////////////////////////////////////////////////////////////////////////////////////////////
struct CInputMessage : yojimbo::Message
{
    uint16_t    sequenceID;
    uint8_t     playerInput[MAX_PLAYER_INPUT];
    
    CInputMessage() :
        sequenceID( 0 )
    {
        for( int i = 0; i < MAX_PLAYER_INPUT; ++i )
            playerInput[i] = 0;
    }
    
    template < typename Stream >
    bool Serialize( Stream& stream )
    {
        serialize_bits( stream, sequenceID, 16 );
        for( int i = 0; i < MAX_PLAYER_INPUT; ++i )
            serialize_bits( stream, playerInput[i], 8 );
        return true;
    }
    
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
struct CStateUpdateMessage : public yojimbo::Message
{
    float posX[MAX_PLAYER_COUNT];
    
    template < typename Stream >
    bool Serialize( Stream& stream )
    {
        for( int i = 0; i < MAX_PLAYER_COUNT; ++i )
            serialize_bits( stream, posX[i], 32 );
        return true;
    }
    
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
enum EMessageType
{
    MESSAGE_TYPE_INPUT,
    MESSAGE_TYPE_STATE_UPDATE,
    MESSAGE_TYPE_COUNT
};
////////////////////////////////////////////////////////////////////////////////////////////////////
YOJIMBO_MESSAGE_FACTORY_START( ClientMessageFactory, MESSAGE_TYPE_COUNT );
    YOJIMBO_DECLARE_MESSAGE_TYPE( MESSAGE_TYPE_INPUT, CInputMessage );
YOJIMBO_MESSAGE_FACTORY_FINISH();
////////////////////////////////////////////////////////////////////////////////////////////////////
YOJIMBO_MESSAGE_FACTORY_START( ServerMessageFactory, MESSAGE_TYPE_COUNT );
    YOJIMBO_DECLARE_MESSAGE_TYPE( MESSAGE_TYPE_STATE_UPDATE, CStateUpdateMessage );
YOJIMBO_MESSAGE_FACTORY_FINISH();
////////////////////////////////////////////////////////////////////////////////////////////////////
