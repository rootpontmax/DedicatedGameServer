#include "Menu.h"
#include "rlutil.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CMenu::CMenu() :
    m_bShouldRedraw( true )
    m_bShouldExit( false )
{
    rlutil::saveDefaultColor();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::Input()
{
    if( !kbhit() )
        return;
        
    const char k = getch();
    if( k == 'q' )
        m_bShouldExit = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CMenu::Draw()
{
    if( !m_bShouldRedraw )
        return;
        
    rlutil::cls();
    std::cout << "Press Q to exit...";
    
    //rlutil::hidecursor();
    
    //rlutil::locate(16,6); std::cout << "(16,6)";
    //rlutil::locate(4,3); std::cout << "(4,3)";
    //rlutil::locate(8,8); std::cout << "(8,8)";
    
    
    //qrlutil::showcursor();
    
    m_bShouldRedraw = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
