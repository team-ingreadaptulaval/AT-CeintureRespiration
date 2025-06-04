
#include "teamtAt_deviceID_V1.0.h"



    #include <stdio.h>

    uint8_t getProductId(unsigned int deviceID){
        return (deviceID >> 8) & 0xFF;
    }

    uint8_t getClientId(unsigned int deviceID){
        return (deviceID >> 16) & 0xFF;
    }

    uint8_t getProductVersion(unsigned int deviceID){
        return (deviceID) & 0xFF;
    }

    uint8_t calculateChecksum(uint8_t clientID, uint8_t productId, uint8_t productVersion)
    {      
        return ((productVersion + productId + clientID + 13) * 63) & 0xFF;
    }

    unsigned int packDeviceId(uint8_t clientID, uint8_t productId, uint8_t productVersion)
    {
        unsigned int PackedData;

        //PackedData = (productVersion + productId + clientID + 13) * 63 << 24;   // Calculate security checksum
        PackedData = (unsigned int)calculateChecksum(clientID,productId,productVersion) << 24;   // Calculate security checksum
        
        PackedData = PackedData | clientID << 16 | productId << 8 | productVersion;

        return PackedData;

    }

    bool validateDeviceId(unsigned int deviceID)
    {
        uint8_t calculatedChecksum, extractedChecksum;

        calculatedChecksum = calculateChecksum(getClientId(deviceID),getProductId(deviceID),getProductVersion(deviceID));

        extractedChecksum = (deviceID >> 24) & 0xFF;

        return (calculatedChecksum == extractedChecksum); // verify that calculated value match value in variable

    }



    void generateKey32(char* keybuffer, unsigned long deviceId, unsigned long long deviceUniqueId)
    {
        int res;
        char deviceIdStr[9];    // size 4 bytes in hex  +1 for\0
        char deviceUIdStr[17];  // 8 bytesin hex + 1 for \0

        sprintf(deviceIdStr,"%08x",deviceId);
        sprintf(&deviceUIdStr[0],"%08x",deviceUniqueId>>32);
        sprintf(&deviceUIdStr[8],"%08x",deviceUniqueId)&0xFFFFFFFFFFFFFFFF;

        memcpy(&keybuffer[0],"/kv/",4);
        memcpy(&keybuffer[4],deviceIdStr,8);
        memcpy(&keybuffer[12],deviceUIdStr,16);
        memcpy(&keybuffer[29],"",1);
    }

    void extractKey32(char* keybuffer, unsigned long *deviceId, unsigned long long *deviceUniqueId)
    {
        char deviceIdStr[9];    // size 4 bytes in hex  +1 for\0
        char deviceUIdStr[17];  // 8 bytesin hex + 1 for \0
        char *endPtr;

        memcpy(deviceIdStr,&keybuffer[4],8);
        deviceIdStr[8] = '\0';
        *deviceId = strtol(deviceIdStr,&endPtr,16);
        memcpy(deviceUIdStr,&keybuffer[12],16);
        deviceUIdStr[16] = '\0';
        *deviceUniqueId = strtoll(deviceUIdStr,&endPtr,16);
    }