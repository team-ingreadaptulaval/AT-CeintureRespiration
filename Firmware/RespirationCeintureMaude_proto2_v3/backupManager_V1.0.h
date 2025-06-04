/**
 * @file flashConfigBackup.h
 * @author Simon Latour
 * @brief   Program Flash backup 
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FLASHCONFIGBACKUP_H
#define FLASHCONFIGBACKUP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "teamtAt_deviceID_V1.0.h"
#define BACKUPMANAGER_ERROR_NO_ERROR        0
#define BACKUPMANAGER_ERROR_NOT_FOUND       1
#define BACKUPMANAGER_ERROR_KEY_NOT_INIT    2



class backupManager{

    private:

    public:

    //flashDataStruct configData;

    /**
     * @brief   This functions connects the data ptr to be use for backups. Then, it check if data is alread present in 
     *          backup memory. If so, the data is loaded, if not, the memory is reset and the content of dataPtr is saved to 
     *          memory. For this reason dataPtr should be initialized with default values before calling this function.
     * 
     * @param dataPtr       Backup data container
     * @param dataLength    Length of the container
     * 
     * @return int          Error status
     */
    virtual int init(void* dataPtr, size_t dataLength);

    
    /**
     * @brief 
     * 
     * @param dataPtr       Backup data container
     * @param dataLength    Length of the container
     * @param deviceId      Device indentification number ( Client, product, version)
     * @param Id            Optional Device unique ID is a 64 bit number unique to a device usefull to protect data to switche between devices. 0 when not used
     * @return int          Error status
     */
    virtual int init(void* dataPtr, size_t dataLength, unsigned long deviceId, unsigned long long deviceUniqueId = 0);


    /**
     * @brief Load config from backup memory and put it int he data container specified at init
     * 
     * @return int Error status
     */
    virtual int loadConfig();

    /**
     * @brief Saved content of data container specified at init to backup memory
     * 
     * @return int Error status
     */
    virtual int saveConfig();

};


#endif
