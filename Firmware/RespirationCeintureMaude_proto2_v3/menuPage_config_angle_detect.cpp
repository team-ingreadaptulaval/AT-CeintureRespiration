/**
 * @file menuPage_config_angle_detect.cpp
 * @author Simon Latour
 * @brief  menu page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_config_angle_detect.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_angle_detect::connect (menuManager* mManager, menu* parentMenu)
{
        m_menuManager = mManager;
        m_parentPage = parentMenu;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_angle_detect::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_angle_detect::handle()
{


     if(m_focusFlag )
    {
        m_focusFlag = false;

        m_displayManager->display.clearDisplay();
        m_displayManager->displaySubMenuLayout("Angle elevation");
    
        m_displayManager->widget_valueSelectUpDown(0,MENU_TITLE_BAR_HEIGHT+1, 
                            m_displayManager->screenWidth(), 
                            m_displayManager->screenHeight() - MENU_TITLE_BAR_HEIGHT,
                            String(mAngleLimit).c_str() + String(" deg."), 
                            (mAngleLimit < ANGLE_DETECT_UPPER_LIMIT), 
                            (mAngleLimit > ANGLE_DETECT_LOWER_LIMIT),
                            MED_FONT, widgetAlignment::center
                        );
 
        m_displayManager->needRefresh();
    }

    

}

/**
 * @brief Code to execute when use input "Button Up" is detected
 * 
 */
void menuPage_angle_detect::onPressUp()
{
    // Highlight next Menu
    if(mAngleLimit <= (ANGLE_DETECT_UPPER_LIMIT -ANGLE_DETECT_INCREMENT))
    {
        mAngleLimit+= ANGLE_DETECT_INCREMENT;
       
    }
    else
    {
        mAngleLimit = ANGLE_DETECT_UPPER_LIMIT; //Failsafe
    }     

    configChange = true;
    m_focusFlag = true; //force refresh
}

void menuPage_angle_detect::onLongPressUp()
{
    // Highlight next Menu
    if(mAngleLimit <= (ANGLE_DETECT_UPPER_LIMIT -(ANGLE_DETECT_INCREMENT*4)))
    {
        mAngleLimit+= (ANGLE_DETECT_INCREMENT*4);
       
    }
    else
    {
        mAngleLimit = ANGLE_DETECT_UPPER_LIMIT; //Failsafe
    }     

    configChange = true;
    m_focusFlag = true; //force refresh
}
    
/**
 * @brief Code to execute when use input "Button Down" is detected
 * 
 */
void menuPage_angle_detect::onPressDown()
{
    // Highlight next Menu
    if(mAngleLimit >= (ANGLE_DETECT_LOWER_LIMIT+ANGLE_DETECT_INCREMENT))
    {
        mAngleLimit-= ANGLE_DETECT_INCREMENT;
       
    }
    else
    {
        mAngleLimit = ANGLE_DETECT_LOWER_LIMIT; //Failsafe
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}

void menuPage_angle_detect::onLongPressDown()
{
    // Highlight next Menu
    if(mAngleLimit >= (ANGLE_DETECT_LOWER_LIMIT+(ANGLE_DETECT_INCREMENT*4)))
    {
        mAngleLimit-= (ANGLE_DETECT_INCREMENT*4);
       
    }
    else
    {
        mAngleLimit = ANGLE_DETECT_LOWER_LIMIT; //Failsafe
    }

    this->configChange = true;
    m_focusFlag = true; //force refresh
}


/**
 * @brief Code to execute when use input "Button return" is detected
 * 
 */
void menuPage_angle_detect::onPressReturn()
{
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);
}

/**
 * @brief Code to execute when use input "Button Enter" is detected
 * 
 */
void menuPage_angle_detect::onPressEnter()
{
    
}

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_angle_detect::onFocus()
{
    m_focusFlag = true;
}
