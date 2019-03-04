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

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GetNumBitsForMessage( uint16_t sequence )
{
    static int messageBitsArray[] = { 1, 320, 120, 4, 256, 45, 11, 13, 101, 100, 84, 95, 203, 2, 3, 8, 512, 5, 3, 7, 50 };
    const int modulus = sizeof( messageBitsArray ) / sizeof( int );
    const int index = sequence % modulus;
    return messageBitsArray[index];
}
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TestMessage : public yojimbo::Message
{
    uint16_t sequence;

    TestMessage()
    {
        sequence = 0;
    }

    template <typename Stream> bool Serialize( Stream & stream )
    {        
        serialize_bits( stream, sequence, 16 );

        int numBits = GetNumBitsForMessage( sequence );
        int numWords = numBits / 32;
        uint32_t dummy = 0;
        for ( int i = 0; i < numWords; ++i )
            serialize_bits( stream, dummy, 32 );
        int numRemainderBits = numBits - numWords * 32;
        if ( numRemainderBits > 0 )
            serialize_bits( stream, dummy, numRemainderBits );

        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TestBlockMessage : public yojimbo::BlockMessage
{
    uint16_t sequence;

    TestBlockMessage()
    {
        sequence = 0;
    }

    template <typename Stream> bool Serialize( Stream & stream )
    {        
        serialize_bits( stream, sequence, 16 );
        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TestSerializeFailOnReadMessage : public yojimbo::Message
{
    template <typename Stream> bool Serialize( Stream & /*stream*/ )
    {        
        return !Stream::IsReading;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TestExhaustStreamAllocatorOnReadMessage : public yojimbo::Message
{
    template <typename Stream> bool Serialize( Stream & stream )
    {        
        if ( Stream::IsReading )
        {
            const int NumBuffers = 100;

            void * buffers[NumBuffers];

            memset( buffers, 0, sizeof( buffers ) );

            for ( int i = 0; i < NumBuffers; ++i )
            {
                buffers[i] = YOJIMBO_ALLOCATE( stream.GetAllocator(), 1024 * 1024 );
            }

            for ( int i = 0; i < NumBuffers; ++i )
            {
                YOJIMBO_FREE( stream.GetAllocator(), buffers[i] );
            }
        }

        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};
////////////////////////////////////////////////////////////////////////////////////////////////////
enum TestMessageType
{
    TEST_MESSAGE,
    TEST_BLOCK_MESSAGE,
    TEST_SERIALIZE_FAIL_ON_READ_MESSAGE,
    TEST_EXHAUST_STREAM_ALLOCATOR_ON_READ_MESSAGE,
    NUM_TEST_MESSAGE_TYPES
};
////////////////////////////////////////////////////////////////////////////////////////////////////
YOJIMBO_MESSAGE_FACTORY_START( TestMessageFactory, NUM_TEST_MESSAGE_TYPES );
    YOJIMBO_DECLARE_MESSAGE_TYPE( TEST_MESSAGE, TestMessage );
    YOJIMBO_DECLARE_MESSAGE_TYPE( TEST_BLOCK_MESSAGE, TestBlockMessage );
    YOJIMBO_DECLARE_MESSAGE_TYPE( TEST_SERIALIZE_FAIL_ON_READ_MESSAGE, TestSerializeFailOnReadMessage );
    YOJIMBO_DECLARE_MESSAGE_TYPE( TEST_EXHAUST_STREAM_ALLOCATOR_ON_READ_MESSAGE, TestExhaustStreamAllocatorOnReadMessage );
YOJIMBO_MESSAGE_FACTORY_FINISH();
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestAdapter : public yojimbo::Adapter
{
public:

    yojimbo::MessageFactory * CreateMessageFactory( yojimbo::Allocator & allocator )
    {
        return YOJIMBO_NEW( allocator, TestMessageFactory, allocator );
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////
static TestAdapter adapter;
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
    
    const int serverPort = 40000;
    yojimbo::ClientServerConfig config;
    yojimbo::Client client( yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), config, adapter, time );
    yojimbo::Address serverAddress( "127.0.0.1", serverPort );
    
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
        client.SendPackets();

        client.ReceivePackets();

        if ( client.IsDisconnected() )
            break;
     
        time += deltaTime;

        client.AdvanceTime( time );

        if ( client.ConnectionFailed() )
            break;

        yojimbo_sleep( deltaTime );
    }

    client.Disconnect();

    return 0;

}
////////////////////////////////////////////////////////////////////////////////////////////////////
