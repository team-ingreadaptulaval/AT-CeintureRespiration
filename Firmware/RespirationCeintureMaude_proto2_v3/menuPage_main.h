/**
 * @file DRMenu_main.h
 * @author Simon Latour
 * @brief Main menu page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "menuManager.h"
#include "menuPage_config.h"
#include "oledDisplayManager_V1_1.h"



//TODO LISTE
// TODO Reset valeurs sur un "hold 2 sec" de return


enum mainSubMenuList{
    MAINSUB_CONFIG,
    MAINSUB_COUNT
};

 
 enum menuItemList{
        MAIN_MENUITEM_PAGE_MAXANGLE_TOTAL,
        MAIN_MENUITEM_PAGE_LASTXMIN,
        MAIN_MENUITEM_PAGE_TOTAL,
        //MAIN_MENUITEM_PAGE_MAXANGLE_LASTXMIN,
        MENUITEM_COUNT            
    };

 enum textBoxList{
        MAIN_TEXTBOX_SYSTEMSTATUS,
        MAIN_TEXTBOX_LIGHTVALUE,
        TEXTBOX_COUNT            
    };

#define DRMENU_DISPLAY_TIMEOUT_DAY      15000
#define DRMENU_DISPLAY_TIMEOUT_NIGHT    5000

enum logModes{
    logMode_off,
    logMode_SD,
    logMode_SERIAL
};

class menuPage_main: public menuPage {

    private: 
    
    menuManager* m_menuManager;

    menu* m_subMenuList[MAINSUB_COUNT];

    oledDisplayManager* m_displayManager; 

    ////DISPLAY FUNCTIONS /// 
    void displayTopTray();
    void displayRec(int16_t posX,int16_t posY); // Display RECORD Active
    void displayMainScreen();
    void displayPageLastXMin();
    void displayPageTotal();
    //void displayPageMaxAngleLastXMin();
    void displayPageMaxAngleTotal();
    void displaySectionLiveAngle(int16_t posX,int16_t posY, int16_t width, int16_t height);
    void displaySectionAngle(int16_t posX, int16_t posY, int16_t width, int16_t height,String title, String angleText);
    void displaySectionDayGoal(int16_t posX,int16_t posY, int16_t width, int16_t height);
    void displaySectionLastXMinRep(int16_t posX,int16_t posY, int16_t width, int16_t height);
    void displaySectionLastXMinTime(int16_t posX,int16_t posY, int16_t width, int16_t height);
    void displaySectionTotalRep(int16_t posX,int16_t posY, int16_t width, int16_t height);
    void displaySectionTotalTime(int16_t posX,int16_t posY, int16_t width, int16_t height);

   
    const char* m_itemLabelList[MENUITEM_COUNT] = {
                                                    "System",
                                                    "Temps:",
                                                    };

    unsigned int  m_lastIemSelect = 65535;
    unsigned int m_itemSelect = 0;

   unsigned long m_10msCounter = 0;
   
   bool _blinkState = false;
   unsigned int _blinkCounter = 0;

   int m_DisplayTimeoutCounter = 0;
   bool m_WakeUpEvent = false;

    int m_angleValue = 0;
    int m_maxAngleValueLastXmin = 0;
    int m_maxAngleValueTotal = 0;

    float m_lastXminTimeSeconds;
    int m_lastXminRep;

    float m_totalTimeSeconds;
    int m_totalRep;

    int m_resetElevationAngle = 0;

    bool m_longOnOff = false;

    
    string logFilename = "0000";
    logModes m_logMode = logMode_off;

    public:

    bool bluetoothConnected = false;


    void setAngle(int angle);
    void setMaxAngleTotal(int angle);
    void setMaxAngleLastXmin(int angle);

    int getAngle();
    int getMaxAngleTotal();
    int getMaxAngleLastXmin();

    void setRepXmin(int rep);
    int  getRepXmin();
    void setRepTotal(int rep);
    int  getRepTotal();

    int getRepLimitXmin();
    int getRepLimitTotal();


    void setTimeSecondsElevationTotal(float time);
    void setTimeSecondsElevationXmin(float time);
    float getTimeSecondsElevationTotal();
    float getTimeSecondsElevationXmin();

    float getTimeLimitXmin_milliseconds();
    float getTimeLimitTotal_milliseconds();
    void setResetElevationAngle(int value);
    int getResetElevationAngle();
  
    bool getLogStatus();
    void setLogNumber(string logNumber);
    void setLogMode(logModes logMode);

    void resetMaxAngle();
    

    int tbTimerValue;
    bool systemOnOffStatus = true;   

    int batteryPercent = 0;
    
    void connect(menuManager* context, menu* parentMenu=NULL);
    void connectDisplayManager(oledDisplayManager * displayManager);

    menuPage_config page_config;

    void handle();

    void onPressUp();
    void onLongPressUp();
    void onPressDown();
    void onPressReturn();
    void onLongPressReturn();
    void onPressEnter();
    void onFocus();


    int labelDelay = 0;
    
    void Callback10ms();

};


#endif
