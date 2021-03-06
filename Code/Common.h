////////////////////////////////////////////////////////////////////////////////////////////////////
// File for common constants.                                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "yojimbo.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
static const int g_serverPort = 40000;
////////////////////////////////////////////////////////////////////////////////////////////////////
enum EGameChannel
{
    GAME_CHANNEL_RELIABLE,
    GAME_CHANNEL_UNRELIABLE,
    GAME_CHANNEL_COUNT
};
////////////////////////////////////////////////////////////////////////////////////////////////////
enum EMessageType
{
    MESSAGE_TYPE_INPUT,
    MESSAGE_TYPE_STATE_UPDATE,
    MESSAGE_TYPE_COUNT
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
