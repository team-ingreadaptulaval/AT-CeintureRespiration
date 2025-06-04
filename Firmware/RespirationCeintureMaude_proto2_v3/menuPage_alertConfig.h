/**
 * @file DRMenu_configReminder.h
 * @author Simon Latour
 * @brief  Reminder config page 
 * @version 0.1
 * @date 2020-11-19
 * 
 * //Note: Pour les "item labels" à cause que les titres peuvent varier, j'ai changé pour utiliser des vecteurs. Idéalement on ferait la même chose partout ça ca marche très bient.
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MENUPAGE_ALERTCONFIG_H
#define MENUPAGE_ALERTCONFIG_H

using namespace std;

#include <vector>
#include <string>

#include "menuManager.h"
#include "oledDisplayManager_V1_1.h"

//#include "menuPage_OnOff.h""

#define   DEFAULT_VIBRATION_ONOFF   1
#define   DEFAULT_TONE_ONOFF        1 
#define   DEFAULT_REPALERT_ONOFF    1
#define   DEFAULT_TIMEALERT_ONOFF   1
#define   DEFAULT_ANGLEDETECT_ONOFF 0
#define   DEFAULT_ANGLEMAX_ONOFF    1


class menuPage_alertConfig: public menuPage
{
    private: 

        menuManager* m_menuManager;

        enum menuItemList {
                                MENUITEM_VIBRATION_ONOFF,
                                MENUITEM_TONE_ONOFF,
                                MENUITEM_REPALERT_ONOFF,
                                MENUITEM_TIMEALERT_ONOFF,
                                MENUITEM_ANGLEDETECT_ONOFF,
                                MENUITEM_ANGLEMAX_ONOFF,
                                MENUITEM_COUNT
                            };
        
        vector<string> itemLabelList{
                                 "Vibrations",
                                 "Tonalite",
                                "Alerte Rep.",
                                "Alerte Temps",
                                "Angle elev.",
                                "Angle Max.",
                            };
     
        vector<string> onOffLabelList = { "off"," on","off"," on","off"," on"};

        uint8_t onOffStatusList[MENUITEM_COUNT] = {
                                1,
                                1,
                                0,
                                1,
                                0,
                                0
                            };

        void updateOnOffLabel();

       
        

        menuPage* m_subMenuList[MENUITEM_COUNT]; 
        int m_itemSelect = 0;
 
        oledDisplayManager* m_displayManager; 

    public:

        void connect(menuManager* context, menu* parentMenu);
        void connectDisplayManager(oledDisplayManager * displayManager);
    


        // Menu Functions
        void handle();

        void onPressUp();
        void onPressDown();
        void onPressReturn();
        void onPressEnter();
        void onFocus();

        bool configChange = false;

        uint8_t getVibrationAlertEnable();
        uint8_t getToneAlertEnable();
        uint8_t getRepetitionAlertEnable();
        uint8_t getTimeAlertEnable();
        uint8_t getAngleDetectAlertEnable();
        uint8_t getAngleMaxAlertEnable(); 

        void setVibrationAlertEnable(uint8_t onOff);
        void setToneAlertEnable(uint8_t onOff);
        void setRepetitionAlertEnable(uint8_t onOff);
        void setTimeAlertEnable(uint8_t onOff);
        void setAngleDetectAlertEnable(uint8_t onOff);
        void setAngleMaxAlertEnable(uint8_t onOff); 

};




#endif