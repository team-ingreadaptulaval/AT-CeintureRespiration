/**
 * @file teamtAt_deviceID_V1.0.h
 * @author Simon Latour
 * @brief  Used to generate an very a device id for identification purpose (Serial comm, data backup, etc)
 * @version 0.1
 * @date 2021-09-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TEAMAT_DEVICEID_H
#define TEAMAT_DEVICEID_H

    #ifdef __cplusplus
    extern "C" {
    #endif

        #include <stddef.h>
        #include <stdint.h>
        #include <string.h>
        #include <stdlib.h>
        #include <stdbool.h>

        /**
         * DEVICE INFO ID
         * 
         * FORMAT
         * 
         *  0x 00 00 00 00
         *      ^  ^  ^  ^
         *      |  |  |  |--- PRODUCT VERSION
         *      |  |  |------ PRODUCT ID
         *      |  |--------- CLIENT ID
         *      |------------ CHECKSUM      
         */


        // Clients/ collaborators/project ID
        #define CLIENT_ID_CIRRIS    0x01
        //#define CLIENT_ID_CIE2      0x02
        //#define CLIENT_ID_CIE3      0x03

        // Product ID list
            // Client CIRRIS
            #define PRODUCT_ID_DRINKREMINDER    0x01
            #define PRODUCT_ID_MODULE_ERGO     0x02

            // Other client example
            //#define PRODUCT_ID_Example1         0x01
            //#define PRODUCT_ID_Example2         0x02  

       /**
        * @brief Extract the product id from the deviceID
        * 
        * @param deviceID The source device ID
        * @return uint8_t The extracted product ID
        */
        uint8_t getProductId(unsigned int deviceID);

        /**
        * @brief Extract the client id from the deviceID
        * 
        * @param deviceID The source device ID
        * @return uint8_t The extracted client ID
        */
        uint8_t getClientId(unsigned int deviceID);

        /**
        * @brief Extract the product version from the deviceID
        * 
        * @param deviceID The source device ID
        * @return uint8_t The extracted product version
        */
        uint8_t getProductVersion(unsigned int deviceID);

        /**
         * @brief Calculate the checksum used for data validation
         * 
         * @param clientID          the client ID
         * @param productId         the product ID
         * @param productVersion    the proiduct version
         * @return uint8_t   the calculated checksum
         */
        uint8_t calculateChecksum(uint8_t clientID, uint8_t productId, uint8_t productVersion);

        /**
         * @brief Creat a device ID using the client ID, productId and product version. This function also generate a checksum to test data validity
         * 
         * @param clientID          the client ID
         * @param productId         the product ID
         * @param productVersion    the proiduct version
         * @return unsigned int     the generated device ID
         */
        unsigned int packDeviceId(uint8_t clientID, uint8_t productId, uint8_t productVersion);

        /**
         * @brief Check Device ID validity 
         * 
         * @param deviceID the device ID 
         * @return true if valide
         * @return false if not valid
         */
        bool validateDeviceId(unsigned int deviceID);


    //// Key mechanics
    //
    // The key is a mix of device ID and a 64 bit unique id and is used to stor data and load it back.
    // If not using the unique ID feature, ID value of 0 is used.

    /**
     * @brief Generate a 32 character Key based on device and and unique ID
     * 
     * @param keybuffer the 32 character buffer that will receive the generated key
     * @param deviceId  The device ID to use
     * @param deviceUniqueId The unique ID of the device ( if no unique id, use 0 for ALL_DEVICE)
     */
    void generateKey32(char* keybuffer, unsigned long deviceId, unsigned long long deviceUniqueId);

    /**
     * @brief Extract device ID and device unique id from key
     * 
     * @param keybuffer in:The buffer containing the key
     * @param deviceId  out:pointer to the device ID
     * @param deviceUniqueId out: pointer to the device unique ID
     */
    void extractKey32(char* keybuffer, unsigned long *deviceId, unsigned long long *deviceUniqueId);




    #ifdef __cplusplus
    }
    #endif
#endif