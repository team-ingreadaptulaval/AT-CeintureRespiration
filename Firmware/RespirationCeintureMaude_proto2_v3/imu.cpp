/*
 * imu.cpp
 *
 *  Created on: 16 avr. 2021
 *      Author: teamat
 */

#include "imu.h"
#include "Wire.h"
#include "Arduino.h"


/////////////////////////////////////////////////
/// IMU global variables
////////////////////////////////////////////////

//! [LSM9DS1 Includes and variables]

	lsm9ds1Handle_t hI2cImu[NB_IMU_MAX];

//! [imu manager variables]
	// IMU manager Setup Step 1: instance of the imu manager
	imuManagerClass imuManager;

	// IMU manager Setup Step 2: global handle(s)
	imuManagerHandle_t hImu[NB_IMU_MAX];

	// Variables For accessing the imu trough the imu manager
	uint8_t imuIndex[NB_IMU_MAX];


//! [I2C FUNCTIONS CALLBACKS]
	uint8_t sendData_I2c1(uint8_t address, uint8_t* buffer, int length, long timeout, bool stopBit)
	{
		TwoWire *_wire;

		#ifdef ARDUINO_ARDUINO_NANO33BLE
			_wire = &Wire1;
		#else
			_wire = &Wire;
		#endif

		_wire->beginTransmission(address>>1);
		_wire->write(buffer, length);

		uint8_t status  = _wire->endTransmission(stopBit);
  		if ( status != 0) {
			return status;
  		}

  		return 0;

	}

	uint8_t receiveData_I2c1(uint8_t address, uint8_t* buffer, int length, long timeout)
	{

		TwoWire *_wire;

		#ifdef ARDUINO_ARDUINO_NANO33BLE
			_wire = &Wire1;

		#else

			_wire = &Wire;

		#endif

		uint8_t rxLen = _wire->requestFrom(address>>1, length);
		if ( rxLen != length) {
			return 1;
		}

		for (size_t i = 0; i < length; i++) {
			*buffer++ = _wire->read();
		}

		return 0;

	}

// void imu1_chipSelectControl(uint8_t state)
// {
	
	
// 	//ex for STM32 IDE: HAL_GPIO_WritePin(OUT_IMU1_CS_GPIO_Port, OUT_IMU1_CS_Pin, (GPIO_PinState)state);

// }

// void imu2_chipSelectControl(uint8_t state)
// {
// 	HAL_GPIO_WritePin(OUT_IMU2_CS_GPIO_Port, OUT_IMU2_CS_Pin, (GPIO_PinState)state);
// }




int imuInit() {

	int errorReturn = IMU_INIT_ERROR_NONE;
	int status = IMU_INIT_ERROR_NONE;
	int tempImuIndex = IMU_INDEX_OUT_OF_BOUND;

	// Step 1: If you need more than 4 IMUs go modify the value of NB_IMU_MAX in imu.h You can also reduce it to same memory if needed to match the exact number of IMU you are using

	#ifdef ARDUINO_ARDUINO_NANO33BLE
		Wire1.begin();
	#else

		Wire.begin();

	#endif

	// First IMU
	// LSM9DS1 Initialization
	// Chip select pin configuration + i2c port

	status = imuHardwareInitV2_lsm9(sendData_I2c1, receiveData_I2c1,// I2c callback functions
			IMU_SELECT_1, 											// Imu #1
			1, 														// Address select on pin low
			NULL // imu1_chipSelectControl  								// Chip select pin info
			);

	 if (status == IMU_INIT_ERROR_NONE) {
	 	int dummyData[3];

	 	// Configure desired scale for accelerometer
	 	errorReturn += imuAccelInit_lsm9(IMU_SELECT_1, ACCEL_SCALE_16G);

	 	// Configure desired scale for gyroscope
	 	errorReturn += imuGyroInit_lsm9(IMU_SELECT_1, GYRO_SCALE_2000_DPS);

	 	// Configure desired scale for magnetometer
	 	errorReturn += imuMagInit_lsm9(IMU_SELECT_1, MAGNOMETER_SCALE_4);

	 	//Dummy data read to test connexion / device presence
	 	errorReturn += lsm9ds1_getRawAcceleration(&hI2cImu[IMU_SELECT_1],
	 	  		dummyData);

	 } else {
	 	errorReturn += status;
	 }

	// IMU manager Setup Step 5: Add IMU to the imu manager
	tempImuIndex = imuManager.addImu(&hImu[IMU_SELECT_1]);
	
	if (tempImuIndex >= 0) {
		imuIndex[IMU_SELECT_1] = (uint8_t) tempImuIndex;
	} else {
		imuIndex[IMU_SELECT_1] = IMU_INDEX_OUT_OF_BOUND;
		errorReturn += IMU_INIT_ERROR_IMU_INDEX;
	}


	// pass pointer to callback functions
	errorReturn += imuManager.init(imuRequestAccel, imuRequestGyro,
	 		imuRequestMag);

	return errorReturn;
}


	int imuHardwareInitV2_lsm9(sendI2cDataCallback_t i2cSendCallback, receiveI2cDataCallback_t i2cReceiveCallback, int imuSelect,
			uint8_t AddressSelectSdo, chipSelectControlCallback_t chipSelectCallback)
	{
		if(imuSelect >= NB_IMU_MAX) return IMU_INIT_ERROR_IMU_INDEX;
		// Chip select pin configuration
		hI2cImu[imuSelect].userConfig.AddressSelectSdo 	= AddressSelectSdo;	// Device will answer when SDO set to zero ( chip select)

		// Specify which I2C port to use
		hI2cImu[imuSelect].userConfig.receiveI2cCallback = i2cReceiveCallback;
		hI2cImu[imuSelect].userConfig.sendI2cCallback = i2cSendCallback;
		hI2cImu[imuSelect].userConfig.chipSelectCallback = chipSelectCallback;

		return IMU_INIT_ERROR_NONE;
	}

	int imuAccelInit_lsm9(int imuSelect, uint8_t accelScale)
	{
		if(imuSelect >= NB_IMU_MAX) return IMU_INIT_ERROR_IMU_INDEX;

		// Configure desired scales
		hI2cImu[imuSelect].userConfig.accelerationScale = accelScale;

		// Send configuration to IMU chip
		return lsm9ds1_accelInit(&hI2cImu[imuSelect]);
	}

	int imuGyroInit_lsm9(int imuSelect, uint8_t gyroScale)
	{
		if(imuSelect >= NB_IMU_MAX) return IMU_INIT_ERROR_IMU_INDEX;

		// Configure desired scales
		hI2cImu[imuSelect].userConfig.gyroScale = gyroScale;

		// Send configuration to IMU chip
		return lsm9ds1_gyroInit(&hI2cImu[imuSelect]);
	}

	int imuMagInit_lsm9(int imuSelect, uint8_t magScale)
	{
		if(imuSelect >= NB_IMU_MAX) return IMU_INIT_ERROR_IMU_INDEX;

		hI2cImu[imuSelect].userConfig.magnetoMeterScale = magScale;

		// Send configuration to IMU chip
		return lsm9ds1_magInit(&hI2cImu[imuSelect]);
	}


	/**
	 * @fn int imuRequestAccel(int imuSelect)
	 * @brief This function gets the acceleration from the lsm9ds1 #1 and sends the received data to the imu manager
	 *
	 * @param imuSelect is the # of the IMU to send the request to
	 * @return status
	 */
	int imuRequestAccel(int imuSelect)
	{
		int status;
		float dataArray[3];
		floatXYZ_t data;

		//Get data from imu IC ( Polling mode)
		status = lsm9ds1_getAccelerationM(&hI2cImu[imuSelect],dataArray);
		// note: For now we only have lsm9 IMUs. in the future, if we would have different types we would need to used the imuSelect to know
		// which imu is requested and call the appropriate function based on that.

		data.x = dataArray[X_AXIS];
		data.y = dataArray[Y_AXIS];
		data.z = dataArray[Z_AXIS];

		if(status == 0)
		{
			imuManager.setAccel(imuSelect, data); // Send received Data to imu manager
		}

		return status;
	}

	/**
     * @fn int imuRequestGyro(int imuSelect)
	 * @brief  This function gets the gyro value from the lsm9ds1  and sends the received data to the imu manager
	 *
	 * @param imuSelect
	 * @return
	 */
	int imuRequestGyro(int imuSelect)
	{
		int status;
		float dataArray[3];
		floatXYZ_t data;

		status = lsm9ds1_getGyroscope(&hI2cImu[imuSelect], dataArray);
		// note: For now we only have lsm9 IMUs. in the future, if we would have different types we would need to used the imuSelect to know
		// which imu is requested and call the appropriate function based on that.

		data.x = dataArray[X_AXIS];
		data.y = dataArray[Y_AXIS];
		data.z = dataArray[Z_AXIS];

		if(status == 0)
		{
			imuManager.setGyro(imuSelect, data); // Send received Data to imu manager
		}

		return status;
	}

	/**
     * @fn int imuRequestMag(int)
	 * @brief  This function gets the magnetometer value from the lsm9ds1 and sends the received data to the imu manager
	 *
	 * @param imuSelect
	 * @return
	 */
	int imuRequestMag(int imuSelect)
	{
		int status;
		float dataArray[3];
		floatXYZ_t data;

		status = lsm9ds1_getMagnetometer(&hI2cImu[imuSelect], dataArray);
		// note: For now we only have lsm9 IMUs. in the future, if we would have different types we would need to used the imuSelect to know
		// which imu is requested and call the appropriate function based on that.

		data.x = dataArray[X_AXIS];
		data.y = dataArray[Y_AXIS];
		data.z = dataArray[Z_AXIS];

		if(status == 0)
		{
			imuManager.setMag(imuSelect, data); // Send received Data to imu manager
		}

		return status;
	}
