/*
 * imu.h
 *
 *  Created on: 16 avr. 2021
 *      Author: teamat
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

	//#include "main.h"
	#include "stddef.h"
	#include "LSM9DS1_I2C_v3.h"
	#include "imuManager_V0_3.h"


	#define NB_IMU_MAX 5

	#define IMU_SELECT_1 0
	#define IMU_SELECT_2 1
	#define IMU_SELECT_3 2
	#define IMU_SELECT_4 3

    #define IMU_INDEX_OUT_OF_BOUND	255


	#define IMU_INIT_ERROR_NONE			0
	#define IMU_INIT_ERROR_ACCELGYRO 	1
	#define IMU_INIT_ERROR_MAG			2
	#define IMU_INIT_ERROR_ACCEL		3
	#define IMU_INIT_ERROR_GYRO			4
	#define IMU_INIT_ERROR_IMU_INDEX	5

	extern lsm9ds1Handle_t hI2cImu1;	// Handle for LSM9DS1 IMU #1
	extern lsm9ds1Handle_t hI2cImu2;	// Handle for LSM9DS1 IMU #2

	//! [imu manager variables]
	// IMU manager Setup Step 1: instance of the imu manager
	extern imuManagerClass imuManager;
	//! [imu manager variables]


	/**
	 * @fn int imuInit()
	 * @brief This function regroups all the initialization tasks of the IMUs
	 *
	 * @return	0 if not error, see IMU_INIT_ERROR_
	 */
	int imuInit();

	/**
     * @fn int imuRequestAccel(int)
	 * @brief Function to request new data to the IMUs that will be used by the IMU manager
	 *
	 * @param imuSelect the # on the imu
	 * @return error status
	 */
	int imuRequestAccel(int imuSelect);

	/**
	 * @fn int imuRequestGyro(int)
	 * @brief Function to request new data to the IMUs that will be used by the IMU manager
	 *
	 * @param imuSelect the # on the imu
	 * @return error status
	 */
	int imuRequestGyro(int imuSelect);

	/**
	 * @fn int imuRequestMag(int)
	 * @brief Function to request new data to the IMUs that will be used by the IMU manager
	 *
	 * @param imuSelect the # on the imu
	 * @return error status
	 */
	int imuRequestMag(int imuSelect);


	/**
	 * @fn int imuHardwareInitV2_lsm9(sendI2cDataCallback_t, receiveI2cDataCallback_t, int, uint8_t, chipSelectControlCallback_t)
	 * @brief Initialisation of the IMUs related to hardware config
	 *
	 * @param i2cSendCallback		I2C send data callback function
	 * @param i2cReceiveCallback	I2C receive data callback function
	 * @param imuSelect				# of the imu
	 * @param AddressSelectSdo		Active low or active high select
	 * @param chipSelectCallback	Chip select pin  control callaback. Set to NULL if not used

	 * @return error status
	 */
	int imuHardwareInitV2_lsm9(sendI2cDataCallback_t i2cSendCallback, receiveI2cDataCallback_t i2cReceiveCallback, int imuSelect, uint8_t AddressSelectSdo = 0,
			chipSelectControlCallback_t chipSelectCallback = NULL);
	/**
     * @fn int imuAccelInit(int, uint8_t)
	 * @brief  Init of the accelerometer parameters
	 *
	 * @param imuSelect # of the imu
	 * @param accelScale	acceleration scale
	 * @return error status
	 */
	int imuAccelInit_lsm9(int imuSelect, uint8_t accelScale);

	/**
	 * @fn int imuGyroInit(int, uint8_t)
	 * @brief  Init of the gyroscope parameters
	 *
	 * @param imuSelect 	# of the imu
	 * @param accelScale 	gyroscope scale
	 * @return error status
	 */
	int imuGyroInit_lsm9(int imuSelect, uint8_t gyroScale);

	/**
	 * @fn int imuMagInit(int, uint8_t)
	 * @brief Init the magnetometer parameters
	 *
	 * @param imuSelect 	# of the imu
	 * @param accelScale 	magnetometer scale
	 * @return error status
	 */
	int imuMagInit_lsm9(int imuSelect, uint8_t magScale);


	

	/**
	 * @fn uint8_t sendData_I2c1(uint8_t, uint8_t*, int, long)
	 * @brief I2C 1 Send data function
	 *
	 * @param address 	I2C address
	 * @param buffer	buffer with data
	 * @param length	number of bytes to send
	 * @param timeout	timeout delay if needed by the platform
	 *
	 * @return error status
	 */
	uint8_t sendData_I2c1(uint8_t address, uint8_t* buffer, int length, long timeout, bool stopBit);

	/**
	 * @fn uint8_t receiveData_I2c1(uint8_t, uint8_t*, int, long)
	 * @brief I2C 1 Receive data function
	 *
	 * @param address 	I2C address
	 * @param buffer	buffer with data
	 * @param length	number of bytes to send
	 * @param timeout	timeout delay if needed by the platform
	 *
	 *
	 * @return error status
	 */
	uint8_t receiveData_I2c1(uint8_t address, uint8_t* buffer, int length, long timeout);

	// /**
	//  * @fn void imu1_chipSelectControl(uint8_t)
	//  * @brief Imu #1 chip select pin control function
	//  *
	//  * @param state 0 = low, 1 = high
	//  */
	// void imu1_chipSelectControl(uint8_t state);

	// /**
	//  * @fn void imu2_chipSelectControl(uint8_t)
	//  * @brief Imu #2 chip select pin control function
	//  *
	//  * @param state 0 = low, 1 = high
	//  */
	// void imu2_chipSelectControl(uint8_t state);


#endif /* INC_IMU_H_ */
