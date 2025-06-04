/**
 * @file DRMENU_config_configLight.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_REFPERIOD_TIME_H
#define MENUPAGE_REFPERIOD_TIME_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

#define REF_PERIOD_TIME_DEFAULT     (5 *60 *1000)
#define REF_PERIOD_TIME_MAX         (120 * 60 *1000)
#define REF_PERIOD_TIME_MIN         (1 * 60 *1000)
#define REF_PERIOD_INCREMENT        (1 * 60 *1000)
#define REF_PERIOD_INCREMENT_FAST   ( 5 * REF_PERIOD_INCREMENT)

class menuPage_refPeriod_time: public menuPage
{
    private: 

        menuManager* m_menuManager;

        enum menuItemList{
            MENUITEM_COUNT          // No sub menu in this page       
        };
        
        oledDisplayManager* m_displayManager; 

    public:

        void connect(menuManager* context, menu* parentMenu);
        void connectDisplayManager(oledDisplayManager * displayManager);
     
        void handle();

        void onPressUp();
        void onLongPressUp();
        void onPressDown();
        void onLongPressDown();
        void onPressReturn();
        void onPressEnter();
        void onFocus();

        unsigned int mTimeTargetMilliseconds = REF_PERIOD_TIME_DEFAULT;
        bool configChange = false;

};




#endif