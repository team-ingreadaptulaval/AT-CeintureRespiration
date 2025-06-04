


#if ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
    || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
    || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
    || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
    || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

    #include "backupManager_Arduino_MKRZERO_V1.0.h"

    #include <string.h>


    #include <FlashStorage.h> // MKRzero flash storage library

    FlashStorage(my_flash_store, mkrZeroBackup_t);

    int backupManager_ArduinoMKRZERO::init(void* dataPtr, size_t dataLength)
    {

        return backupManager_ArduinoMKRZERO::init(dataPtr,dataLength, 0, 0);

        // mLocalDataPtr = dataPtr;
        // mDataLenght = dataLength;

        // #ifdef DEBUG_PRINT 
        //     Serial.println("Loading configuration"); 
        // #endif

        // int res = loadConfig();
        // if(res == BACKUPMANAGER_ERROR_NOT_FOUND ) // key not found
        // {
        //     #ifdef DEBUG_PRINT 
        //         Serial.println("Key not found, need to do first init"); 
        //     #endif
        //     // Copy data because it was overwritten during read.
        //     strcpy(mBackupData.key,kv_key_in); // copy key in data structure
        //     memcpy(mBackupData.dataBytes, mLocalDataPtr,mDataLenght);

        //     saveConfig();
        // }
        // else
        // {
        //     #ifdef DEBUG_PRINT 
        //         Serial.println("Data loaded successfully"); 
        //     #endif
        // }
  

        // return BACKUPMANAGER_ERROR_NO_ERROR; // return other error code

    }

    int backupManager_ArduinoMKRZERO::init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId)
    {

        generateKey32(kv_key_in,deviceId,deviceUniqueId); //generate key and stor in kv_key_in
        keyInitReady = 1;

        mLocalDataPtr = dataPtr;
        mDataLenght = dataLength;

        #ifdef DEBUG_PRINT 
            Serial.println("Loading configuration"); 
        #endif

        int res = loadConfig();
        if(res == BACKUPMANAGER_ERROR_NOT_FOUND ) // key not found
        {
            #ifdef DEBUG_PRINT 
                Serial.println("Key not found, need to do first init"); 
            #endif
            // Copy data because it was overwritten during read.
            strcpy(mBackupData.key,kv_key_in); // copy key in data structure
            memcpy(mBackupData.dataBytes, mLocalDataPtr,mDataLenght);

            saveConfig();
        }
        else
        {
            #ifdef DEBUG_PRINT 
                Serial.println("Data loaded successfully"); 
            #endif
        }
  

        return BACKUPMANAGER_ERROR_NO_ERROR; // return other error code

    }


    int backupManager_ArduinoMKRZERO::loadConfig()
    {

        if(!keyInitReady)
        {
            return BACKUPMANAGER_ERROR_KEY_NOT_INIT;
        }

        mBackupData = my_flash_store.read();
 
        if(strcmp(mBackupData.key,kv_key_in)) // key not found
        {
            #ifdef DEBUG_PRINT 
                Serial.println("Key not found"); 
            #endif
            
            return BACKUPMANAGER_ERROR_NOT_FOUND;
        }
    
        #ifdef DEBUG_PRINT 
            Serial.println("Data loaded successfully"); 
        #endif
        memcpy(mLocalDataPtr,mBackupData.dataBytes,mDataLenght);
        
        return BACKUPMANAGER_ERROR_NO_ERROR;
    }


    int backupManager_ArduinoMKRZERO::saveConfig()
    {
        if(!keyInitReady)
        {
            return BACKUPMANAGER_ERROR_KEY_NOT_INIT;
        }
        memcpy(mBackupData.dataBytes, mLocalDataPtr,mDataLenght);
        my_flash_store.write(mBackupData);

        return BACKUPMANAGER_ERROR_NO_ERROR;
    }

    int backupManager_ArduinoMKRZERO::clearMemory()
    {

        memset(mBackupData.dataBytes,0xFF,mDataLenght);
        memset(mBackupData.key,0xFF,KEY_LENGTH);

        return BACKUPMANAGER_ERROR_NO_ERROR;
    }

#endif