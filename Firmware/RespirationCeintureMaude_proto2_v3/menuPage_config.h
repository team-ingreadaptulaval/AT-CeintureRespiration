
/**
 * @file menuPage_config.h
 * @author Simon Latour
 * @brief root config page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_CONFIG_H
    #define MENUPAGE_CONFIG_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

#include "backupData_ModuleErgonomie.h"

//#include "menuPage_refPeriod.h"
//#include "menuPage_day.h"
#include "menuPage_alertConfig.h"
#include "menuPage_config_refPeriod_length.h"
#include "menuPage_config_refPeriod_rep.h"
#include "menuPage_config_refPeriod_time.h"
#include "menuPage_config_angle_detect.h"
#include "menuPage_config_angle_max.h"




class menuPage_config: public menuPage
{
    private: 

        menuManager* m_menuManager;

        enum menuItemList {
                                MENUITEM_ANGLE_DETECT,
                                MENUITEM_ANGLE_MAX,
                                MENUITEM_LENGHT,
                                MENUITEM_REP,
                                MENUITEM_TIME,
                                MENUITEM_ALERTS,
                                MENUITEM_COUNT
                            };
        vector<string> itemLabelList = {
                                "Angle elevation",
                                "Angle max",
                                "Long. periode",
                                "Alerte rep.",
                                "Alerte temps",
                                "Alertes"
                            };

        vector<string> valueLabelList = { 
                                "000",
                                "000",
                                "TBD",
                                "0",
                                "0",
                                "/img/arrowRightSmall"
                                };

        menuPage* m_subMenuList[MENUITEM_COUNT]; 
        int m_itemSelect = 0;

        oledDisplayManager* m_displayManager; 

        void updateValueLabel();

    public:

        void connect(menuManager* context, menu* parentMenu);
        void connectDisplayManager(oledDisplayManager * displayManager);
    
        // Sub Menus
        menuPage_refPeriod_length   page_Length;
        menuPage_refPeriod_rep      page_rep;
        menuPage_refPeriod_time     page_time;
        menuPage_alertConfig    pageAlerts;
        menuPage_angle_detect   page_angle_detect ;
        menuPage_angle_max      page_angle_max ;

        char bluetoothDeviceName[BLUTOOTH_NAME_MAX_SIZE] = DEFAULT_BLUTOOTH_NAME;

        bool configChange = false;
        
        // Menu Functions
        void handle();

        void onPressUp();
        void onPressDown();
        void onPressReturn();
        void onPressEnter();
        void onFocus();

};


#endif