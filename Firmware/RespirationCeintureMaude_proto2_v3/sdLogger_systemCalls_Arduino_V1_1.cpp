/*
 * sdLogger_systemCalls_Arduino_V1_1.cpp
 *
 *  Created on: 6 déc. 2021
 *      Author: teamat
 */

#include "sdLogger_systemCalls_V1_1.h"
#include "Arduino.h"

#include "teamATbasic_V1_1.h"
#include "SdFat.h"
#include "string.h"


#include "stdint.h"

extern bool debug_newLineReceived;
static SdFat sd;

static SdFile m_file;
static SdFat *sdCardPtr;

SdFile sdfileRead;
File32 folderRead;

static int m_initDone = 0;
static uint8_t m_csPin = D10;//SS;
static int m_frequenc = 8;
uint8_t m_verbose = 0;


void sdLogger_delay(uint32_t delayValue)
{
    delay(delayValue);
}

void sdLogger_serialOut(const char* outData)
{
    Serial.print(outData);
}

bool sdLogger_serialGetNewLineRx()
{
  int bytesToRead = Serial.available();
  if (bytesToRead)
  {
    //Serial.println(ReadSerialPortVal.c_str());
    for(int i = 0; i < bytesToRead; i++)
    {
     // Serial.print(ReadSerialPortVal[i]);
      char ReadSerialPortVal;
      Serial.readBytes(&ReadSerialPortVal,1); // Read Serial port
      if(ReadSerialPortVal == '\r')
      {
          return true;
      }
    }
  }

 return false;
}

void sdLogger_clearNewLineRx()
{
	debug_newLineReceived = false;
}

void sdLogger_setNewLineRx()
{
	debug_newLineReceived = true;
}

void sdLogger_serialDataOut(const uint8_t* outData, uint32_t dataLen)
{
    for(uint32_t i = 0 ; i < dataLen; i++)
    {
        // while(Serial.availableForWrite() == 0 )
        // {
        //   delay(1);
        // }
        Serial.write(&outData[i],1);
    }
}
void sdLogger_debugPrint(const char* outData)
{
    if(m_verbose)
    {
        sdLogger_serialOut(outData);
    }
   
}

uint8_t sdLogger_initSystem(uint8_t verboseOffOn)
{
    sdCardPtr = &sd;
    m_csPin = D10;//SS;
    m_frequenc = 8;//8UL;
    m_initDone = 1;
    m_verbose = verboseOffOn;
    //_folderName = "/" + folderName;

    return 0;
}

bool sdLogger_initBackupMedia()
{
    //SPI_FULL_SPEED
    bool status;
    if(!(status = sdCardPtr->begin(m_csPin), SD_SCK_MHZ(m_frequenc)))
    {
        char errorCode[10];
        sprintf(errorCode,"%u",sdCardPtr->sdErrorCode());

        sdLogger_debugPrint("\n\r Sd init error: ");
        sdLogger_debugPrint(errorCode);
        sdLogger_debugPrint("\n\r");

        sdCardPtr->printSdError(&Serial);
       

    }
    return status;
    //return sdCardPtr->begin(m_csPin, SD_SCK_MHZ(m_frequenc));
}



uint8_t sdLogger_createFolder(const char* folderName)
{
    sdLogger_goToRoot();
    
    // Create a new folder.
    if (!sdCardPtr->exists(folderName)) {
       // Create a new folder.
        if (!sdCardPtr->mkdir(folderName)) {

            sdLogger_debugPrint("Create LOG folder  failed");
        }
        else
        {
            sdLogger_debugPrint("LOG folder created\n\r");
        }
    }
    else
    {
        sdLogger_debugPrint("LOG folder already exists");
    }


    // if (!sdCardPtr->chdir(folderName)) {

    //     sdLogger_serialOut("chdir to log folder failed.\n");

    //     return SDLOGGER_ERROR_FILECREATE;
    // }

    return SDLOGGER_ERROR_NO_ERROR;
}

uint8_t sdLogger_goToRoot()
{
    if (!sdCardPtr->chdir()) {

        //sdLogger_serialOut("chdir to root failed.\n");

        return SDLOGGER_ERROR_ROOTOPEN;
    }  

    return SDLOGGER_ERROR_NO_ERROR;  
}

uint8_t sdLogger_gotoFolder(const char* folderName)
{
    if (!sdCardPtr->chdir(folderName)) {

        sdLogger_debugPrint("chdir to log folder failed.\n");

        return SDLOGGER_ERROR_CHANGEDIR;
    }

    return SDLOGGER_ERROR_NO_ERROR;
}


uint8_t sdLogger_deleteFolder(const char* folderName)
{
    FatFile folder;

    if(!folder.open(folderName))
    {
         sdLogger_debugPrint("error opening folder ");
         sdLogger_debugPrint(folderName);
         sdLogger_debugPrint("\n\r");
         return SDLOGGER_ERROR_FILEOPEN;
    }

    if (!folder.rmRfStar()) {
        sdLogger_debugPrint("remove failed.\n\r");
        return SDLOGGER_ERROR_DELETE;
    } 

    return SDLOGGER_ERROR_NO_ERROR;
}

uint8_t sdLogger_deleteAll()
{

    File32 folder;
    File32 file;

    //TODO valider si on a besoin du O_WRITE
    //if(!folder.open("/",O_WRITE))
    if(!folder.open("/"))
    {
         sdLogger_debugPrint("error opening folder\n\r");
         return SDLOGGER_ERROR_FILEOPEN;
    }

    while(file.openNext(&folder,O_READ))
    {
        if (file.isDir() && !file.isHidden()) 
        {     
            //char fileName[128];
            // file.getName(fileName,128);
            // serialOut("Dir  to remove:  "); serialOut(fileName);serialOut(" --> ");

            if (!file.rmRfStar()) 
            {
                sdLogger_debugPrint("remove directory failed.\n\r");
            }
            else
            {
                sdLogger_debugPrint("Remove successfull\n\r");
            }
        }
        else
        {     
            if(!file.isSystem() && !file.isHidden())
            {
                char fileName[128];

                file.getName(fileName,128);
                sdLogger_debugPrint("file to remove:  "); sdLogger_debugPrint(fileName);sdLogger_debugPrint(" --> ");
                file.close(); 

                if(!file.open(fileName, O_WRITE))
                {
                    sdLogger_debugPrint("remove Open in write failed.\n\r");
                }
                else{
                    
                    if (!file.remove()) {
                        sdLogger_debugPrint("remove file failed.\n\r");
                    }
                    else
                    {
                        sdLogger_debugPrint("Remove successfull\n\r");
                    } 
                } 
            }    
        }
       
        sdLogger_debugPrint("\n\r");

        file.close();  
    }

    return SDLOGGER_ERROR_NO_ERROR;
}

bool sdLogger_fileExists(const char* fileName)
{
    return sdCardPtr->exists(fileName);
}


uint8_t sdLogger_openWriteFile(const char* fileName)
{
    if(m_file.open(fileName, O_CREAT | O_WRITE))
    {  
        return SDLOGGER_ERROR_NO_ERROR;
    }
    
    return SDLOGGER_ERROR_FILEOPEN;
}

uint8_t sdLogger_writeFile(const uint8_t* data, uint32_t size)
{
    if(m_file.write(data, size))
    {
        return SDLOGGER_ERROR_NO_ERROR;
    }
    return SDLOGGER_ERROR_WRITE;
}

uint8_t sdLogger_writeFile(const char* data, uint32_t size)
{
    if(sdLogger_writeFile((uint8_t*)data, size))
    {
        return SDLOGGER_ERROR_NO_ERROR;
    }
     return SDLOGGER_ERROR_WRITE;
}

uint8_t sdLogger_closeWriteFile()
{
    if(m_file.close())
    {
          return SDLOGGER_ERROR_NO_ERROR;
    }
     return SDLOGGER_ERROR_SDCLOSE;
    
}



bool sdLogger_openReadFile(const char* fileName)
{
    return (sdfileRead.open(fileName, O_READ));
}

uint32_t sdLogger_readFile(uint8_t* data, uint32_t size)
{
    uint32_t status =  sdfileRead.read(data, size);//sdfileRead.fgets((char*)data, (int)size);
   
    return status;
}

uint32_t sdLogger_readFileString(char* data, uint32_t maxSize,char delimiter)
{
    uint32_t status =  sdfileRead.fgets((char*)data, (int)maxSize, &delimiter);
   
    return status;
}


uint8_t sdLogger_closeReadFile()
{
    sdfileRead.close();
}




int32_t sdLogger_datatoReadinFile()
{
    return (int32_t)sdfileRead.available();
}

//uint8_t sdLogger_deleteFiles();

uint8_t sdLogger_printFileList(const char* folderName,uint8_t displayTimeStamp )
{

  SdFile fileRead;
  SdFile folder;

  if (!folder.open(folderName)){
        
        sdLogger_debugPrint("Problem opening folder\r\n");

        return SDLOGGER_ERROR_FILEOPEN;

    }

    while (fileRead.openNext(&folder, O_RDONLY)) 
    {
        if (!fileRead.isDir()) 
        {
            fileRead.printName(&Serial);

            if(displayTimeStamp)
            {         
                sdLogger_serialOut(" ");       
                fileRead.printModifyDateTime(&Serial);
            }
        
            sdLogger_serialOut("\n\r"); //Serial.println();
        }
        fileRead.close();
    }


    if (folder.getError()) 
    {
        sdLogger_debugPrint("Problem opening file\r\n");
        return SDLOGGER_ERROR_OPENNEXT;
    }  

    return SDLOGGER_ERROR_NO_ERROR; 
}
