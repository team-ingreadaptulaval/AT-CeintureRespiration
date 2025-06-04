/**
 * @file ButtonPollingManager
 * @author Simon Latour
 * @brief  This library is used to simplify and manage Button polling.
 * @version 2.2
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#ifndef BUTTONPOLLINGMANAGER_H
#define BUTTONPOLLINGMANAGER_H

#include <stdint.h>





// Button reaction defines
#define BUTTON_DEBOUNCE_TIME 50
#define BUTTON_SHORT_CLICK_TIME 500 // Anything under 500ms is considered à click
#define BUTTON_DOUBLE_CLICK_TIME 500 // Anything under 500ms is considered à click
#define BUTTON_LONG_CLICK_TIME BUTTON_SHORT_CLICK_TIME // Anything under 500ms is considered à click
#define BUTTON_LONG_ONRELEASE_CLICK_TIME 2000
//#define BUTTON_CLICK_HOLD BUTTON_SHORT_CLICK_TIME //anythign longer than a click is considered a hold

// Polarity defines
#define ACTIVE_LOW	0
#define ACTIVE_HIGH	1

// Port defines
#define NO_PORT 0

// Bit position for types of clicks
#define BITPOS_CLICK			0
#define BITPOS_DOUBLE_CLICK 	1
#define BITPOS_LONG_CLICK		3
#define BITPOS_HOLD				4

#define CLICK_ENABLE		(0x01 << BITPOS_CLICK)
#define DOUBLECLICK_ENABLE	(0x01 << BITPOS_DOUBLE_CLICK)
#define LONGCLICK_ENABLE	(0x01 << BITPOS_LONG_CLICK)
#define HOLD_ENABLE			(0x01 << BITPOS_HOLD)

typedef struct 
{
	uint16_t buttonPin;
	uint32_t *buttonPort;
	unsigned char polarity;
  	uint8_t clickingOptionsActive; //see bit positions above

}ButtonConfig;

typedef struct 
{
	ButtonConfig config;

	int lastState;
	int currentState;

	unsigned long lastChangeMillis;
	unsigned long lastClickMillis;
	unsigned long buttonDonwMillis;

 	bool clickListeningFlag;
  	int clickListeningCount;
  	unsigned long clickListeningMillis;
  	bool clickListeningLongClickEnable;

	bool clickEventFlag;
	bool doubleClickEventFlag;
	bool longClickEventFlag;
	bool holdEvent;

}managedButton;



/**
 * @class ButtonPollingManager
 * @brief Made to manage button polling and extract events such as click detect, hold detect
 *
 */
class ButtonPollingManager
{
    private:

	managedButton *buttons;
	int mNbButtons = 0;

    public:

	/**
	 * @fn void init(unsigned char, managedButton*)
	 * @brief In order to initialize the button polling manager, we need the number of buttons to manage
	 *  	  and the list of buttons
	 *
	 * @param nbButtons the number of buttons to manage
	 * @param buttonConfigList Pointer to the list of buttons  \see managedButton
	 */
	void init(unsigned char nbButtons, managedButton *buttonConfigList);

    /**
     * @brief Call this function in the main loop
     * 
     * @param currentTimeMillis the current system time/tick in ms
     */
    void handle(unsigned long currentTimeMillis);

	/**
	* @brief This functions returns the status if a button click event is true
	*
	* @param  buttonIndex   The index of the manage button in the list
	* @return true when a click was detected
	*/
	bool getClickEvent(int buttonIndex);

	/**
	* @brief This functions returns the status if a double button click event is true
	*
	* @param  buttonIndex   The index of the manage button in the list
	* @return true when a double click was detected
	*/
	bool getDoubleClickEvent(int buttonIndex);


	/**
	* @brief This functions returns the status if a button long click event is true
	*
	* @param  buttonIndex   The index of the manage button in the list
	* @return true when a long click was detected
	*/
	bool getLongClickEvent(int buttonIndex);


	/**
	* @brief This function is used to know if a hold event is active for a button
	*
	* @param buttonIndex   The index of the button in the button list
	* @return true         if button hold status in progress
	*/
	bool getHoldEvent(int buttonIndex);

};


#endif
