/**
 * @file menuPage_config_angle_max.cpp
 * @author Simon Latour
 * @brief  menuPage_config_angle_max
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_angle_max.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_angle_max::connect (menuManager* mManager, menu* parentMenu)
{
        m_menuManager = mManager;
        m_parentPage = parentMenu;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_angle_max::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_angle_max::handle()
{
    String stringValue;

     if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("Angle maximum");
    
        if(mAngleLimit == 0)
        {
            stringValue = String("Off");
        }
        else
        {
            stringValue = String(mAngleLimit) + String(" deg.") ;
        }
        m_displayManager->widget_valueSelectUpDown(0,MENU_TITLE_BAR_HEIGHT+1, 
                            m_displayManager->screenWidth(), 
                            m_displayManager->screenHeight() - MENU_TITLE_BAR_HEIGHT,
                            stringValue , 
                            (mAngleLimit < ANGLE_MAX_UPPER_LIMIT), 
                            (mAngleLimit > 0),
                            MED_FONT, W_CENTER_ALIGN
                        );
 
        m_displayManager->needRefresh();
    }

    

}

/**
 * @brief Code to execute when use input "Button Up" is detected
 * 
 */
void menuPage_angle_max::onPressUp()
{
    

    if(mAngleLimit < ANGLE_MAX_LOWER_LIMIT) // Was off, turn on at lower limit
    {
        mAngleLimit = ANGLE_MAX_LOWER_LIMIT; 
    }
    else
    {
        // Highlight next Menu
        if(mAngleLimit <= (ANGLE_MAX_UPPER_LIMIT-ANGLE_MAX_INCREMENT))
        {
            mAngleLimit += ANGLE_MAX_INCREMENT;
           
        }
        else
        {
            mAngleLimit = ANGLE_MAX_UPPER_LIMIT;
        }       
    }
    
    configChange = true;
    m_focusFlag = true; //force refresh
}

void menuPage_angle_max::onLongPressUp()
{
    

    if(mAngleLimit < ANGLE_MAX_LOWER_LIMIT) // Was off, turn on at lower limit
    {
        mAngleLimit = ANGLE_MAX_LOWER_LIMIT; 
    }
    else
    {
        // Highlight next Menu
        if(mAngleLimit <= (ANGLE_MAX_UPPER_LIMIT-ANGLE_MAX_INCREMENT_FAST))
        {
            mAngleLimit+= ANGLE_MAX_INCREMENT_FAST;
           
        }
        else
        {
            mAngleLimit = ANGLE_MAX_UPPER_LIMIT;
        }     
    }
    
    configChange = true;
    m_focusFlag = true; //force refresh
}

    
/**
 * @brief Code to execute when use input "Button Down" is detected
 * 
 */
void menuPage_angle_max::onPressDown()
{
    // Highlight next Menu
    if(mAngleLimit > ANGLE_MAX_UPPER_LIMIT)
    {
           mAngleLimit =  ANGLE_MAX_UPPER_LIMIT;
    }
    else
    {
        if(mAngleLimit >= (ANGLE_MAX_LOWER_LIMIT + ANGLE_MAX_INCREMENT))
        {
            mAngleLimit-= ANGLE_MAX_INCREMENT;
            
        }
        else if(mAngleLimit > ANGLE_MAX_LOWER_LIMIT){
            mAngleLimit = ANGLE_MAX_LOWER_LIMIT;
        }
        else
        {
            mAngleLimit = 0;    // 0 = OFF
        }
    }
    this->configChange = true;
    m_focusFlag = true; //force refresh
    
}

void menuPage_angle_max::onLongPressDown()
{
    // Highlight next Menu
    if(mAngleLimit > ANGLE_MAX_UPPER_LIMIT)
    {
           mAngleLimit =  ANGLE_MAX_UPPER_LIMIT;
    }
    else
    {
        if(mAngleLimit >= (ANGLE_MAX_LOWER_LIMIT + ANGLE_MAX_INCREMENT_FAST))
        {
            mAngleLimit-= ANGLE_MAX_INCREMENT_FAST;
            
        }
        else if(mAngleLimit > ANGLE_MAX_LOWER_LIMIT){
            mAngleLimit = ANGLE_MAX_LOWER_LIMIT;
        }
        else
        {
            mAngleLimit = 0;    // 0 = OFF
        }
    }
    this->configChange = true;
    m_focusFlag = true; //force refresh
    
}

/**
 * @brief Code to execute when use input "Button return" is detected
 * 
 */
void menuPage_angle_max::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input "Button Enter" is detected
 * 
 */
void menuPage_angle_max::onPressEnter()
{
    
}

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_angle_max::onFocus()
{
    m_focusFlag = true;
}