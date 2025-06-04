/**
 * @file menuPage_config.cpp
 * @author Simon Latour
 * @brief root config page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config.h"
//#include "readaptLogo.h"
#include "teamATbasic_V1_1.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_config::connect(menuManager* mManager, menu* parentMenu)
{
    m_menuManager = mManager;
    m_parentPage = parentMenu;

    //Connect sub menues with their parents and the context manager


    pageAlerts.connect(mManager,this);
    page_angle_detect.connect(mManager,this);
    page_angle_max.connect(mManager,this);
    page_Length.connect(mManager,this);
    page_rep.connect(mManager,this);
    page_time.connect(mManager,this);


    m_subMenuList[MENUITEM_ALERTS] = &pageAlerts;
    m_subMenuList[MENUITEM_ANGLE_DETECT]  = &page_angle_detect; 
    m_subMenuList[MENUITEM_ANGLE_MAX]  = &page_angle_max;
    m_subMenuList[MENUITEM_LENGHT]  = &page_Length;  
    m_subMenuList[MENUITEM_REP]  = &page_rep; 
    m_subMenuList[MENUITEM_TIME]  = &page_time;  
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_config::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;

    pageAlerts.connectDisplayManager(displayManager);
    page_angle_detect.connectDisplayManager(displayManager);
    page_angle_max.connectDisplayManager(displayManager);
    page_Length.connectDisplayManager(displayManager);
    page_rep.connectDisplayManager(displayManager);
    page_time.connectDisplayManager(displayManager);
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_config::handle()
{
    uint16_t valueSectionWidth = 5 * 5;
    uint16_t mainSectionY = MENU_TITLE_BAR_HEIGHT + 1; // 12;

    uint16_t scrollbarWidth = 12;


    if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("   Configuration");
        
       // m_displayManager->displayScrollMenu(m_itemSelect, (unsigned char)MENUITEM_COUNT, itemLabelList, true,1,true);

        updateValueLabel();

        uint8_t scrollStatus = m_displayManager->displayList(0,mainSectionY,
                                        m_displayManager->screenHeight()-mainSectionY-1,m_displayManager->screenWidth()-valueSectionWidth-scrollbarWidth,
                                        m_itemSelect,(unsigned char)MENUITEM_COUNT,itemLabelList,highlightMode_t::highlightMode_off);
        
        m_displayManager->displayList(m_displayManager->screenWidth()-valueSectionWidth-scrollbarWidth,mainSectionY,
                                        m_displayManager->screenHeight()-mainSectionY-1,valueSectionWidth,
                                        m_itemSelect,(unsigned char)MENUITEM_COUNT,valueLabelList,highlightMode_t::highlightMode_Highlighted,1,widgetAlignment::centerRight,0);

        m_displayManager->widget_scrollBarVertical(m_displayManager->screenWidth()-scrollbarWidth,
                                        mainSectionY, scrollbarWidth, 
                                        m_displayManager->screenHeight()-mainSectionY-1,
                                        scrollStatus & SCROLL_UP_MASK, scrollStatus & SCROLL_DOWN_MASK,
                                        borderStyle::left);        
                                
    
        m_displayManager->needRefresh();
    }
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_config::onPressUp()
{
    // fonction up désactivée
    // // Highlight next Menu
    // if(m_itemSelect > 0 )
    // {
    //     m_itemSelect--;
    //     m_focusFlag = true; //force refresh
    // }       
}
    
/**
 * @brief Code to execute when use input is detected
 */
void menuPage_config::onPressDown()
{
    // Highlight next Menu
    if(m_itemSelect < (MENUITEM_COUNT -1))
    {
        m_itemSelect++;
    }
    else
    {
        m_itemSelect = 0;
    }
    m_focusFlag = true; //force refresh
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_config::onPressReturn()
{
    m_itemSelect = 0;
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_config::onPressEnter()
{
    //Set the highlighted menu as the active page
    if(m_subMenuList[m_itemSelect])
    {
        m_menuManager->setActivePage(m_subMenuList[m_itemSelect]);
    }
    
}


void menuPage_config::onFocus()
{
    m_focusFlag = true;
    m_displayManager->disableAutoSleepMode();
}


void menuPage_config::updateValueLabel()
{    
    if(menuPage_config::page_angle_detect.mAngleLimit > 0 )
    {
        // char timeString[8];
        valueLabelList[MENUITEM_ANGLE_DETECT] = String(page_angle_detect.mAngleLimit).c_str();
    }
    else
    {
        valueLabelList[MENUITEM_ANGLE_DETECT] = "Off";
    }

    if(menuPage_config::page_angle_max.mAngleLimit > 0 )
    {
        // char timeString[8];
        valueLabelList[MENUITEM_ANGLE_MAX] = String(page_angle_max.mAngleLimit).c_str();
    }
    else
    {
        valueLabelList[MENUITEM_ANGLE_MAX] = "Off";
    }

    if(menuPage_config::page_rep.mRepLimit > 0 )
    {
        valueLabelList[MENUITEM_REP] = String(menuPage_config::page_rep.mRepLimit).c_str();
    }
    else
    {
        valueLabelList[MENUITEM_REP] = "Off";
    }

    if(menuPage_config::page_time.mTimeTargetMilliseconds > 0 )
    {
        char timeString[8];
        millisToHumanTime(page_time.mTimeTargetMilliseconds,timeString,8);
        valueLabelList[MENUITEM_TIME] = String(timeString).c_str();
    }
    else
    {
        valueLabelList[MENUITEM_TIME] = "Off";
    }

    if(page_Length.mPeriodLength > 0 )
    {
        char timeString[8];
        millisToHumanTime(page_Length.mPeriodLength,timeString,8);

        valueLabelList[MENUITEM_LENGHT] = String(timeString).c_str();
    }
    else
    {
        valueLabelList[MENUITEM_LENGHT] = "Off";
    }



}