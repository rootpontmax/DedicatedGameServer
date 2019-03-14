#include "Menu.h"
#include "rlutil.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CMenu::CMenu() :
    m_bShouldRedraw( true ),
    m_bShouldExit( false )
{
    rlutil::saveDefaultColor();
    m_log.reserve( 10000 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::Input()
{
    if( !kbhit() )
        return;
        
    const int keyIndex = rlutil::getkey();
    switch( keyIndex )
    {
        case rlutil::KEY_LEFT:
            break;
            
        case rlutil::KEY_RIGHT:
            break;
            
        case rlutil::KEY_UP:
            break;
            
        case rlutil::KEY_DOWN:
            break;
            
        case rlutil::KEY_ESCAPE:
            m_bShouldExit = true;
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::Draw()
{
//    if( !m_bShouldRedraw )
//        return;
        
    rlutil::cls();
    //rlutil::hidecursor();
    
    DrawMenu();
    
    //std::cout << "Press ESC to exit...";
    
    //rlutil::hidecursor();
    
    //rlutil::locate(16,6); std::cout << "(16,6)";
    //rlutil::locate(4,3); std::cout << "(4,3)";
    //rlutil::locate(8,8); std::cout << "(8,8)";
    
    
    //qrlutil::showcursor();
    
    m_bShouldRedraw = false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::AddLogString( const char *pString )
{
    m_log.push_back( pString );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::DrawMenu()
{
    
    rlutil::locate( 1, 1 ); std::cout << "Server main menu";
    
    rlutil::locate( 1, 5 ); std::cout << "Log";
    rlutil::locate( 1, 6 ); std::cout << "Exit";
    //rlutil::showcursor();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::DrawLog()
{
    rlutil::cls();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
