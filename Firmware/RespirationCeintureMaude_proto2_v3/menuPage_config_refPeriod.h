/**
 * @file menuPage_config_refPeriod.h
 * @author Simon Latour
 * @brief  menuPage_config_refPeriod
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MENUPAGE_REFPERIOD_H
#define MENUPAGE_REFPERIOD_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

#include "menuPage_config_refPeriod_length.h"
#include "menuPage_config_refPeriod_rep.h"
#include "menuPage_config_refPeriod_time.h"


class menuPage_refPeriod: public menuPage
{
    private: 

        menuManager* m_menuManager;

        enum menuItemList {
                                MENUITEM_LENGHT,
                                MENUITEM_REP,
                                MENUITEM_TIME,
                                MENUITEM_COUNT
                            };
        const char* itemLabelList[MENUITEM_COUNT] = {
                                "Longueur periode",
                                "Repetitions max",
                                "Temps max"
                            };

        menuPage* m_subMenuList[MENUITEM_COUNT]; 
        int m_itemSelect = 0;
 
        oledDisplayManager* m_displayManager; 

    public:

        void connect(menuManager* context, menu* parentMenu);
        void connectDisplayManager(oledDisplayManager * displayManager);
    
        // Sub Menus
        menuPage_refPeriod_length   page_Length;
        menuPage_refPeriod_rep      page_rep;
        menuPage_refPeriod_time     page_time;

        // Menu Functions
        void handle();

        void onPressUp();
        void onPressDown();
        void onPressReturn();
        void onPressEnter();
        void onFocus();

};




#endif