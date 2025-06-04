/**
 * @file menuPage_config_refPeriod_rep.h
 * @author Simon Latour
 * @brief menuPage_config_refPeriod_rep
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_REFPERIOD_REP_H
#define MENUPAGE_REFPERIOD_REP_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

#define REF_PERIOD_REP_DEFAULT 10
#define REF_PERIOD_REP_MAX 9999
#define REF_PERIOD_REP_MIN 5
#define REF_PERIOD_REP_INCREMENT  1
#define REF_PERIOD_REP_INCREMENT_FAST  10

class menuPage_refPeriod_rep: public menuPage
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

        unsigned int mRepLimit = REF_PERIOD_REP_DEFAULT;
        bool configChange = false;

};




#endif