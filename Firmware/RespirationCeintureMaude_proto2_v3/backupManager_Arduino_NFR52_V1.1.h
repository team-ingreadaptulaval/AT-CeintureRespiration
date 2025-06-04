#ifndef BACKUPMANAGER_ARDUINO_NFR52_H
#define BACKUPMANAGER_ARDUINO_NFR52_H


/**
 * @file backupManager_Arduino_NFR52_V1.0.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 * 
 * 
 */
//TODO Passer la key en param pour pouvoir avoir des config évolutive par projets 

#include "backupManager_V1.0.h"

#define KEY_LENGTH 32

class backupManager_ArduinoNfr52: public backupManager
{
    private:

     void *mLocalDataPtr;
     int mDataLenght = 0;
     bool keyInitReady = 0;

     char kv_key_in[KEY_LENGTH] = {"/kv/config_device_v1"};


    public:

    /**
     * @brief   This functions connects the data ptr to be use for backups. Then, it check if data is alread present in 
     *          backup memory. If so, the data is loaded, if not, the memory is reset and the content of dataPtr is saved to 
     *          memory. For this reason dataPtr should be initialized with default values before calling this function.
     * 
     * @param dataPtr       Backup data container (Set to default value before calling this function)
     * @param dataLength    Length of the container
     * 
     * @return int Error status
     */
    int init(void* dataPtr, size_t dataLength);

     /**
     * @brief 
     * 
     * @param dataPtr       Backup data container
     * @param dataLength    Length of the container
     * @param deviceId      Device indentification number ( Client, product, version)
     * @param Id            Optional Device unique ID is a 64 bit number unique to a device usefull to protect data to switche between devices. 0 when not used
     * @return int 
     */
    int init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId = 0);
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