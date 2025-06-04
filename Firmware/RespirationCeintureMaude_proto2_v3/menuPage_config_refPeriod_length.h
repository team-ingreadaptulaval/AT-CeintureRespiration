/**
 * @file menuPage_config_refPeriod_length.h
 * @author Simon Latour
 * @brief menuPage_config_refPeriod_length
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_REFPERIOD_LENGTH_H
#define MENUPAGE_REFPERIOD_LENGTH_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"


#define REF_PERIOD_LENGHT_MAX (120 * 60 * 1000)
#define REF_PERIOD_LENGHT_MIN (5*60*1000)
#define REF_PERIOD_DEFAULT (10 *60 *1000)
#define REF_PERIOD_INCREMENT  (5 * 60 *1000)
#define REF_PERIOD_INCREMENT_FAST  (REF_PERIOD_INCREMENT * 4)

class menuPage_refPeriod_length: public menuPage
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

        //int  getSelectedItemID();
        //void getSelectedItemLabel(char* itemLabel);

        unsigned int mPeriodLength = REF_PERIOD_DEFAULT;
        bool configChange = false;

};




#endif