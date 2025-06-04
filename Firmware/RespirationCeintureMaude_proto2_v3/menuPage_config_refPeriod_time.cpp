/**
 * @file DRMenu_configLight.cpp
 * @author Simon Latour
 * @brief  Light sensoir config page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_refPeriod_time.h"
#include "teamATbasic_V1_1.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_refPeriod_time::connect (menuManager* mManager, menu* parentMenu)
{
        m_menuManager = mManager;
        m_parentPage = parentMenu;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_refPeriod_time::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_refPeriod_time::handle()
{


     if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("Alerte de Temps");

        String displayData;
        if(mTimeTargetMilliseconds < REF_PERIOD_TIME_MIN)
        {
            displayData = String("Off");
        }
        else
        {
            char timeString[8];
            millisToHumanTime(mTimeTargetMilliseconds,timeString,8);
            displayData = String(timeString) + String(" min.");
        }
    
        m_displayManager->widget_valueSelectUpDown(0,MENU_TITLE_BAR_HEIGHT+1, 
                            m_displayManager->screenWidth()/*- MENU_SCROLLBAR_WIDTH*/, 
                            m_displayManager->screenHeight() - MENU_TITLE_BAR_HEIGHT,
                            displayData, 
                            (mTimeTargetMilliseconds < REF_PERIOD_TIME_MAX), 
                            (mTimeTargetMilliseconds > REF_PERIOD_TIME_MIN),
                            MED_FONT, W_CENTER_ALIGN
                        );
 
        m_displayManager->needRefresh();
    }

    

}

/**
 * @brief Code to execute when use input "Button Up" is detected
 * 
 */
void menuPage_refPeriod_time::onPressUp()
{

    if(mTimeTargetMilliseconds < REF_PERIOD_TIME_MIN) // On/off mechanism
    {
        mTimeTargetMilliseconds = REF_PERIOD_TIME_MIN;
    }
    else
    {
        if(mTimeTargetMilliseconds < REF_PERIOD_TIME_MAX )
        {
            mTimeTargetMilliseconds+= REF_PERIOD_INCREMENT;

            if(mTimeTargetMilliseconds > REF_PERIOD_TIME_MAX)
            {
                mTimeTargetMilliseconds = REF_PERIOD_TIME_MAX;
            }  
        }    
    }

    configChange = true;
    m_focusFlag = true; //force refresh
     
}

void menuPage_refPeriod_time::onLongPressUp()
{

    if(mTimeTargetMilliseconds < REF_PERIOD_TIME_MIN) // On/off mechanism
    {
        mTimeTargetMilliseconds = REF_PERIOD_TIME_MIN;
    }
    else
    {
        if(mTimeTargetMilliseconds < REF_PERIOD_TIME_MAX )
        {
            mTimeTargetMilliseconds+= REF_PERIOD_INCREMENT_FAST;

            if(mTimeTargetMilliseconds > REF_PERIOD_TIME_MAX)
            {
                mTimeTargetMilliseconds = REF_PERIOD_TIME_MAX;
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
void menuPage_refPeriod_time::onPressDown()
{
    // Highlight next Menu
    if(mTimeTargetMilliseconds >=  (REF_PERIOD_TIME_MIN + REF_PERIOD_INCREMENT ))
    {
        mTimeTargetMilliseconds-=REF_PERIOD_INCREMENT;
    }
    else if(mTimeTargetMilliseconds > REF_PERIOD_TIME_MIN)
    {
        mTimeTargetMilliseconds = REF_PERIOD_TIME_MIN;
    }
    else
    {
        mTimeTargetMilliseconds = 0; // OFF
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh

}

void menuPage_refPeriod_time::onLongPressDown()
{
    // Highlight next Menu
    if(mTimeTargetMilliseconds >= (REF_PERIOD_TIME_MIN + REF_PERIOD_INCREMENT_FAST))
    {
        mTimeTargetMilliseconds-=REF_PERIOD_INCREMENT_FAST;
    }
    else if(mTimeTargetMilliseconds > REF_PERIOD_TIME_MIN)
    {
        mTimeTargetMilliseconds = REF_PERIOD_TIME_MIN;
    }
    else
    {
        mTimeTargetMilliseconds = 0; // OFF
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh

}

/**
 * @brief Code to execute when use input "Button return" is detected
 * 
 */
void menuPage_refPeriod_time::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input "Button Enter" is detected
 * 
 */
void menuPage_refPeriod_time::onPressEnter()
{
    
}

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_refPeriod_time::onFocus()
{
    m_focusFlag = true;
}