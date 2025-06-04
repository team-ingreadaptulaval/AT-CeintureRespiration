 /* sdLogger_systemCalls_V1_0.h
 *
 *  Created on: 6 déc. 2021
 *      Author: teamat
 */

#ifndef TEAMATLIBS_SDLOGGER_SYSTEMCALLS_V1_1_H_
#define TEAMATLIBS_SDLOGGER_SYSTEMCALLS_V1_1_H_

#include "stdint.h"
#include "stdbool.h"


  // Error Codes
    #define SDLOGGER_ERROR_NO_ERROR         0
    #define SDLOGGER_ERROR_SDBEGIN          1
    #define SDLOGGER_ERROR_SDOPEN           2
    #define SDLOGGER_ERROR_LINEERROR        3
    #define SDLOGGER_ERROR_ROOTOPEN         4
    #define SDLOGGER_ERROR_OPENNEXT         5
    #define SDLOGGER_ERROR_FILECREATE       6
    #define SDLOGGER_ERROR_FILEOPEN         7
    #define SDLOGGER_ERROR_NAMELENGHT       8
    #define SDLOGGER_ERROR_NOT_INITIALIZED  9
    #define SDLOGGER_ERROR_LOGNOTOPEN       10
    #define SDLOGGER_ERROR_LOGINPROGRESS    11
    #define SDLOGGER_ERROR_FORMAT           12
    #define SDLOGGER_ERROR_DELETE           13
    #define SDLOGGER_ERROR_CHANGEDIR        14
	#define SDLOGGER_ERROR_WRITE	        15
	#define SDLOGGER_ERROR_SDCLOSE          16
	#define SDLOGGER_ERROR_BUFFER_SIZE		17

	#define SDLOGGER_ERROR_COMM_TIMEOUT     20

	#define SDLOGGER_ERROR_GENERAL          255


/**
 * @fn void sdLogger_delay(uint32_t)
 * @brief System delay function
 *
 * @param delayValue in ms
 */
void sdLogger_delay(uint32_t delayValue);

/**
 * @fn void sdLogger_serialOut(const char*)
 * @brief System serial out function
 *
 * @param outData string to print
 */
void sdLogger_serialOut(const char* outData);

void sdLogger_serialDataOut(const uint8_t* outData, uint32_t dataLen);


bool sdLogger_serialGetNewLineRx();
void sdLogger_clearNewLineRx();


/**
 * @fn uint8_t sdLogger_initSystem(uint8_t)
 * @brief Log file system setup
 *
 * @param verboseOffOn ton enable/disable the printing of debug message / info
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_initSystem(uint8_t verboseOffOn);

/**
 * @fn bool sdLogger_initBackupMedia()
 * @brief if backup media needs init/mount/...
 *
 * @return true is success
 */
bool sdLogger_initBackupMedia();

/**
 * @fn uint8_t sdLogger_openWriteFile(const char*)
 * @brief Open file for writing
 *
 * @param fileName name of file
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_openWriteFile(const char* fileName);

/**
 * @fn uint8_t sdLogger_writeFile(const uint8_t*, uint32_t)
 * @brief Write bytes to opened write file
 *
 * @param data data to write
 * @param size of data
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_writeFile(const uint8_t* data, uint32_t size);

/**
 * @fn uint8_t sdLogger_writeFile(const char*, uint32_t)
 * @brief write character string to opened write file
 *
 * @param data data to write
 * @param size of data
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_writeFile(const char* data, uint32_t size);

/**
 * @fn uint8_t sdLogger_closeWriteFile()
 * @brief close the opened write file
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_closeWriteFile();

/**
 * @fn bool sdLogger_openReadFile(const char*)
 * @brief open fil for reading
 *
 * @param fileName name of the file to open
 * @return true if success
 */
bool sdLogger_openReadFile(const char* fileName);

/**
 * @fn uint32_t sdLogger_readFile(uint8_t*, uint32_t)
 * @brief read data from file
 *
 * @param data pointer to the storage buffer [out]
 * @param size Size of the data to read
 * @return number of bytes read
 */
uint32_t sdLogger_readFile(uint8_t* data, uint32_t size);

/**
 * @fn uint32_t sdLogger_readFileString(char*, uint32_t, char='\0')
 * @brief read character string from file
 *
 * @param data data pointer to the storage buffer [out]
 * @param maxSize Size of the data to read
 * @param delimiter the end string delimiter. \0 by defaut but can be change for \n or any character
 * @return nb of characters read
 */
uint32_t sdLogger_readFileString(char* data, uint32_t maxSize, char delimiter = '\0');

/**
 * @fn uint8_t sdLogger_closeReadFile()
 * @brief close the opened read file.
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_closeReadFile();

/**
 * @fn bool sdLogger_fileExists(const char*)
 * @brief check if fil exists
 *
 * @param fileName name of the file [in]
 * @return true if file exists
 */
bool sdLogger_fileExists(const char* fileName);

/**
 * @fn uint8_t sdLogger_goToRoot()
 * @brief chang to root directory
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_goToRoot();

/**
 * @fn uint8_t sdLogger_createFolder(const char*)
 * @brief Creat a folder in the actual working folder
 *
 * @param folderName name of the folder [in]
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_createFolder(const char* folderName);

/**
 * @fn uint8_t sdLogger_deleteFolder(const char*)
 * @brief Delete a folder
 *
 * @param folderName name of the folder [in]
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_deleteFolder(const char* folderName);

/**
 * @fn uint8_t sdLogger_deleteAll()
 * @brief Format media
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_deleteAll();

/**
 * @fn uint8_t sdLogger_gotoFolder(const char*)
 * @brief Change active directory
 *
 * @param folderName name of the folder [in]
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_gotoFolder(const char* folderName);

/**
 * @fn uint8_t sdLogger_getCurrentFolder(char*, uint32_t)
 * @brief Ge current working folder
 *
 * @param folderName returned folder name [out]
 * @param maxLen Max lenght for the receiving buffer
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_getCurrentFolder(char* folderName, uint32_t maxLen);

/**
 * @fn uint8_t sdLogger_deleteFiles()
 * @brief Delete all files in folder
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_deleteFiles();

/**
 * @fn uint8_t sdLogger_printFileList(const char*, uint8_t)
 * @brief Display the list of all files in folder
 *
 * @param folderName name of the folder [in]
 * @param displayTimeStamp set to 1 to print file's timestamp
 *
 * @return error status: see SDLOGGER_ERROR_
 */
uint8_t sdLogger_printFileList(const char* folderName,uint8_t displayTimeStamp);

#endif /* TEAMATLIBS_SDLOGGER_SYSTEMCALLS_V1_1_H_ */
