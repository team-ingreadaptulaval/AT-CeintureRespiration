/**
 * @file menuPage_config_angle_max.h
 * @author simon latour
 * @brief   menuPage_config_angle_max
 * @version 0.1
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MENUPAGE_ANGLE_MAX_H
#define MENUPAGE_ANGLE_MAX_H

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

 #define ANGLE_MAX_DEFAULT 120
 #define ANGLE_MAX_UPPER_LIMIT 200
 #define ANGLE_MAX_LOWER_LIMIT 30
 #define ANGLE_MAX_INCREMENT 5
  #define ANGLE_MAX_INCREMENT_FAST (4*ANGLE_MAX_INCREMENT)

class menuPage_angle_max: public menuPage
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

        unsigned int mAngleLimit = ANGLE_MAX_DEFAULT;
        bool configChange = false;
      

};




#endif