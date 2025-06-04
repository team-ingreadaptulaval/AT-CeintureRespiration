/**
 * @file menuPage_config_refPeriod_length.cpp
 * @author Simon Latour
 * @brief  menuPage_config_refPeriod_length
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_refPeriod_length.h"
#include "teamATbasic_V1_1.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_refPeriod_length::connect (menuManager* mManager, menu* parentMenu)
{
        m_menuManager = mManager;
        m_parentPage = parentMenu;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_refPeriod_length::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_refPeriod_length::handle()
{


     if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("Longueur periode");

        char timeString[8];
        millisToHumanTime(mPeriodLength,timeString,8);
    
        m_displayManager->widget_valueSelectUpDown(0,MENU_TITLE_BAR_HEIGHT+1, 
                            m_displayManager->screenWidth(), 
                            m_displayManager->screenHeight() - MENU_TITLE_BAR_HEIGHT,
                            String(timeString) + String(" min."), 
                            (mPeriodLength < REF_PERIOD_LENGHT_MAX), 
                            (mPeriodLength > REF_PERIOD_LENGHT_MIN),
                            MED_FONT, W_CENTER_ALIGN
                        );
 
        m_displayManager->needRefresh();
    }

    

}

/**
 * @brief Code to execute when use input "Button Up" is detected
 * 
 */
void menuPage_refPeriod_length::onPressUp()
{
    // Highlight next Menu
    if(mPeriodLength < (REF_PERIOD_LENGHT_MAX - REF_PERIOD_INCREMENT ))
    {
        mPeriodLength+= REF_PERIOD_INCREMENT;
    }
    else
    {
        mPeriodLength = REF_PERIOD_LENGHT_MAX;
    } 

    configChange = true;
    m_focusFlag = true; //force refresh    
}

void menuPage_refPeriod_length::onLongPressUp()
{
    // Highlight next Menu
    if(mPeriodLength < (REF_PERIOD_LENGHT_MAX - REF_PERIOD_INCREMENT_FAST ))
    {
        mPeriodLength+= REF_PERIOD_INCREMENT_FAST;    
    }
    else
    {
        mPeriodLength = REF_PERIOD_LENGHT_MAX;
    }     

    configChange = true;
    m_focusFlag = true; //force refresh
}

    
/**
 * @brief Code to execute when use input "Button Down" is detected
 * 
 */
void menuPage_refPeriod_length::onPressDown()
{
    // Highlight next Menu
    if(mPeriodLength > (REF_PERIOD_LENGHT_MIN + REF_PERIOD_INCREMENT))
    {
        mPeriodLength -= REF_PERIOD_INCREMENT;
    }
    else
    {
        mPeriodLength = REF_PERIOD_LENGHT_MIN;
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}

/**
 * @brief Code to execute when use input "Button Down" is detected
 * 
 */
void menuPage_refPeriod_length::onLongPressDown()
{
    // Highlight next Menu
    if(mPeriodLength > (REF_PERIOD_LENGHT_MIN + REF_PERIOD_INCREMENT_FAST))
    {
        mPeriodLength -= REF_PERIOD_INCREMENT_FAST;
    }
    else
    {
        mPeriodLength = REF_PERIOD_LENGHT_MIN;
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}

/**
 * @brief Code to execute when use input "Button return" is detected
 * 
 */
void menuPage_refPeriod_length::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input "Button Enter" is detected
 * 
 */
void menuPage_refPeriod_length::onPressEnter()
{
    
}


// /**
//  * @brief Returns the ID of the selected item in the menu
//  * 
//  * @return int id numer
//  */
// int     menuPage_refPeriod_length::getSelectedItemID()
// {
//     return lightIntensitySetting;
// }


// /**
//  * @brief This function give the selected item label name
//  * 
//  * @param itemLabel buffer to copy the name
//  */
// void    menuPage_refPeriod_length::getSelectedItemLabel(char* itemLabel)
// {
//     strcpy(itemLabel,itemLabelList[lightIntensitySetting]);
// }

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_refPeriod_length::onFocus()
{
    m_focusFlag = true;
}