
/**
 * @file DRMenu_configReminder.cpp
 * @author Simon Latour
 * @brief Reminder config page
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menuPage_alertConfig.h"
#include "readaptLogo.h"
#include "string.h"


/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_alertConfig::connect(menuManager* mManager, menu* parentMenu)
{
    m_menuManager = mManager;
    m_parentPage = parentMenu;

    //Connect sub menues with their parents and the context manager
    // page_Length.connect(mManager,this);
    // page_rep.connect(mManager,this);
    // page_time.connect(mManager,this);
    // page_VibrationsOnOff.connect(mManager,this);
    // page_ToneOnOff.connect(mManager,this);
    // page_RepAlertOnOff.connect(mManager,this);
    // page_TimeAlertOnOff.connect(mManager,this);
    // page_AngleDetectionAlertOnOff.connect(mManager,this);
    // page_AngleMaxAlertOnOff.connect(mManager,this);

    // m_subMenuList[MENUITEM_VIBRATION_ONOFF]  =   &page_VibrationsOnOff;
    // m_subMenuList[MENUITEM_TONE_ONOFF]  =   &page_ToneOnOff;
    // m_subMenuList[MENUITEM_REPALERT_ONOFF]  =   &page_RepAlertOnOff;
    // m_subMenuList[MENUITEM_TIMEALERT_ONOFF]  =   &page_TimeAlertOnOff;
    // m_subMenuList[MENUITEM_ANGLEDETECT_ONOFF]  =   &page_AngleDetectionAlertOnOff;
    // m_subMenuList[MENUITEM_ANGLEMAX_ONOFF]  =   &page_AngleMaxAlertOnOff;
 

    
   
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_alertConfig::connectDisplayManager(oledDisplayManager * displayManager)
{
    m_displayManager = displayManager;
    // page_Length.connectDisplayManager(displayManager);
    // page_rep.connectDisplayManager(displayManager);
    // page_time.connectDisplayManager(displayManager);
    // page_VibrationsOnOff.connectDisplayManager(displayManager);
    // page_ToneOnOff.connectDisplayManager(displayManager);
    // page_RepAlertOnOff.connectDisplayManager(displayManager);
    // page_TimeAlertOnOff.connectDisplayManager(displayManager);
    // page_AngleDetectionAlertOnOff.connectDisplayManager(displayManager);
    // page_AngleMaxAlertOnOff.connectDisplayManager(displayManager);

}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_alertConfig::handle()
{
    if(m_focusFlag )
    {
        m_focusFlag = false;
        
        m_displayManager->display.clearDisplay();
       
        m_displayManager->displaySubMenuLayout("Config Alertes");

        highlightMode_t hlMode = highlightMode_centered;
        //m_displayManager->displayScrollMenu(m_itemSelect, (unsigned char)MENUITEM_COUNT, itemLabelList, true,1,true);
        uint16_t onOffRowWidth = 22;
        uint16_t startPosY = 9+6;
        uint16_t scrollbarWidth = 12;
        uint16_t titlebarHeight= 12;

        updateOnOffLabel();
        uint8_t scrollStatus = m_displayManager->displayList(0, startPosY, 
                              m_displayManager->screenHeight()-titlebarHeight, m_displayManager->screenWidth()-onOffRowWidth-scrollbarWidth,
                              m_itemSelect,
                              (unsigned char)MENUITEM_COUNT, itemLabelList,
                              highlightMode_Invisible ,
                              SMALL_FONT, widgetAlignment::centerLeft,2);

      
         m_displayManager->displayList(m_displayManager->screenWidth()-onOffRowWidth-scrollbarWidth, startPosY, 
                             m_displayManager->screenHeight()-titlebarHeight, onOffRowWidth,
                             m_itemSelect,
                             (unsigned char)MENUITEM_COUNT, onOffLabelList,
                             highlightMode_Highlighted ,
                             SMALL_FONT, widgetAlignment::centerRight,2);
        if(scrollStatus & SCROLL_UP_MASK)
        {     
             m_displayManager->displayScrollUpArrow();
        }
        if(scrollStatus & SCROLL_DOWN_MASK)
        {
            m_displayManager->displayScrollDownArrow();
        }

        m_displayManager->needRefresh();
    }
  
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_alertConfig::onPressUp()
{
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
void menuPage_alertConfig::onPressDown()
{
    // Highlight next Menu
    if(m_itemSelect < (MENUITEM_COUNT))
    {
        m_itemSelect++;
        if(m_itemSelect == MENUITEM_COUNT)
        {
            m_itemSelect = 0; // loop around
        }
        m_focusFlag = true; //force refresh
    }
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_alertConfig::onPressReturn()
{
    m_itemSelect = 0;
    //Set parent page as the active page
    m_menuManager->setActivePage(m_parentPage);

}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_alertConfig::onPressEnter()
{
    //Set the highlighted menu as the active page
    // if(m_subMenuList[m_itemSelect]){
    //     m_menuManager->setActivePage(m_subMenuList[m_itemSelect]);
    // }
    onOffStatusList[m_itemSelect] = 1-onOffStatusList[m_itemSelect];
     m_focusFlag = true; //force refresh
     configChange = 1;
}

/**
 * @brief Call when page gets focus.
 * 
 */
void menuPage_alertConfig::onFocus()
{
    m_focusFlag = true;
    
    m_displayManager->disableAutoSleepMode();
}


void menuPage_alertConfig::updateOnOffLabel()
{

    for(int i = 0; i < MENUITEM_COUNT;i++){

        if (onOffStatusList[i] == 0)
        {
            //onOffLabelList[i] = String("On");
            //strcpy(onOffLabelList[i],"Off");
            onOffLabelList[i] = "Off";
        }
        else
        {
            //strcpy(onOffLabelList[i],"On ");
            onOffLabelList[i] = "On";
        }
    }
}


uint8_t menuPage_alertConfig::getVibrationAlertEnable()
{
    return onOffStatusList[MENUITEM_VIBRATION_ONOFF];
}
uint8_t menuPage_alertConfig::getToneAlertEnable()
{
    return onOffStatusList[MENUITEM_TONE_ONOFF];
}
uint8_t menuPage_alertConfig::getRepetitionAlertEnable()
{
    return onOffStatusList[MENUITEM_REPALERT_ONOFF];
}
uint8_t menuPage_alertConfig::getTimeAlertEnable()
{
    return onOffStatusList[MENUITEM_TIMEALERT_ONOFF];
}
uint8_t menuPage_alertConfig::getAngleDetectAlertEnable()
{
    return onOffStatusList[MENUITEM_ANGLEDETECT_ONOFF];
}
uint8_t menuPage_alertConfig::getAngleMaxAlertEnable()
{
    return onOffStatusList[MENUITEM_ANGLEMAX_ONOFF];
}



void menuPage_alertConfig::setVibrationAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_VIBRATION_ONOFF] = onOff;
    }
void menuPage_alertConfig::setToneAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_TONE_ONOFF]= onOff;
    }
void menuPage_alertConfig::setRepetitionAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_REPALERT_ONOFF] = onOff;
    }
void menuPage_alertConfig::setTimeAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_TIMEALERT_ONOFF] = onOff;
    }
void menuPage_alertConfig::setAngleDetectAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_ANGLEDETECT_ONOFF] = onOff;
    }
void menuPage_alertConfig::setAngleMaxAlertEnable(uint8_t onOff)
{
    onOffStatusList[MENUITEM_ANGLEMAX_ONOFF] = onOff;
    } 