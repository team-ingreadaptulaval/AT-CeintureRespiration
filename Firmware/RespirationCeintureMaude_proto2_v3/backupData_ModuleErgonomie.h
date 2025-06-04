
#ifndef BACKUPDATA_MODULEERGONOMIE_H
#define BACKUPDATA_MODULEERGONOMIE_H


#include "menuPage_config_angle_detect.h"
#include "menuPage_config_angle_max.h"
// #include "menuPage_day_rep.h"
// #include "menuPage_day_time.h"
#include "menuPage_config_refPeriod_length.h"
#include "menuPage_config_refPeriod_rep.h"
#include "menuPage_config_refPeriod_time.h"


    #define INDEX_DEFAULT_VIBRATION_ONOFF  0                  
    #define INDEX_DEFAULT_TONE_ONOFF     1                    
    #define INDEX_DEFAULT_REPALERT_ONOFF   2                  
    #define INDEX_DEFAULT_TIMEALERT_ONOFF   3                 
    #define INDEX_DEFAULT_ANGLEDETECT_ONOFF 4                 
    #define INDEX_DEFAULT_ANGLEMAX_ONOFF 5
    #define DEFAULT_BLUTOOTH_NAME   "Module Ergo"
    #define BLUTOOTH_NAME_MAX_SIZE  32

    #define BACKUP_STRUCT_VERSION   010100  //01.00.00
    typedef struct
        {
            unsigned int  structVersion;
            uint8_t alertsOnOff[6];
            int angleElevation;     // Detection of elevation
            int angleLimit;         // Absolume maximum limit

            unsigned int repLimit_lastXmin;
            unsigned int timeLimit_lastXmin;
            unsigned int periodLenght_lastXmin;
            char bluetoothDeviceName[BLUTOOTH_NAME_MAX_SIZE];
            //unsigned int repLimit_Total;
            //unsigned int timeLimit_Total;



        }backupStruct_t;

    


#endif