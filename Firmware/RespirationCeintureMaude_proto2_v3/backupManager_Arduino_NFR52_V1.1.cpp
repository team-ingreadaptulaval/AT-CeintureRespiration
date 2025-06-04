/**
 * @file backupManager_Arduino_NFR52_V1.1.cpp
 * @author Simon Latour
 * @brief 
 * @version 1.1
 * @date 2021-09-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "backupManager_Arduino_NFR52_V1.1.h"
#if ( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )


    #include "kvstore_global_api.h"
    #include "Arduino.h"
    #include "Mbed.h"

    /**
     * @brief Basic init, legacy support with no device ID
     * 
     * @param dataPtr pointer to the data buffer
     * @param dataLength lenghts of the data buffer
     * @return int the init result.
     */
    int backupManager_ArduinoNfr52::init(void* dataPtr, size_t dataLength)
    {
        int res;

        // strcpy_s(kv_key_in, KEY_LENGTH, "/kv/config_device_v1");
        // keyInitReady = 1;

        // mLocalDataPtr = dataPtr;
        // mDataLenght = dataLength;
    
        // res = loadConfig();
  
        // if(res == MBED_ERROR_ITEM_NOT_FOUND )
        // {
        //     Serial.println("Key not found, need to do first init"); 
        //     res = clearMemory();
        //     return saveConfig();
        // }
        // else if(res == 0)
        // {
        //     Serial.println("Data loaded successfully"); 
        // }
        // else
        // {
        //     Serial.println("Init error"); 
        // }

        //return res; // return other error code

        return init(dataPtr, dataLength, 0, 0);

    }

    /**
     * @brief Initialization the the backup manager
     * 
     * @param dataPtr           pointer to the data buffer
     * @param dataLength        lenghts of the data buffer
     * @param deviceId          The device ID ( /see teamAt_deviceIDV1.0.h)
     * @param deviceUniqueId    the devie Unique ID
     * @return int the init result.
     */
    int backupManager_ArduinoNfr52::init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId)
    {
        int res;

        generateKey32(kv_key_in,deviceId,deviceUniqueId); //generate key and stor in kv_key_in


        keyInitReady = 1;

        mLocalDataPtr = dataPtr;
        mDataLenght = dataLength;
    
        res = loadConfig();
  
        if(res == MBED_ERROR_ITEM_NOT_FOUND )
        {
            //DEBUG Serial.println("Key not found, need to do first init"); 
            res = clearMemory();
            return saveConfig();
        }
        else if(res == 0)
        {
            //DEBUG Serial.println("Data loaded successfully"); 
        }
        else
        {
            //DEBUG Serial.println("Init error"); 
        }

        return res; // return other error code

    }


    int backupManager_ArduinoNfr52::loadConfig()
    {
        kv_info_t info;
        size_t actualSize;

        if(!keyInitReady)
        {
            return BACKUPMANAGER_ERROR_KEY_NOT_INIT;
        }

        int res = kv_get_info(kv_key_in, &info);
    
        if(res == 0)
        {
            //DEBUG Serial.print("load data -> "); 
            int res = kv_get(kv_key_in, mLocalDataPtr, info.size, &actualSize);
            //DEBUG Serial.println(MBED_GET_ERROR_CODE(res));
            return MBED_GET_ERROR_CODE(res);
        }

        return res;
    }


    int backupManager_ArduinoNfr52::saveConfig()
    {
        if(!keyInitReady)
        {
            return BACKUPMANAGER_ERROR_KEY_NOT_INIT;
        }

        //DEBUG Serial.print("save data -> ");
        int res = kv_set(kv_key_in, mLocalDataPtr, mDataLenght, 0);
        //DEBUG Serial.println(MBED_GET_ERROR_CODE(res));
        return MBED_GET_ERROR_CODE(res);
    }

    int backupManager_ArduinoNfr52::clearMemory()
    {
        
        //DEBUG Serial.println("kv_reset\n");
        int res = kv_reset("/kv/");
        return MBED_GET_ERROR_CODE(res);
    }

#endif