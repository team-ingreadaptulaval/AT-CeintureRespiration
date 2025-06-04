#ifndef BACKUPMANAGER_ARDUINO_MKRZERO_H
    #define BACKUPMANAGER_ARDUINO_MKRZERO_H

    

    #if ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
        || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
        || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
        || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
        || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )

        #include <Arduino.h>
        #include <FlashStorage.h>
        #include "backupManager_V1.0.h"
        

        #define BACKUPMANAGER_MAX_DATA_LENGTH 256
        #define KEY_LENGTH 32

        typedef struct{
            char key[KEY_LENGTH];
            uint8_t dataBytes[BACKUPMANAGER_MAX_DATA_LENGTH];

        }mkrZeroBackup_t;

        class backupManager_ArduinoMKRZERO: public backupManager
        {
            private:

            void *mLocalDataPtr;
            int mDataLenght = 0;
            bool keyInitReady = 0;

            mkrZeroBackup_t mBackupData;

            char kv_key_in[KEY_LENGTH] = {"/kv/config_device_v1"};

            public:

            /**
             * @brief   This functions connects the data ptr to be use for backups. Then, it check if data is alread present in 
             *          backup memory. If so, the data is loaded, if not, the memory is reset and the content of dataPtr is saved to 
             *          memory. For this reason dataPtr should be initialized with default values before calling this function.
             * 
             * @note legacy support version, use  init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId = 0;);
             * 
             * @param dataPtr       Backup data container (Set to default value before calling this function)
             * @param dataLength    Length of the container
             * 
             * @return int Error status
             */
            int init(void* dataPtr, size_t dataLength);

            /**
           * @brief   This functions connects the data ptr to be use for backups. Then, it check if data is alread present in 
             *          backup memory. If so, the data is loaded, if not, the memory is reset and the content of dataPtr is saved to 
             *          memory. For this reason dataPtr should be initialized with default values before calling this function.
             * 
             * @param dataPtr       Backup data container (Set to default value before calling this function)
             * @param dataLength    Length of the container
             * @param deviceId      Device ID
             * @param deviceUniqueId    Device unique ID, if not used use 0
             */
            init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId = 0;);

            /**
             * @brief Load config from backup memory and put it int he data container specified at init
             * 
             * @return int Error status
             */
            int loadConfig();

            /**
             * @brief Saved content of data container specified at init to backup memory
             * 
             * @return int Error status
             */
            int saveConfig();

            /**
             * @brief This function resets the backup memory
             * 
             * @return int Error status
             */
            int clearMemory();

        };

   #endif
#endif