/**
 * @file menuPage_config_angle_detect.h
 * @author simon latour
 * @brief 
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_ANGLE_DETECT_H
#define MENUPAGE_ANGLE_DETECT_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

 #define ANGLE_DETECT_DEFAULT 70
 #define ANGLE_DETECT_UPPER_LIMIT 120
 #define ANGLE_DETECT_LOWER_LIMIT 40
 #define  ANGLE_DETECT_INCREMENT 5


class menuPage_angle_detect: public menuPage
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

        unsigned int mAngleLimit = ANGLE_DETECT_DEFAULT;
        bool configChange = false;

};




#endif