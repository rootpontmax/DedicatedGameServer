////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  DedicatedGameServer
//
//  Created by Mikhail Scherbakov on 03/10/2018.
//  Copyright © 2018 sh. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "Server.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, const char * argv[] )
{
    std::cout << "Init Yojimbo...\n";
    InitializeYojimbo();
    
    std::cout << "Init game sever...\n";
    
    const int serverPort = 40000;
    const yojimbo::Address address( "127.0.0.1", serverPort );
    const SConnectionConfig config;
    CGameServer server( address, config );
    
    std::cout << "Start game sever...\n";
    server.Run();
    
    std::cout << "Game server completed\n";
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
