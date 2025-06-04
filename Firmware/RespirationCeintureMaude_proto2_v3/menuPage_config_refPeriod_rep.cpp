/**
 * @file menuPage_config_refPeriod_rep.h
 * @author Simon Latour
 * @brief  menuPage_config_refPeriod_rep
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_refPeriod_rep.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_refPeriod_rep::connect (menuManager* mManager, menu* parentMenu)
{
        m_menuManager = mManager;
        m_parentPage = parentMenu;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_refPeriod_rep::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_refPeriod_rep::handle()
{
    String stringValue;

     if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("Alerte repetition");
    
        
        if(mRepLimit == 0)
        {
            stringValue = String("Off");
        }
        else
        {
            stringValue = String(mRepLimit);
        }

        m_displayManager->widget_valueSelectUpDown(0,MENU_TITLE_BAR_HEIGHT+1, 
                            m_displayManager->screenWidth()/*- MENU_SCROLLBAR_WIDTH*/, 
                            m_displayManager->screenHeight() - MENU_TITLE_BAR_HEIGHT,
                            stringValue, 
                            (mRepLimit < REF_PERIOD_REP_MAX), 
                            (mRepLimit > REF_PERIOD_REP_MIN),
                            MED_FONT, W_CENTER_ALIGN
                        );
 
        m_displayManager->needRefresh();
    }

    

}

/**
 * @brief Code to execute when use input "Button Up" is detected
 * 
 */
void menuPage_refPeriod_rep::onPressUp()
{

    if(mRepLimit < REF_PERIOD_REP_MIN) // on/off mechanism
    {
        mRepLimit = REF_PERIOD_REP_MIN;
    }
    else
    {
        if(mRepLimit < REF_PERIOD_REP_MAX)
        {
            mRepLimit+= REF_PERIOD_REP_INCREMENT;
            if(mRepLimit > REF_PERIOD_REP_MAX)
            {
                mRepLimit = REF_PERIOD_REP_MAX;
            }
        }  
    }

    configChange = true;
    m_focusFlag = true; //force refresh
}

void menuPage_refPeriod_rep::onLongPressUp()
{

    if(mRepLimit < REF_PERIOD_REP_MIN) // on/off mechanism
    {
        mRepLimit = REF_PERIOD_REP_MIN;
    }
    else
    {
        if(mRepLimit < REF_PERIOD_REP_MAX)
        {
            mRepLimit+= REF_PERIOD_REP_INCREMENT_FAST;
            if(mRepLimit > REF_PERIOD_REP_MAX)
            {
                mRepLimit = REF_PERIOD_REP_MAX;
            }
        }  
    }

    configChange = true;
    m_focusFlag = true; //force refresh
}

    
/**
 * @brief Code to execute when use input "Button Down" is detected
 * 
 */
void menuPage_refPeriod_rep::onPressDown()
{
    if(mRepLimit >= (REF_PERIOD_REP_MIN +REF_PERIOD_REP_INCREMENT))
    {
        mRepLimit-=REF_PERIOD_REP_INCREMENT;
    }
    else if(mRepLimit > REF_PERIOD_REP_MIN)
    {
       mRepLimit = REF_PERIOD_REP_MIN;
    }
    else
    {
        mRepLimit = 0; // if already at min, turn feature off
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}

void menuPage_refPeriod_rep::onLongPressDown()
{
    if(mRepLimit >= (REF_PERIOD_REP_MIN + REF_PERIOD_REP_INCREMENT_FAST))
    {
        mRepLimit-=REF_PERIOD_REP_INCREMENT_FAST;
    }
    else if(mRepLimit > REF_PERIOD_REP_MIN)
    {
       mRepLimit = REF_PERIOD_REP_MIN;
    }
    else
    {
        mRepLimit = 0; // if already at min, turn feature off
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}

/**
 * @brief Code to execute when use input "Button return" is detected
 * 
 */
void menuPage_refPeriod_rep::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input "Button Enter" is detected
 * 
 */
void menuPage_refPeriod_rep::onPressEnter()
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
void menuPage_refPeriod_rep::onFocus()
{
    m_focusFlag = true;
}