//using namespace std; 

#include "sdLogger_systemCalls_V1_1.h"
#include "SD_LOGGER_V1_3.h"
#include "teamATbasic_V1_1.h"
#include "string.h"

#include "atComm.h"
#include "at_plaformAbstraction_V1_1.h"
//TEMPORAIRE enlever
//#include "stm32l4xx_hal.h"
   

/**
 * @brief This function needs to be called first to initialize the loger
 * 
 * @param chipSelectPin The chip select pin for the sd card
 * @param frequencymhz  The SPI frequency
 * @param serialOutFunc Callback to a function thats prints onm the serial port.
 * @return int  status
 */

int sdlogger::init(uint32_t dataPackageSize, uint32_t dataTypeCount, char (*dataTitlesTable)[32], uint8_t* dataTypesTable, 
                                    uint8_t filetFormat, std::string folderName, uint8_t verboseOffOn)
{
   
   	m_dataPackageSize = dataPackageSize;
	m_dataTypeCount = dataTypeCount;

	m_dataTitlesTable = dataTitlesTable;
	m_dataTypesTable = dataTypesTable;

    m_loggerFormat = filetFormat;

    m_verbose = verboseOffOn;
    _folderName = "/" + folderName;

    uint8_t res = sdLogger_initSystem(m_verbose);
    if (SDLOGGER_ERROR_NO_ERROR == res)
    {
    	m_initDone = 1;

    	debugPrint("\n\r\tFolder name: ");
		debugPrint(_folderName.c_str());
		debugPrint("\n\r ");
		initFilename();

		return SDLOGGER_ERROR_NO_ERROR;
    }

    return (int)res;

}


/**
 * @brief Allows to turn on/of verbose mode on serial port 
 * 
 * Verbose on will share text information and status in ahuma readable format.
 * By default, Verbose is on.
 * 
 * @param verboseOffOn 1 = on, 2 = off
 */
void sdlogger::setSerialVerbose(uint8_t verboseOffOn)
{
    m_verbose = verboseOffOn;
}

/**
 * @brief This functions inits the filename with the correct extension and number
 *        depending on the format of the data
 * 
 */
void sdlogger::initFilename()
{
    char number[SDLOGGER_NB_DECIMAL];

    for(int i = 0; i<SDLOGGER_NB_DECIMAL; i++)
    {
        number[i] = '0';
    }

    strcpy(m_fileName,"Data");
    strcpy(&m_fileName[BASE_NAME_SIZE],number);

    if(m_loggerFormat == SDLOGGER_BINARY)
    {
        strcpy(&m_fileName[BASE_NAME_SIZE + SDLOGGER_NB_DECIMAL],".dat");
    }
    else
    {
        strcpy(&m_fileName[BASE_NAME_SIZE + SDLOGGER_NB_DECIMAL],".csv");
    }
}

/**
 * @brief Sets the format for the file on the SD Card
 * 
 * @param dataFormat The format  (SDLOGGER_BINARY  / SDLOGGER_ASCII )
 * @return uint8_t error status
 */
uint8_t sdlogger::setDataFormat(uint8_t dataFormat)
{
    if( m_loggerFormat == SDLOGGER_BINARY || m_loggerFormat == SDLOGGER_ASCII)
    {
        m_loggerFormat = dataFormat;

        return SDLOGGER_ERROR_NO_ERROR;
    }
    
    return SDLOGGER_ERROR_FORMAT;
    
}

/**
 * @brief Thsi functions returns the actif file format for the log (SDLOGGER_BINARY  / SDLOGGER_ASCII )
 * 
 * @return uint8_t 
 */
uint8_t sdlogger::getDataFormat()
{
    return m_loggerFormat;
}

/**
 * @brief Write Data to the log, the format ( binary / ascii) will be applied base on the active 
 *        settigns of the sdLogger
 *        @see setDataFormat()
 * 
 * @param myData pointer to the data to write
 * @param size  the syze of the data in bytes
 * 
 * @return error status
 */
uint8_t sdlogger::write(const uint8_t *myData ,uint16_t size, uint32_t dataCount)
{
    if( m_loggerFormat == SDLOGGER_BINARY)
    {
        return writeBinary(myData,size,dataCount);
    }else if (m_loggerFormat == SDLOGGER_ASCII)
    {
        return writeAscii(myData,size,dataCount);
    }
    
    return SDLOGGER_ERROR_FORMAT;
}


///**
// * @brief Write Data to the log forcing the binary mode
// *
// * @param myData pointer to the data to write
// * @param size  the syze of the data in bytes
// *
// * @return error status
// */
//uint8_t sdlogger::writeBinary(const uint8_t *myData,uint16_t size)
//{
//
//    if(!m_initDone)
//    {
//        return SDLOGGER_ERROR_NOT_INITIALIZED;
//    }
//
//    if(!m_logInProgress)
//    {
//        return SDLOGGER_ERROR_LOGNOTOPEN;
//    }
//
//    sdLogger_writeFile(myData, size);
//
//    return SDLOGGER_ERROR_NO_ERROR;
//}


/**
 * @brief Write Data to the log forcing the binary mode
 *
 * @param myData 	pointer to the data to write
 * @param dataSize  the size of the data in bytes
 * @param dataCount the number of data
 *
 * @return error status
 */
uint8_t sdlogger::writeBinary(const uint8_t *myData,uint16_t dataSize, uint32_t dataCount)
{

    if(!m_initDone)
    {
        return SDLOGGER_ERROR_NOT_INITIALIZED;
    }

    if(!m_logInProgress)
    {
        return SDLOGGER_ERROR_LOGNOTOPEN;
    }

    sdLogger_writeFile(myData, dataSize * dataCount); ///// ICI : Ne pas oublier de rechanger

    return SDLOGGER_ERROR_NO_ERROR;
}


///**
// * @brief Write Data to the log, forcing the ascii mode
// *
// * @param myData pointer to the binary data to write into ascii format
// * @param size  the size of the data in bytes
// *
// * Note: If a binarydata-to-char callback function has not been conencted using   assignBinaryToCharConverter(),
// * the function will simply send data as-is with an added '\n'
// *
// * @return error status
// */
//uint8_t sdlogger::writeAscii(const uint8_t *myData,uint16_t size)
//{
//
//    if(!m_initDone)
//    {
//        return SDLOGGER_ERROR_NOT_INITIALIZED;
//    }
//
//    if(!m_logInProgress)
//    {
//        return SDLOGGER_ERROR_LOGNOTOPEN;
//    }
//
//    uint32_t charLength;
//    char myDataInChar[1000];
//
//    charLength = binToCharDataConvert(myData, myDataInChar, m_dataTypesTable, m_dataTypeCount);
//
//    myDataInChar[charLength] = '\n';
//    charLength++;
//    myDataInChar[charLength] = '\0';
//
//
//    sdLogger_writeFile((uint8_t*)myDataInChar, charLength);
//    return SDLOGGER_ERROR_NO_ERROR;
//}



/**
 * @brief Write Data to the log, forcing the ascii mode
 *
 * @param myData pointer to the binary data to write into ascii format
 * @param size  the size of the data in bytes
 * @param dataCount is the number of data to save.
 *
 * Note: If a binarydata-to-char callback function has not been connected using   assignBinaryToCharConverter(),
 * the function will simply send data as-is with an added '\n'
 *
 * @return error status
 */
uint8_t sdlogger::writeAscii(const uint8_t *inData, uint16_t dataSize, uint32_t dataCount)
{

    if(!m_initDone)
    {
        return SDLOGGER_ERROR_NOT_INITIALIZED;
    }

    if(!m_logInProgress)
    {
        return SDLOGGER_ERROR_LOGNOTOPEN;
    }

    if(dataCount * dataSize >= SDLOGGER_WRITEMULTI_BUFFER_SIZE)
    {
    	return SDLOGGER_ERROR_BUFFER_SIZE;
    }

    uint32_t charLength;
    char myDataInChar[1000];
    int dataIndex = 0;

    for(uint32_t i = 0 ; i < dataCount; i++)
    {
    	charLength = binToCharDataConvert(&inData[i*dataSize], &(myDataInChar[dataIndex]), m_dataTypesTable, m_dataTypeCount);
        myDataInChar[dataIndex + charLength] = '\n';
        charLength++;
        dataIndex += charLength;
    }
    myDataInChar[dataIndex] = '\0';
    dataIndex++;

    sdLogger_writeFile((uint8_t*)myDataInChar, dataIndex);
    return SDLOGGER_ERROR_NO_ERROR;
}

/**
 * @brief Write Data to the serial port, forcing the ascii mode
 *
 * @param myData pointer to the binary data to write into ascii format
 * @param size  the size of the data in bytes
 * @param dataCount is the number of data to save.
 *
 * Note: If a binarydata-to-char callback function has not been connected using assignBinaryToCharConverter(),
 * the function will simply send data as-is with an added '\n'
 *
 * @return error status
 */
uint8_t sdlogger::writeSerial(const uint8_t *inData, uint16_t dataSize, uint32_t dataCount)
{

    if(!m_initDone)
    {
        return SDLOGGER_ERROR_NOT_INITIALIZED;
    }

    if(dataCount * dataSize >= SDLOGGER_WRITEMULTI_BUFFER_SIZE)
    {
    	return SDLOGGER_ERROR_BUFFER_SIZE;
    }

    uint32_t charLength;
    char myDataInChar[1000];
    int dataIndex = 0;

    for(uint32_t i = 0 ; i < dataCount; i++)
    {
    	charLength = binToCharDataConvert(&inData[i*dataSize], &(myDataInChar[dataIndex]), m_dataTypesTable, m_dataTypeCount);
        myDataInChar[dataIndex + charLength] = '\n';
        charLength++;
        dataIndex += charLength;
    }
    myDataInChar[dataIndex] = '\0';
    dataIndex++;

    sdLogger_serialOut(myDataInChar);
    return SDLOGGER_ERROR_NO_ERROR;
}


/**
 * @brief This function closes the active log file
 * 
 */
void sdlogger::stopLog()
{
    if(m_verbose)
    {
        sdLogger_serialOut("\n Closing file: ");
        sdLogger_serialOut(m_fileName);
        sdLogger_serialOut("\r\n");
    }

    if(m_logInProgress){
        sdLogger_closeWriteFile();
        m_logInProgress = 0;
    }

}

uint8_t sdlogger::resumeNumbering()
{
	if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

   initFilename();

    debugPrint("\n\r\tResuming file numbering\r\n");

    if (!sdLogger_initBackupMedia()) 
    {
        debugPrint("\n\r\tProblem with SD card");
        debugPrint("\r\n");

        return SDLOGGER_ERROR_SDBEGIN;
    }else
    {
        debugPrint("\n\r\tBegin success");
        debugPrint("\r\n");
    }
        
    //Find an unused file name.
    if (BASE_NAME_SIZE > 6) 
    {
        return SDLOGGER_ERROR_NAMELENGHT;
    }

    // Create a new folder.
    sdLogger_createFolder(_folderName.c_str());

    sdLogger_goToRoot();

    sdLogger_gotoFolder(_folderName.c_str());
    
    while (sdLogger_fileExists(m_fileName))
    {
    	debugPrint("\n\r\t-->");
        debugPrint(m_fileName);
        debugPrint(" trouvé.");

        uint32_t fileNumber = arrayToUint(&m_fileName[BASE_NAME_SIZE],SDLOGGER_NB_DECIMAL );
        
        if(!uintToArray(fileNumber+1,&m_fileName[BASE_NAME_SIZE],SDLOGGER_NB_DECIMAL))
        {
            debugPrint("\n\r\tMaximum log files reached");
            debugPrint("\r\n");
            
            return SDLOGGER_ERROR_FILECREATE;
        }
     }

    return SDLOGGER_ERROR_NO_ERROR;
}
/**
 * @brief This function creates and open a new log file
 * 
 * @return uint8_t 
 */
uint8_t sdlogger::startNewLog()
{   

	if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

    // If a log is already in progress, make sur to close previous file befor startign a new log.
    if(m_logInProgress)
    {
        return SDLOGGER_ERROR_LOGINPROGRESS;
    }
          
    if ( !sdLogger_initBackupMedia() ) {

        debugPrint("\n\r\tProblem with SD card");
        debugPrint("\r\n");

        return SDLOGGER_ERROR_SDBEGIN;
    }else
    {
        debugPrint("\n\r\tBegin success");
        debugPrint("\r\n");
    }
        
    //Find an unused file name.
    if (BASE_NAME_SIZE > 6) 
    {
        return SDLOGGER_ERROR_NAMELENGHT;
    } 
    
    // Create a new folder.
    sdLogger_createFolder(_folderName.c_str());

    //sdLogger_goToRoot(); // temporaire, pour test
    if(sdLogger_gotoFolder(_folderName.c_str()))
    {
         debugPrint("\n\r\tchdir to log folder failed.");

         return SDLOGGER_ERROR_CHANGEDIR;
    }
    else
    {
        debugPrint("\n\r\tchdir to log folder success.");
    }

     while (sdLogger_fileExists(m_fileName))
     {
        debugPrint(m_fileName);
        debugPrint("\n\r\tAlready exists");

        uint32_t fileNumber = arrayToUint(&m_fileName[BASE_NAME_SIZE],SDLOGGER_NB_DECIMAL );
        
        if(!uintToArray(fileNumber+1,&m_fileName[BASE_NAME_SIZE],SDLOGGER_NB_DECIMAL))
        {
            debugPrint("\n\r\tMaximum log files reached");

            return SDLOGGER_ERROR_FILECREATE;
        }
     }


     debugPrint("\n\r\tFilename verif done.");

    if(sdLogger_openWriteFile(m_fileName))
    {
        debugPrint("\n\r\tnError opening file");

        return SDLOGGER_ERROR_SDOPEN;
    }
    else
    {
        debugPrint("\n\r\tFile opened: ");
        debugPrint(m_fileName);
        debugPrint("\r\n");
    }
    
    debugPrint("\n\r\tStarting File header\r\n");

    if(m_loggerFormat == SDLOGGER_BINARY)
    {  
        writeHeaderTypes();  
    }
    else
    {
        writeHeaderTitles();
    }

    debugPrint("\n\r\tFile header written \r\n");

    m_logInProgress = 1;
    return SDLOGGER_ERROR_NO_ERROR;

}

/**
 * @brief Get the status of the log to know if a log is currently opened / in progress
 * 
 * @return uint8_t 1 if log in progress, 0 if not
 */
uint8_t sdlogger::getLogStatus()
{
    return m_logInProgress;
}

/**
 * @brief Returns the name of the Active log, if log is active
 * 
 * @param fileName Pointer to the buffer where to copy the name
 * @return uint8_t 1 if log is active, 0 if no log activer
 */
uint8_t sdlogger::getActiveLogName(char *fileName)
{
    if(m_logInProgress)
    {
        strcpy(fileName,m_fileName);
        return 1;
    }
        
    return 0;
}

//strcpy(&m_fileName[BASE_NAME_SIZE + SDLOGGER_NB_DECIMAL],".dat");
/**
 * @brief Returns the numbner of the Active log, if log is active
 * 
 * @param fileName Pointer to the buffer where to copy the name
 * @return uint8_t 1 if log is active, 0 if no log activer
 */
int sdlogger::getActiveLogNumber(void)
{
    if(m_logInProgress)
    {
        uint32_t fileNumber = arrayToUint(&m_fileName[BASE_NAME_SIZE],SDLOGGER_NB_DECIMAL );
        return (int)fileNumber;
    }
        
    return -1;
}

/**
 * @brief This function takes the data from a file and prints it on the serial port.
 * 
 * @param fileName The name of the file to print
 * @return uint8_t status
 */
uint8_t sdlogger::printFile(const char* fileName)
{

    uint32_t dataStructSize;
    uint32_t datacount;
    uint8_t dataTypesTable[50];

    uint8_t fileFormat;
    


    if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

    // Détection du format en utilisant l'extension des fichiers
    if(strstr (fileName,".csv"))
    {
        fileFormat = SDLOGGER_ASCII;
    }
    else if(strstr (fileName,".dat"))
    {
        fileFormat = SDLOGGER_BINARY; 
    }
    else{
        debugPrint("\n\r\tUnknown file format...\n\r");
        return SDLOGGER_ERROR_FORMAT;
    } 

    if (!sdLogger_initBackupMedia()) 
    {
        debugPrint("\n\r\tProblem with SD card");
        debugPrint("\r\n");
        return SDLOGGER_ERROR_SDBEGIN;
    }
  
    std::string fullPath = std::string(_folderName.c_str()) + std::string("/") + std::string(fileName);
    
    debugPrint("\n\r\tFile to open:[");
    debugPrint(fullPath.c_str());
    debugPrint("]\r\n");

    if(!sdLogger_openReadFile(fullPath.c_str()))
    {
        debugPrint("\n\r\tFile Not found");
        debugPrint("\r\n");
        return SDLOGGER_ERROR_SDOPEN;
    }
    else
    {
        //int ln = 1;
        size_t n;
        const size_t LINE_DIM = 1000;
        char line[LINE_DIM];

        //Read Binary header
        if(fileFormat == SDLOGGER_BINARY)
        {
           
            if(sdLogger_readFile((uint8_t*)&dataStructSize, 4)) { // Read data structure size
                if(sdLogger_readFile((uint8_t*)&datacount, 4)) { // Read type data count
                    sdLogger_readFile(dataTypesTable, datacount); // load data types table
                    
                    for(uint32_t ii = 0 ; ii< datacount; ii++)
                    {
                        char titleName[50];
                        int readLen = sdLogger_readFileString(titleName, 50,'\n');
                        if(readLen)
                        {                      
                            switch(dataTypesTable[ii])        {
                                case logDataType_float:
                                    sprintf(&titleName[readLen-1],"(float),");
                                    break;
                
                                case logDataType_int:
                                    sprintf(&titleName[readLen-1],"(int),");
                                    break;

                                case logDataType_uInt:
                                    sprintf(&titleName[readLen-1],"(uint),");
                                    break;

                                case logDataType_char:
                                    sprintf(&titleName[readLen-1],"(char),");
                                    break;

                                case logDataType_int64:
                                    sprintf(&titleName[readLen-1],"(int64),");
                                    break;
                                case logDataType_uint64:
                                    sprintf(&titleName[readLen-1],"(uint64),");
                                    break;

                                default:
                                    sprintf(&titleName[readLen-1],"(?),");
                                    break;
                            } 
                        }   

                        sdLogger_serialOut(titleName);
                    }
                    char serialdata[200];
                    sprintf(serialdata,"\n\r\tStucture size:%lu, data types: %lu",dataStructSize, datacount);
                    debugPrint(serialdata);
                    debugPrint("\n\r");
                }
            }
        
        }

        bool datatoReadinFile = true;
        //while (sdLogger_datatoReadinFile())
        while(datatoReadinFile)
        {
            char serialaccum[1000];

            if(fileFormat == SDLOGGER_BINARY)
            {

                uint8_t dataRead[1000];
                //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_SET);
                if(sdLogger_readFile(dataRead, dataStructSize)){
                	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_RESET);
                	// Transfor binary data in a human readable data
                	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_SET);
					binToCharDataConvert(dataRead,serialaccum, dataTypesTable, datacount);
					//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_RESET);

					//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_SET);
					sdLogger_serialOut(serialaccum);
					sdLogger_serialOut("\n\r");
					//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_RESET);
                }
                else
                {
                	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PinState::GPIO_PIN_RESET);
                	datatoReadinFile = false;
                }

            }else
            {
                if((n = sdLogger_readFileString(line, sizeof(line),'\n')) > 0) 
                {
                    if (line[n - 1] == '\n') 
                    {
                        line[n-1] = 0;
                        sdLogger_serialOut(line);
                        sdLogger_serialOut("\n\r");
                    } 
                    else 
                    {
                        // no '\n' - line too long or missing '\n' at EOF
                        debugPrint("\n\r\t<-- Line too long or missing nl\n\r");
                        return SDLOGGER_ERROR_LINEERROR;
                    }
                }
                else
                {
                	datatoReadinFile = false;
                }
            }
           
        }                
                  
        //sdfileRead.close();
        sdLogger_closeReadFile();
    }

    
    return SDLOGGER_ERROR_NO_ERROR;
}


uint8_t sdlogger::downloadFile(const char* fileName)
{


    uint32_t dataStructSize;
    uint32_t datacount;
    uint8_t dataTypesTable[50];

    uint8_t fileFormat;

    if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

    // Détection du format en utilisant l'extension des fichiers
    if(strstr (fileName,".dat"))
    {
        fileFormat = SDLOGGER_BINARY;
    }
    else{
        debugPrint("\n\rOnly .dat data supported for download.\n\r");
        return SDLOGGER_ERROR_FORMAT;
    }

    if (!sdLogger_initBackupMedia())
    {
        debugPrint("\n\r\tProblem with SD card");
        debugPrint("\r\n");
        return SDLOGGER_ERROR_SDBEGIN;
    }

    std::string fullPath = std::string(_folderName.c_str()) + std::string("/") + std::string(fileName);

    if(!sdLogger_openReadFile(fullPath.c_str()))
    {
        debugPrint("\n\r\tFile Not found");
        debugPrint("\r\n");
        return SDLOGGER_ERROR_SDOPEN;
    }
    else
    {

//        size_t n;
//        const size_t LINE_DIM = 5000;
//        char line[LINE_DIM];

        bool datatoReadinFile = true;
        uint32_t bytesRead;

        atComm comm(SDLOGGER_COMM_BUFFER_SIZE);
        uint32_t byteCount = 0;
        uint32_t packetNumber = 0;

        bool ackReceived = false;
        uint32_t timeoutTime = atGetSysTick_ms();
        int retryCount = 0;

        while(datatoReadinFile)
        {
            if(fileFormat == SDLOGGER_BINARY)
            {
                uint8_t dataRead[1000];
                uint8_t serialaccum[SDLOGGER_COMM_BUFFER_SIZE];
                bytesRead = sdLogger_readFile(dataRead, 1000);

                comm.resetBuffer();
                comm.startNewMessage(0, 0);
                comm.addData(SDLOGGER_COMM_DATA_TYPE_MESSAGE_COUNT, sizeof(packetNumber), &packetNumber);

                if(bytesRead)
                {
                	byteCount += bytesRead;
                	comm.addData(SDLOGGER_COMM_DATA_TYPE_BINARYLOGDATA, bytesRead, dataRead);
                }
                else
                {
					comm.setLastPacketStatus();
                	datatoReadinFile = false;
                }

                comm.completeMessage();
                ackReceived = false;
				bytesRead = comm.getSendPacket(serialaccum, SDLOGGER_COMM_BUFFER_SIZE);
				sdLogger_serialDataOut(serialaccum,bytesRead);
				retryCount = 0;

				timeoutTime = atGetSysTick_ms();

				while(!ackReceived)
				{
					if(sdLogger_serialGetNewLineRx())
					{
						ackReceived = true;
						sdLogger_clearNewLineRx();
						packetNumber++;
					}
					else
					{
						if((atGetSysTick_ms() - timeoutTime) >= 200)
						{
							retryCount++;
							if(retryCount < 3)
							{
								sdLogger_serialDataOut(serialaccum,bytesRead);
								timeoutTime = atGetSysTick_ms();
							}
							else
							{
								// Error abort
								return SDLOGGER_ERROR_COMM_TIMEOUT;
							}

						}
					}

				}


            }

        }
        sdLogger_closeReadFile();
    }


    return SDLOGGER_ERROR_NO_ERROR;

}

/**
 * @brief This functions erase the log folder only
 * 
 * @return uint8_t error status
 */
uint8_t sdlogger::deleteLogFolder()
{

	if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

    if (!sdLogger_initBackupMedia()) 
    {
        debugPrint("\n\r\tProblem with SD card\r\n");
        return SDLOGGER_ERROR_SDBEGIN;
    }
    uint8_t errorStatus;

    if(( errorStatus = sdLogger_goToRoot())){

        debugPrint("\n\r\tchdir to root failed.\n\r");
        return errorStatus;
    } 
    if((errorStatus = sdLogger_deleteFolder( _folderName.c_str()))){
        debugPrint("\n\r\terror deleting folder\n\r");
        return errorStatus;
    }  

    return resumeNumbering();

    //return SDLOGGER_ERROR_NO_ERROR;
}

/**
 * @brief This functions erase the entire content of an the SD Card
 * 
 * @return uint8_t error status
 */
uint8_t sdlogger::deleteSdCard()
{

//	if(!m_initDone)
//	{
//		return SDLOGGER_ERROR_NOT_INITIALIZED;
//	}
//
//    if (!sdLogger_initBackupMedia())
//    {
//        debugPrint("\n\r\tProblem with SD card\r\n");
//
//        return SDLOGGER_ERROR_SDBEGIN;
//    }
 
    return sdLogger_deleteAll();
}



/**
 * @brief This functions prints the list of the data files on the SD Card
 * 
 * @param timeStamp Set to 1 to include the file timestamp
 * @return uint8_t 
 */
uint8_t sdlogger::printFileList(uint8_t displayTimeStamp)
{
	if(!m_initDone)
	{
		return SDLOGGER_ERROR_NOT_INITIALIZED;
	}

    
    if (!sdLogger_initBackupMedia()) 
    {

        debugPrint("\n\r\tProblem with SD card\r\n");

        return SDLOGGER_ERROR_SDBEGIN;
    }
    
    return sdLogger_printFileList(_folderName.c_str(),displayTimeStamp);

}


void sdlogger::writeHeaderTitles()
{   

	if(m_initDone)
	{
		char dataToWrite[50];
		  int datalen;
		  for(uint32_t i = 0; i < m_dataTypeCount ; i++)
		  {
		      datalen = sprintf(dataToWrite,m_dataTitlesTable[i]);
		      sdLogger_writeFile((uint8_t*)dataToWrite,datalen);
		      datalen = sprintf(dataToWrite,",");
		      sdLogger_writeFile((uint8_t*)dataToWrite,datalen);
		  }
		  sdLogger_writeFile((uint8_t*)"\n",1);
	}
    
}


void sdlogger::writeHeaderTypes() 
{
	if(m_initDone)
	{
		char serialdata[200];
		sprintf(serialdata,"\n\r\t\n\r\tStucture size:%lu, datacount: %lu",m_dataPackageSize,m_dataTypeCount);
		debugPrint(serialdata);
		debugPrint("\n\r");

		sdLogger_writeFile((uint8_t*)&m_dataPackageSize,4); // Write data package size
		sdLogger_writeFile((uint8_t*)&m_dataTypeCount,4);   // write datat count
		for(uint32_t i = 0 ; i < m_dataTypeCount; i++)           //write types table
		{
			sdLogger_writeFile(&m_dataTypesTable[i],1);
		}

		for(uint32_t i = 0 ; i < m_dataTypeCount; i++)           //write types table
		{
			char dataToWrite[50];
			int writeCount = 0;

			writeCount = sprintf(dataToWrite,m_dataTitlesTable[i]);

			sdLogger_writeFile(dataToWrite,(uint32_t)writeCount);
			sdLogger_writeFile("\n",1);
		}
	}

}


void sdlogger::debugPrint(const char* outData)
{
     if (m_verbose==1)
    {
        sdLogger_serialOut(outData);
    }
}

uint32_t sdlogger::binToCharDataConvert(const uint8_t* binData, char* charData, uint8_t* dataTypesTable, uint32_t dataTypeCount)
{
    uint32_t dataPtr = 0;
	uint32_t targetPtr = 0;
	
    uint32_t uData;
    float  fData;
    int    intData;
    char cData;
    uint32_t nbCarWritten = 0;
    int64_t i64Data;
    uint64_t u64Data;

	for(uint32_t i = 0 ; i < dataTypeCount; i++)
	{
        switch(dataTypesTable[i]) 
        {
            case logDataType_float:
                fData = *(float*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%f,", fData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(float);
                break;
            
            case logDataType_int:
                intData = *(int*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%d,", intData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(int);
                break;

            case logDataType_uInt:
                uData = *(uint32_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%lu,", uData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(uint32_t);
                break;

            case logDataType_char:
                cData = *(char*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%c,", cData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(char);
                break;

            case logDataType_int64:
                i64Data = *(int64_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%lld,", i64Data);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(int64_t);
                break;
            case logDataType_uint64:
                u64Data = *(uint64_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%llu,", u64Data);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(uint64_t);
                break;


            default:
                break;
        }

	}

    return targetPtr; 
}
