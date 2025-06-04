
#ifndef MONRE_LIGHT_ARDUINO_DEFINES_H
#define MONRE_LIGHT_ARDUINO_DEFINES_H


#define SPEAKER_OUT D8

#define led_alert_pin  D3
#define led_sd_pin  D2
#define audioread_pin  A3
#define IN_BAT_VOLTAGE  A0

#define IN_BUTTON_UP        A6 // 
#define IN_BUTTON_DOWN      A7 // 
#define IN_BUTTON_ENTER     D0 // 
#define IN_BUTTON_RETURN    D1 // 


    //#if ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
     #define OLED_DC   D9
     #define OLED_RESET   D6
     #define OLED_CS   D7
    //#endif

#endif
