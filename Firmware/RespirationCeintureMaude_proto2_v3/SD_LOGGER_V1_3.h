/**
 * @file SD_LOGGER_V1_3.h
 * @author Simon Latour
 * @brief Library to log data 
 * @version 1.3
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 * V1.0: Nouvelle version avec abstraction de la plateforme.
 * V1.1: Modification de la fonctionalité "listfiles" pour fonctionner avec le programme c++ sur l'ordinateur
 * V1.2: Modification de write, writeAscii et writeBinary poru supporte des listes de données.
 */


#ifndef SD_LOGGER_H
    #define SD_LOGGER_H

#include "sdLogger_systemCalls_V1_1.h"
#include <string>

    #define SDFORMAT_BINARY   1 //binary: change SDcartprint, readaccel, writeheader et structure.  
    #define SDFORMAT_ASCII    2 // ascii: change SD card print and SDPrintAccelGyro() et writeheader
    #define SDFORMAT_RAW      3 // Direct flash write, no fgile system

    #define logDataType_int         0
    #define logDataType_uInt        1
    #define logDataType_int64       2
    #define logDataType_uint64      3
    #define logDataType_float       4
    #define logDataType_char        5

	#define SDLOGGER_COMM_BUFFER_SIZE				1042
	#define SDLOGGER_COMM_DATA_TYPE_EMPTY			0x00
	#define SDLOGGER_COMM_DATA_TYPE_BINARYLOGDATA	0x01
	#define SDLOGGER_COMM_DATA_TYPE_MESSAGE_COUNT	0x02

    #define SDstructascii SDFORMAT_BINARY 

    // Log file base name.  Must be six characters or less.
    #define FILE_BASE_NAME "Data"
    #define SDLOGGER_NB_DECIMAL 4    //Define between 1 and 4 how many decimal the log files should

    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;

    #define SDLOGGER_MAX_FILENAME_LEN 13
    #define SDLOGGER_DECIMAL_MAX_LEN 4

    #if (BASE_NAME_SIZE + NB_LOG_DECIMAL + 4) > SDLOGGER_MAX_FILENAME_LEN

        #pragma error: File Lenght error
    #endif
 
    #define SDLOGGER_BINARY SDFORMAT_BINARY // To log data in binary mode. Faster but not human readable
    #define SDLOGGER_ASCII  SDFORMAT_ASCII  // To log data in ASCII (character) mode. Human readable but slower
    #define SDLOGGER_RAW  	SDFORMAT_RAW  // To log data in ASCII (character) mode. Human readable but slower

    #define SDLOGGER_VERBOSE_ON 1   // Print status on serial port (usefull for debug)
    #define SDLOGGER_VERBOSE_OFF 0  // Don't print status on serial port

	#define SDLOGGER_WRITEMULTI_BUFFER_SIZE 4500

    class sdlogger
    {
        private:

            char m_fileName[SDLOGGER_MAX_FILENAME_LEN];

            uint8_t m_verbose = 1;
            uint8_t m_initDone = 0;
            uint8_t m_loggerFormat = SDLOGGER_ASCII;
            uint8_t m_logInProgress = 0;
            uint32_t m_frequenc = 8;

            std::string _folderName;

            uint32_t m_dataPackageSize;
            uint32_t m_dataTypeCount;

            char (*m_dataTitlesTable)[32];

            uint8_t* 	m_dataTypesTable;

            void initFilename();
            void writeHeaderTitles();
            void writeHeaderTypes();

            void debugPrint(const char* outData);

        public:

            int init(uint32_t dataPackageSize, uint32_t dataTypeCount, char (*dataTitlesTable)[32], uint8_t* dataTypesTable, 
                                    uint8_t filetFormat = SDLOGGER_ASCII, std::string folderName = "LOG", uint8_t verboseOffOn = SDLOGGER_VERBOSE_OFF);

            uint8_t resumeNumbering();
            uint8_t startNewLog();
            void stopLog();
            uint8_t getLogStatus();
            uint8_t getActiveLogName(char *fileName);
            int getActiveLogNumber(void);
            
            uint8_t setDataFormat(uint8_t dataFormat);
            uint8_t getDataFormat();

            uint8_t write(const uint8_t *myData ,uint16_t size, uint32_t dataCount);
           // uint8_t writeBinary(const uint8_t *myData ,uint16_t size);
            uint8_t writeBinary(const uint8_t *myData,uint16_t dataSize, uint32_t dataCount);
            //uint8_t writeAscii(const uint8_t *myData,uint16_t size);
            uint8_t writeAscii(const uint8_t *inData,uint16_t size, uint32_t dataCount);
            uint8_t writeSerial(const uint8_t *inData, uint16_t dataSize, uint32_t dataCount);

            uint8_t printFileList(uint8_t timeStamp = 0);
            uint8_t printFile(const char* dataNumber);
            uint8_t downloadFile(const char* filename);
            uint8_t deleteSdCard();
            uint8_t deleteLogFolder();
            
            void setSerialVerbose(uint8_t verboseOffOn);

            uint32_t binToCharDataConvert(const uint8_t* binData, char* charData, uint8_t* dataTypesTable, uint32_t dataTypeCount);
        
    };

#endif
