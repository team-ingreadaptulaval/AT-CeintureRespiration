/**
 * @file ButtonPollingManager_V2.3.cpp
 * @author Simon Latour
 * @brief  This library is used to simplify and manage Button polling.
 * @version 2.3
 * @date 2022-07-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ButtonPollingManager_V2.3.h"
#include "at_plaformAbstraction_V1_1.h"
#include "teamATbasic_V1_1.h"


void ButtonPollingManager::init(unsigned char nbButtons, managedButton *buttonConfigList)
{
	buttons = buttonConfigList;
	mNbButtons = nbButtons;



    for(int i = 0 ; i < mNbButtons;  i++)
    {
        buttons[i].clickEventFlag = false;
        buttons[i].longClickEventFlag = false;
        buttons[i].doubleClickEventFlag = false;
        buttons[i].holdEvent = false;

        buttons[i].currentState = digitalPinRead(buttons[i].config.buttonPin, buttons[i].config.buttonPort);
        buttons[i].lastState = buttons[i].currentState;


        buttons[i].clickListeningFlag = false;
        buttons[i].clickListeningCount = 0;
        buttons[i].clickListeningMillis = 0;
        buttons[i].clickListeningLongClickEnable = true;
    }
}

void ButtonPollingManager::handle(unsigned long currentTimeMillis)
{

  //Clear previous event
  for(int i = 0 ; i < mNbButtons;  i++)
  {
    
    buttons[i].clickEventFlag = false;
    buttons[i].longClickEventFlag = false;
    buttons[i].doubleClickEventFlag = false;

    buttons[i].lastState = buttons[i].currentState;
    if((currentTimeMillis-buttons[i].lastChangeMillis) >= BUTTON_DEBOUNCE_TIME)
    {
        buttons[i].currentState = digitalPinRead(buttons[i].config.buttonPin, buttons[i].config.buttonPort);
    }
    
    if(buttons[i].currentState != buttons[i].lastState )  //Button change detected
    {
        buttons[i].lastChangeMillis = currentTimeMillis ;
        if(buttons[i].currentState == buttons[i].config.polarity) // Button pressed
        {
            buttons[i].buttonDonwMillis = currentTimeMillis; 
        }
        else  //Button released
        {
            buttons[i].holdEvent = false; //If button was in hold state, we need to clear it
            buttons[i].clickListeningLongClickEnable = true;

          if((currentTimeMillis - buttons[i].buttonDonwMillis) <= BUTTON_SHORT_CLICK_TIME)
          {
              if (buttons[i].clickListeningFlag == false)
              {
                  buttons[i].clickListeningFlag = true;
                  buttons[i].clickListeningMillis = currentTimeMillis;
                  buttons[i].clickListeningCount ++;
              }
              else
              {
                buttons[i].clickListeningCount ++;
              }
          }
          else if((currentTimeMillis - buttons[i].buttonDonwMillis) < BUTTON_LONG_ONRELEASE_CLICK_TIME && (buttons[i].config.clickingOptionsActive & HOLD_ENABLE))
          {
                if((buttons[i].config.clickingOptionsActive & LONGCLICK_ENABLE))
                  {buttons[i].longClickEventFlag = true;}
          }

            /*if((currentTimeMillis - buttons[i].buttonDonwMillis) <= BUTTON_SHORT_CLICK_TIME)
            {

                if(currentTimeMillis - buttons[i].lastClickMillis < BUTTON_DOUBLE_CLICK_TIME)
                {
                    buttons[i].doubleClickEventFlag =  true;
                    buttons[i].lastClickMillis = 0; // avoir double click repeat
                }
                else
                {
                    buttons[i].clickEventFlag =  true;
                    buttons[i].lastClickMillis = currentTimeMillis;
                }

            }
            else if((currentTimeMillis - buttons[i].buttonDonwMillis) <= BUTTON_LONG_CLICK_TIME)
            {
                    buttons[i].longClickEventFlag =  true;
            }*/
        }
    }
    else
    {
        if(!(buttons[i].config.clickingOptionsActive & HOLD_ENABLE))//le hold n'est pas actif
        {
          if(buttons[i].currentState == buttons[i].config.polarity && (timeElapsed(currentTimeMillis, buttons[i].buttonDonwMillis) > BUTTON_LONG_CLICK_TIME) && buttons[i].clickListeningLongClickEnable == true )
          {
              if((buttons[i].config.clickingOptionsActive & LONGCLICK_ENABLE))
                {buttons[i].longClickEventFlag = true;}
              buttons[i].clickListeningLongClickEnable = false;
          }
        }

        if((buttons[i].config.clickingOptionsActive & HOLD_ENABLE))//le hold est  actif
        {
          unsigned long BUTTON_CLICK_HOLD = BUTTON_SHORT_CLICK_TIME;
          if((buttons[i].config.clickingOptionsActive & LONGCLICK_ENABLE))
          {
            BUTTON_CLICK_HOLD = BUTTON_LONG_ONRELEASE_CLICK_TIME;
          }
          if(buttons[i].currentState == buttons[i].config.polarity && (timeElapsed(currentTimeMillis, buttons[i].buttonDonwMillis) > BUTTON_CLICK_HOLD) )
          {
              buttons[i].holdEvent = true;
          }
        }
    }

    unsigned long clickListeningWaitTime = BUTTON_SHORT_CLICK_TIME;
    if((buttons[i].config.clickingOptionsActive & DOUBLECLICK_ENABLE))
    {
      clickListeningWaitTime = BUTTON_SHORT_CLICK_TIME + BUTTON_DOUBLE_CLICK_TIME;
    }
    
    if (buttons[i].clickListeningFlag == true)
    {
      
      if (((currentTimeMillis - buttons[i].clickListeningMillis) > clickListeningWaitTime) || !(buttons[i].config.clickingOptionsActive & DOUBLECLICK_ENABLE) || buttons[i].clickListeningCount > 1)
      {
        buttons[i].clickListeningFlag = false;
        
        if (buttons[i].clickListeningCount == 1)
        {
          if((buttons[i].config.clickingOptionsActive & CLICK_ENABLE))
            {buttons[i].clickEventFlag =  true;}
        }
        else if (buttons[i].clickListeningCount > 1)
        {
          if((buttons[i].config.clickingOptionsActive & CLICK_ENABLE))
            {buttons[i].doubleClickEventFlag =  true;}
          else if((buttons[i].config.clickingOptionsActive & CLICK_ENABLE))
            {buttons[i].clickEventFlag =  true;}
        }
        
        buttons[i].clickListeningCount = 0;
      }
    }
    
  }      
   

}

bool  ButtonPollingManager::getClickEvent(int buttonIndex)
{
    if(buttonIndex < mNbButtons)
    {
        return buttons[buttonIndex].clickEventFlag;
    }
  
  return false;
  
}

bool ButtonPollingManager::getDoubleClickEvent(int buttonIndex)
{
     if(buttonIndex < mNbButtons)
    {
        return buttons[buttonIndex].doubleClickEventFlag;
    }
  
  return false;
}

bool ButtonPollingManager::getLongClickEvent(int buttonIndex)
{
     if(buttonIndex < mNbButtons)
    {
        return buttons[buttonIndex].longClickEventFlag;
    }
  
  return false;
}



bool ButtonPollingManager::getHoldEvent(int buttonIndex)
{
    if(buttonIndex < mNbButtons)
    {
        return buttons[buttonIndex].holdEvent;
    }
  
  return false;
}
