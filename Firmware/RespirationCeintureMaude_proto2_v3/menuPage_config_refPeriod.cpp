
/**
 * @file menuPage_config_refPeriod.cpp
 * @author Simon Latour
 * @brief menuPage_config_refPeriod
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_refPeriod.h"
#include "readaptLogo.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_refPeriod::connect(menuManager* mManager, menu* parentMenu)
{
    m_menuManager = mManager;
    m_parentPage = parentMenu;

    //Connect sub menues with their parents and the context manager
    page_Length.connect(mManager,this);
    page_rep.connect(mManager,this);
    page_time.connect(mManager,this);

    m_subMenuList[MENUITEM_LENGHT]  = &page_Length;  
    m_subMenuList[MENUITEM_REP]  = &page_rep; 
    m_subMenuList[MENUITEM_TIME]  = &page_time; 
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_refPeriod::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
    page_Length.connectDisplayManager(displayManager);
    page_rep.connectDisplayManager(displayManager);
    page_time.connectDisplayManager(displayManager);

}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_refPeriod::handle()
{
    if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
       
        m_displayManager->displaySubMenuLayout("Config Periode");
        m_displayManager->displayScrollMenu(m_itemSelect, (unsigned char)MENUITEM_COUNT, itemLabelList, true,1,true);

        m_displayManager->needRefresh();
    }
  
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_refPeriod::onPressUp()
{
    // Highlight next Menu
    if(m_itemSelect > 0 )
    {
        m_itemSelect--;
        m_focusFlag = true; //force refresh
    }       
}
    
    /**
 * @brief Code to execute when use input is detected
 */
void menuPage_refPeriod::onPressDown()
{
    // Highlight next Menu
    if(m_itemSelect < (MENUITEM_COUNT -1))
    {
        m_itemSelect++;
        m_focusFlag = true; //force refresh
    }
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_refPeriod::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);

}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_refPeriod::onPressEnter()
{
    //Set the highlighted menu as the active page
    if(m_subMenuList[m_itemSelect]){
        m_menuManager->setActivePage(m_subMenuList[m_itemSelect]);
    }
    
    
}

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_refPeriod::onFocus()
{
    m_focusFlag = true;
    m_displayManager->disableAutoSleepMode();
}
