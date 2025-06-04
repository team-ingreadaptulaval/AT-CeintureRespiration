/**
 * @file DRMenu_main.cpp
 * @author Simon Latour
 * @brief Main menu page
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

using namespace std;

#include "string.h"
#include "menuPage_main.h"
#include "readaptLogo.h"
#include "teamATbasic_V1_1.h"

// 'SOUND_on', 15x9px
const unsigned char icon_sound_on[] PROGMEM = {
    0x08, 0x10, 0x18, 0x48, 0x38, 0x24, 0xf9, 0x12, 0xf8, 0x92, 0xf9, 0x12, 0x38, 0x24, 0x18, 0x48,
    0x08, 0x10};
// 'sound_off', 15x9px
const unsigned char icon_sound_off[] PROGMEM = {
    0x08, 0x00, 0x1b, 0x0c, 0x39, 0x98, 0xf8, 0xf0, 0xf8, 0x60, 0xf8, 0xf0, 0x39, 0x98, 0x1b, 0x0c,
    0x08, 0x00};
// 'vibration', 15x9px
const unsigned char icon_vibration[] PROGMEM = {
    0xc0, 0x06, 0x60, 0x0c, 0x37, 0xd8, 0x64, 0x4c, 0xc4, 0x46, 0x67, 0xcc, 0x37, 0xd8, 0x60, 0x0c,
    0xc0, 0x06};

    // 'Bluetooth', 6x9px
const unsigned char icon_Bluetooth [] PROGMEM = {
	0x30, 0x28, 0xa4, 0x68, 0x30, 0x68, 0xa4, 0x28, 0x30
};


// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 144)
const int icon_allArray_LEN = 4;
const unsigned char *icon_allArray[4] = {
    icon_sound_on,
    icon_sound_off,
    icon_vibration,
    icon_Bluetooth
    };

#define MAIN_DISPLAY_POS 10
#define TRAY_ORIGIN 0
#define SCREEN_MIN_SECTION (SCREEN_WIDTH_128 / 16)

#define TRAY_HEIGHT (SCREEN_MIN_SECTION * 2) // Reserve 1 section / 16  for tray

#define GLASS_SECTION_ORIGIN (TRAY_HEIGHT)
#define GLASS_SECTION_HEIGHT (SCREEN_MIN_SECTION * 6)

#define GOAL_SECTION_ORIGIN (GLASS_SECTION_ORIGIN + GLASS_SECTION_HEIGHT)
#define GOAL_SECTION_HEIGHT (SCREEN_MIN_SECTION * 6)

#define CONF_SECTION_ORIGIN (GOAL_SECTION_ORIGIN + GOAL_SECTION_HEIGHT)
#define CONF_SECTION_HEIGHT (SCREEN_MIN_SECTION * 2)

/**
 * Connect page with parent and menu manager
 * 
 * @param mManager is a pointer to the menu manager
 * @param parentMenu is apointer to the parent menu
 */
void menuPage_main::connect(menuManager *mManager, menu *parentMenu)
{
    //Connect this menu  to the menu manager and parent ( if any)
    m_menuManager = mManager;
    m_parentPage = parentMenu;

    //Connect sub menues to the menu manager and to this page an
    page_config.connect(m_menuManager, this);

    m_subMenuList[MAINSUB_CONFIG] = &page_config;
}

/**
 * @brief This functions connects to the display manager intance.
 * 
 * @param displayManager pointer tot he display manager
 */
void menuPage_main::connectDisplayManager(oledDisplayManager *displayManager)
{

    m_displayManager = displayManager;

    page_config.connectDisplayManager(displayManager);
    
}

/**
 * @brief Code to execute each loop
 * 
 */
void menuPage_main::handle()
{
    bool needDisplayRefresh = false;

    if (m_WakeUpEvent)
    {
        m_WakeUpEvent = false;
        m_DisplayTimeoutCounter = 0;

        //If display is sleeping, wakeup
        if (m_displayManager->getSleepStatus())
        {
            m_displayManager->wakeUp();
            m_focusFlag = true; // force refresh
        }
    }

    if (this->m_10msCounter >= 10)
    {
        m_10msCounter = 0;

        _blinkCounter++;
        if(_blinkCounter >=5)
        {
            _blinkCounter = 0;
            _blinkState = !_blinkState;
        }
        
        needDisplayRefresh = true;
    }

    // If display is not sleeping
    if (!m_displayManager->getSleepStatus())
    {
        //If not event for a certain amopunt of time and not already sleeping
        if (m_DisplayTimeoutCounter >= DRMENU_DISPLAY_TIMEOUT_DAY)
        {
            // m_displayManager->sleep();
            // #ifdef DEBUG_INFO_SERIAL
            //     Serial.println("Time to sleep");
            // #endif
        }

        if (m_focusFlag)
        {
            m_displayManager->display.clearDisplay();

            m_focusFlag = false;
            needDisplayRefresh = true;
        }

        // Display stuff that are refreshed only on changes
        if ((m_lastIemSelect != m_itemSelect) || needDisplayRefresh)
        {
            m_lastIemSelect = m_itemSelect; // used to only display on change
            needDisplayRefresh = true;
        }

        // Main Section Display
        if (m_itemSelect == MAIN_MENUITEM_PAGE_LASTXMIN)
        {
            if (needDisplayRefresh)
            {
                m_displayManager->display.clearDisplay();

                displayPageLastXMin();
            }
        }

        if (m_itemSelect == MAIN_MENUITEM_PAGE_TOTAL) 
        {
           if (needDisplayRefresh)
            {
                m_displayManager->display.clearDisplay();

                displayPageTotal();
            }
        }

        /*if (m_itemSelect == MAIN_MENUITEM_PAGE_MAXANGLE_LASTXMIN) 
        {
           if (needDisplayRefresh)
            {
                m_displayManager->display.clearDisplay();
                
                displayPageMaxAngleLastXMin();
            }
        }*/

        if (m_itemSelect == MAIN_MENUITEM_PAGE_MAXANGLE_TOTAL) 
        {
           if (needDisplayRefresh)
            {
                m_displayManager->display.clearDisplay();
                
                displayPageMaxAngleTotal();
            }
        }

        //Persistent menu overlay
        if (needDisplayRefresh)
        {
            //TOP TRAY SECTION
            displayTopTray();

            m_displayManager->needRefresh();
        }
    }
}


void menuPage_main::resetMaxAngle()
{
    m_maxAngleValueLastXmin = 0;
    m_maxAngleValueTotal = 0;
    
    m_focusFlag = true; //force refresh
}


/**
 * @brief Code to execute when use input is detected
 */
void menuPage_main::onPressUp()
{

    m_WakeUpEvent = true;

    m_maxAngleValueLastXmin = 0;
    m_maxAngleValueTotal = 0;
    
    m_focusFlag = true; //force refresh



}

void menuPage_main::onLongPressUp()
{
    m_WakeUpEvent = true;

    m_longOnOff = !m_longOnOff;

    m_focusFlag = true; //force refresh

    // Refresh display to indicate user that is is started now because opening the file is long
    handle();
    m_displayManager->display.display();
    
    
}



/**
 * @brief Code to execute when use input is detected
 */
void menuPage_main::onPressDown()
{
    m_WakeUpEvent = true;
    // If device is sleeping, button press only wakes up device
    if (m_displayManager->getSleepStatus())
    {
        return;
    }


        m_itemSelect++;
        if (m_itemSelect >= MENUITEM_COUNT)
        {
          m_itemSelect = 0;
        }
        m_focusFlag = true; //force refresh
        

   // m_displayManager->wakeUp();
}

/**
 * @brief Code to execute when use input is detected
 */
void menuPage_main::onPressReturn()
{
    m_WakeUpEvent = true;

    // If device is sleeping, button press only wakes up device
    if (m_displayManager->getSleepStatus())
    {
        return;
    }

    // if (m_itemSelect == MAIN_MENUITEM_PAGE_MAXANGLE_TOTAL) //Force Refresh to update timer
    // {
    //     //m_maxAngleValueTotal = 0;
    // }

    m_focusFlag = true; //force refresh
}

void menuPage_main::onLongPressReturn()
{
    m_WakeUpEvent = true;

    //If device is sleeping, button press only wakes up device
    if (m_displayManager->getSleepStatus())
    {
        return;
    }

   resetMaxAngle();
}




/**
 * @brief Code to execute when use input is detected
 */
void menuPage_main::onPressEnter()
{
    m_WakeUpEvent = true;
    // If device is sleeping, button press only wakes up device
    if (m_displayManager->getSleepStatus())
    {
        return;
    }

    m_menuManager->setActivePage(m_subMenuList[MAINSUB_CONFIG]); //Switch to
}

void menuPage_main::onFocus()
{
    m_focusFlag = true;
    m_WakeUpEvent = true;
}



/**
 * @brief This function needs to be called every 100 ms
 * 
 */
void menuPage_main::Callback10ms()
{
    m_10msCounter++;
    m_DisplayTimeoutCounter += 10;
}

#define TOP_BAR_SECTION_COUNT 6
#define TOP_BAR_SECTION_WIDTH (SCREEN_WIDTH_128 / TOP_BAR_SECTION_COUNT)
#define TOP_BAR_SECTION_HEIGHT 10

#define BAT_POS_X (0 * TOP_BAR_SECTION_WIDTH)
#define VIBRATION_POS_X (1 * TOP_BAR_SECTION_WIDTH)
#define SOUND_POS_X (2 * TOP_BAR_SECTION_WIDTH)
#define BT_POS_X (3 * TOP_BAR_SECTION_WIDTH)
#define REC_POS_X (4 * TOP_BAR_SECTION_WIDTH)
#define RECNUMBER_POS_X (5 * TOP_BAR_SECTION_WIDTH)

void menuPage_main::displayTopTray()
{

    m_displayManager->display.setRotation(0);
    //m_displayManager->displayBattery(1, 1, 10, 26,batteryPercent);
    m_displayManager->displayBattery(BAT_POS_X, 0, 7, 18, batteryPercent); //TODO connecter batterie
    
    if(page_config.pageAlerts.getVibrationAlertEnable())
    {
        m_displayManager->display.drawBitmap(VIBRATION_POS_X, 0, icon_vibration, 15, 9, WHITE);
    }

    if(page_config.pageAlerts.getToneAlertEnable())
    {
        m_displayManager->display.drawBitmap(SOUND_POS_X, 0, icon_sound_on, 15, 9, WHITE);
    }

    if(bluetoothConnected)
    {
        m_displayManager->display.drawBitmap(BT_POS_X, 0, icon_Bluetooth, 6, 9, WHITE);
    }

    if(m_longOnOff)
    {
        switch(m_logMode)
        {
            case logMode_SD:
                displayRec(REC_POS_X, 1);
                m_displayManager->widget_1row(RECNUMBER_POS_X,1,TOP_BAR_SECTION_WIDTH*2,TOP_BAR_SECTION_HEIGHT,logFilename,1,widgetAlignment::topCenter,0);
                
                break;

            case logMode_SERIAL:
                m_displayManager->widget_1row(REC_POS_X,1,TOP_BAR_SECTION_WIDTH*2,TOP_BAR_SECTION_HEIGHT,"Ser.",1,widgetAlignment::topRight,0);
                break;

            default:
                displayRec(REC_POS_X, 1);
                break;
        }
       

        
    }
    
}
void menuPage_main::displayRec(int16_t posX, int16_t posY)
{
    m_displayManager->display.setCursor(posX, posY);
    m_displayManager->display.setTextSize(1);
    m_displayManager->display.setTextColor(WHITE);
    m_displayManager->display.write("REC");

    
    if(_blinkState)
    {
        m_displayManager->display.fillCircle(posX + 25, posY + 3, 4, WHITE);
    }
    else
    {
        m_displayManager->display.drawCircle(posX + 25, posY + 3, 4, WHITE);
    }

}

#define MAIN_SECTION_HEIGHT (SCREEN_HEIGHT_64 - TOP_BAR_SECTION_HEIGHT)
#define H_CENTER (SCREEN_WIDTH_128 / 2)

void menuPage_main::displayPageLastXMin()
{

    //   char timeString[8];
    //   millisToHumanTime(page_config.page_Length.mPeriodLength,timeString,8);

    m_displayManager->widget_1row(0, TOP_BAR_SECTION_HEIGHT, 128, SMALL_FONT_ROW_HEIGH, 
            String("Periode "+ String(page_config.page_Length.mPeriodLength/1000/60) + " Min").c_str(), 
            1, widgetAlignment::center,W_NO_PADDING);
      
    displaySectionLastXMinRep(0, TOP_BAR_SECTION_HEIGHT + SMALL_FONT_ROW_HEIGH , 64, 
                                MAIN_SECTION_HEIGHT - SMALL_FONT_ROW_HEIGH);
    // Vertical separator
    m_displayManager->display.drawFastVLine(H_CENTER,TOP_BAR_SECTION_HEIGHT+10+6,
                                            MAIN_SECTION_HEIGHT-SMALL_FONT_ROW_HEIGH-12,WHITE);

    displaySectionLastXMinTime(64, TOP_BAR_SECTION_HEIGHT + SMALL_FONT_ROW_HEIGH , 64, 
                                MAIN_SECTION_HEIGHT - SMALL_FONT_ROW_HEIGH);
}

void menuPage_main::displayPageTotal()
{

    m_displayManager->widget_1row(0, TOP_BAR_SECTION_HEIGHT, 128, SMALL_FONT_ROW_HEIGH, 
                                    String("Total").c_str(), 1, W_CENTER_ALIGN,W_NO_PADDING);
      
    displaySectionTotalRep(0, TOP_BAR_SECTION_HEIGHT + SMALL_FONT_ROW_HEIGH , 64, 
                            MAIN_SECTION_HEIGHT - SMALL_FONT_ROW_HEIGH);
    // Vertical separator
    m_displayManager->display.drawFastVLine(H_CENTER,TOP_BAR_SECTION_HEIGHT+10+6,
                                            MAIN_SECTION_HEIGHT-SMALL_FONT_ROW_HEIGH-12,WHITE);

    displaySectionTotalTime(64, TOP_BAR_SECTION_HEIGHT + SMALL_FONT_ROW_HEIGH , 
                            64, MAIN_SECTION_HEIGHT - SMALL_FONT_ROW_HEIGH);
}

// void menuPage_main::displayPageMaxAngleLastXMin()
// {
//       displaySectionAngle(0, TOP_BAR_SECTION_HEIGHT, H_CENTER, MAIN_SECTION_HEIGHT,"Angle", String(m_angleValue));
//       // Vertical separator
//     m_displayManager->display.drawFastVLine(H_CENTER,TOP_BAR_SECTION_HEIGHT+10+6,MAIN_SECTION_HEIGHT-SMALL_FONT_ROW_HEIGH-12,WHITE);
//       displaySectionAngle(H_CENTER, TOP_BAR_SECTION_HEIGHT, H_CENTER, MAIN_SECTION_HEIGHT,String(
//           String(page_config.page_Length.mPeriodLength) + " Min"),String(m_maxAngleValueLastXmin));
// }

void menuPage_main::displayPageMaxAngleTotal()
{
      displaySectionAngle(0, TOP_BAR_SECTION_HEIGHT, H_CENTER, MAIN_SECTION_HEIGHT,"Angle", String(m_angleValue));
      // Vertical separator
    m_displayManager->display.drawFastVLine(H_CENTER,TOP_BAR_SECTION_HEIGHT+10+6,MAIN_SECTION_HEIGHT-SMALL_FONT_ROW_HEIGH-12,WHITE);
      displaySectionAngle(H_CENTER, TOP_BAR_SECTION_HEIGHT, H_CENTER, MAIN_SECTION_HEIGHT,String("Max"),String(m_maxAngleValueTotal));
}


void menuPage_main::displaySectionAngle(int16_t posX, int16_t posY, int16_t width, int16_t height,String title, String angleText)
{

    m_displayManager->widget_3Row(posX, posY, width, height,
                                        title, 1,
                                        angleText, 2,
                                        "deg.", 1);
}


void menuPage_main::displaySectionLastXMinRep(int16_t posX, int16_t posY, int16_t width, int16_t height)
{
    int repLimit = getRepLimitXmin();//menuPage_main::page_config.page_rep.mRepLimit;
    if(repLimit > 0)
    {
        m_displayManager->widget_divisor(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                     String(m_lastXminRep).c_str(), String(repLimit), 2);
    }
    else
    {
        m_displayManager->widget_1row(posX, posY, width, height - SMALL_FONT_ROW_HEIGH, 
                                    String(m_lastXminRep).c_str(),2,W_CENTER_ALIGN);
    }

    

    m_displayManager->widget_1row(posX, posY + height - SMALL_FONT_ROW_HEIGH , 
                                    width, SMALL_FONT_ROW_HEIGH, "rep.",1,W_CENTER_ALIGN,W_NO_PADDING);                           
}

void menuPage_main::displaySectionLastXMinTime(int16_t posX, int16_t posY, int16_t width, int16_t height)
{
    int timeLimit = menuPage_main::page_config.page_time.mTimeTargetMilliseconds;

    if(timeLimit > 0)
    {
         m_displayManager->widget_divisor(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                     String((int)m_lastXminTimeSeconds/60).c_str(), String((int)timeLimit/1000/60).c_str(), 2);
    }
    else
    {
        m_displayManager->widget_1row(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                     String((int)m_lastXminTimeSeconds/60).c_str(),2,W_CENTER_ALIGN,W_NO_PADDING);         
    }

    m_displayManager->widget_1row(posX, posY + height - SMALL_FONT_ROW_HEIGH , 
                                    width, SMALL_FONT_ROW_HEIGH, "min.",1,W_CENTER_ALIGN,W_NO_PADDING);  

    // int16_t progressBarHeight = 5;
    // int16_t progressBarWidth = width - 8;
    // int percent = (((int)m_lastXminTimeSeconds % 60)*100) /60;

    // int timeLimit = menuPage_main::page_config.page_time.mTimeLimit;
    // m_displayManager->widget_divisor(posX, posY, width, height - SMALL_FONT_ROW_HEIGH - progressBarHeight,
    //                                  String((int)m_lastXminTimeSeconds/60).c_str(), String((int)timeLimit/1000/60).c_str(), 1);
    // m_displayManager->widget_1row(posX, posY + height - SMALL_FONT_ROW_HEIGH , 
    //                                 width, SMALL_FONT_ROW_HEIGH, "min.",1,W_CENTER_ALIGN,W_NO_PADDING);  

    
    // m_displayManager->display.drawRect(posX+5,posY + height - progressBarHeight - SMALL_FONT_ROW_HEIGH,progressBarWidth, progressBarHeight,WHITE);
    // int progressWidth = percent * (width-6)/100;
    // m_displayManager->display.fillRect(posX+5+1,posY + height - progressBarHeight + 3 - SMALL_FONT_ROW_HEIGH-2,progressWidth, progressBarHeight-2,WHITE);
}

void menuPage_main::displaySectionTotalRep(int16_t posX, int16_t posY, int16_t width, int16_t height)
{

    
    int RepLimit = 0; //TODO if configurable one day: menuPage_main::page_config.pageDay.page_rep.mRepLimit;

    if(RepLimit == 0)
    {
        m_displayManager->widget_1row(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                        String(m_totalRep).c_str(),MED_FONT,W_CENTER_ALIGN,W_NO_PADDING);
    }
    else{
        m_displayManager->widget_divisor(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                            String(m_totalRep), String(RepLimit), 2);
    }
    

    m_displayManager->widget_1row(posX, posY + height - SMALL_FONT_ROW_HEIGH , width, 
                                        SMALL_FONT_ROW_HEIGH, "rep.",1,W_CENTER_ALIGN,W_NO_PADDING);                           
}

void menuPage_main::displaySectionTotalTime(int16_t posX, int16_t posY, int16_t width, int16_t height)
{

    int timeLimit = 0; //TODO if configurable one day:  menuPage_main::page_config.pageDay.page_time.mTimeLimit;

    if(timeLimit == 0)
    {
        m_displayManager->widget_1row(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                        String((int)m_totalTimeSeconds/60).c_str(),MED_FONT,W_CENTER_ALIGN,W_NO_PADDING);
    }else
    {
        m_displayManager->widget_divisor(posX, posY, width, height - SMALL_FONT_ROW_HEIGH,
                                     String((int)m_totalTimeSeconds/60), String((int)timeLimit), 2);
    }
   
    m_displayManager->widget_1row(posX, posY + height - SMALL_FONT_ROW_HEIGH , width, SMALL_FONT_ROW_HEIGH, "min.",1,W_CENTER_ALIGN,W_NO_PADDING);  
}


void menuPage_main::setAngle(int angle)
{
    m_angleValue = angle;
}
void menuPage_main::setMaxAngleTotal(int angle)
{
    m_maxAngleValueTotal = angle;
}
void menuPage_main::setMaxAngleLastXmin(int angle)
{
    m_maxAngleValueLastXmin = angle;
}

int menuPage_main::getAngle()
{
    return m_angleValue;
}
int menuPage_main::getMaxAngleTotal()
{
    return m_maxAngleValueTotal;
}

int menuPage_main::getMaxAngleLastXmin()
{
    return m_maxAngleValueLastXmin;
}

  void menuPage_main::setRepXmin(int rep)
  {
      m_lastXminRep = rep;
  }

  int menuPage_main::getRepXmin()
  {
      return m_lastXminRep;
  }

  void menuPage_main::setRepTotal(int rep)
  {
      m_totalRep = rep;
  }

  int menuPage_main::getRepTotal()
  {
      return m_totalRep;
  }


  void menuPage_main::setTimeSecondsElevationTotal(float time)
  {
      m_totalTimeSeconds = time;
  }
  void menuPage_main::setTimeSecondsElevationXmin(float time)
  {
      m_lastXminTimeSeconds = time;
  }

  float menuPage_main::getTimeSecondsElevationTotal()
  {
      return m_totalTimeSeconds;
  }

  float menuPage_main::getTimeSecondsElevationXmin()
  {
      return m_lastXminTimeSeconds;
  }

  int menuPage_main::getRepLimitXmin()
  {
      return page_config.page_rep.mRepLimit;
  }

  int menuPage_main::getRepLimitTotal()
  {
      return 0;//page_config.pageDay.page_rep.mRepLimit;
  }

  float menuPage_main::getTimeLimitXmin_milliseconds()
  {
      return page_config.page_time.mTimeTargetMilliseconds;
  }
  
  float menuPage_main::getTimeLimitTotal_milliseconds()
  {
      return 0;//page_config.pageDay.page_time.mTimeLimit;
  }

  void menuPage_main::setResetElevationAngle(int value)
  {
    m_resetElevationAngle = value;
  }

  int menuPage_main::getResetElevationAngle()
  {
    return m_resetElevationAngle;
  }
    

bool menuPage_main::getLogStatus()
{
  return m_longOnOff;      
}

void menuPage_main::setLogNumber(string logNumber)
{

    logFilename = logNumber;
}

void menuPage_main::setLogMode(logModes logMode)
{
    m_logMode = logMode;
}