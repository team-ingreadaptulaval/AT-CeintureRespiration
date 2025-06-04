/**
  ******************************************************************************
  * @file   LSM9DS1_I2C_v2.h
  * @author Simon Latour
  * @brief  LSM 9 IMU Driver for LSM9DS1 IMU created to be as much plaform independent as possible
  * @version 3.0
  * @date 2021-006-29
  *
  * @copyright Copyright (c) 2020
  *
  */


#ifndef __LSM9DS1_I2C_H__
#define __LSM9DS1_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//////////////////////////////////////////////////////
/// Accelerometer configuration constants
//////////////////////////////////////////////////////
#define ACCEL_SCALE_2G		 0x00//(0b00 << 3) // 0x00
#define ACCEL_SCALE_4G		 0x10//(0b10 << 3) // 0x10
#define ACCEL_SCALE_8G		 0x18//(0b11 << 3) // 0x18
#define ACCEL_SCALE_16G		 0x08//(0b01 << 3) // 0x08

#define SENSITIVITY_ACCELEROMETER_2  0.000061
#define SENSITIVITY_ACCELEROMETER_4  0.000122
#define SENSITIVITY_ACCELEROMETER_8  0.000244
#define SENSITIVITY_ACCELEROMETER_16 0.000732


//////////////////////////////////////////////////////
/// Gyroscope configuration constants
//////////////////////////////////////////////////////
#define GYRO_SCALE_245_DPS  	0xc3//(0b00 << 3) //0xc3
#define GYRO_SCALE_500_DPS  	0xcb//(0b01 << 3) //0xcb
#define GYRO_SCALE_2000_DPS 	0xdb//(0b11 << 3) //0xdb

#define SENSITIVITY_GYROSCOPE_245    0.00875
#define SENSITIVITY_GYROSCOPE_500    0.0175
#define SENSITIVITY_GYROSCOPE_2000   0.07

//////////////////////////////////////////////////////
/// Magnetometer configuration constants
//////////////////////////////////////////////////////
#define MAGNOMETER_SCALE_4 		(0b00 << 5) // 0x00
#define MAGNOMETER_SCALE_8		(0b01 << 5) // 0x20
#define MAGNOMETER_SCALE_12 	(0b10 << 5) // 0x40
#define MAGNOMETER_SCALE_16		(0b11 << 5) // 0x60

#define SENSITIVITY_MAGNETOMETER_4   0.00014
#define SENSITIVITY_MAGNETOMETER_8   0.00029
#define SENSITIVITY_MAGNETOMETER_12  0.00043
#define SENSITIVITY_MAGNETOMETER_16  0.00058

#define MAG_OPMODE_CONTINUOUS_CONVERSION 0
#define MAG_OPMODE_SINGLE_CONVERSION	 1
#define MAG_OPMODE_POWER_DOWN			 3


//////////////////////////////////////////////////////
/// General IMU constants
//////////////////////////////////////////////////////

// For data access
#define X_AXIS	0
#define Y_AXIS  1
#define Z_AXIS  2


//Used by library for device selection
#define XLG_SELECT	0 // Select accelerometer+gyroscope
#define MAG_SELECT  1 // Select magnetometer

//////////////////////////////////////////////////////
/// IMU Possible ADDRESSES
//////////////////////////////////////////////////////

//Master address
#define LSM9DS1_WHO_AM_I           0x0f

/// ACCEL/GYRO ADDRESS
#define IMU_I2C_ADRESS_ACC1 0X6a			// when SA0=0
#define IMU_I2C_ADRESS_ACC2 0X6b			// when SA0=1 (Default when pin not connected)

/// Magneto Address ADDRESS
#define IMU_I2C_ADRESS_MAG1 0X1c			// when SA0=0
#define IMU_I2C_ADRESS_MAG2 0X1e			//SA1 =1 (Default when pin not connected)


//////////////////////////////////////////////////////
/// Errors
//////////////////////////////////////////////////////
#define IMU_ERRORS_NONE 	0 // HAL_I2C_ERROR_NONE    	//  0 = No error

// I2C RELATED ERRORS
//#define IMU_ERRORS_BERR 		HAL_I2C_ERROR_BERR    	//  (0x00000001U)    BERR error
//#define IMU_ERRORS_ARLO 		HAL_I2C_ERROR_ARLO    	//  (0x00000002U)    ARLO error
//#define IMU_ERRORS_AF 			HAL_I2C_ERROR_AF      	//  (0x00000004U)    ACKF error
//#define IMU_ERRORS_OVR 			HAL_I2C_ERROR_OVR     	//  (0x00000008U)    OVR error
//#define IMU_ERRORS_DMA 			HAL_I2C_ERROR_DMA     	//  (0x00000010U)    DMA transfer error
//#define IMU_ERRORS_TIMEOUT 		HAL_I2C_ERROR_TIMEOUT 	//  (0x00000020U)    Timeout error
//#define IMU_ERRORS_SIZE 		HAL_I2C_ERROR_SIZE    	//  (0x00000040U)    Size Management error
//#define IMU_ERRORS_DMA_PARAM 	HAL_I2C_ERROR_DMA_PARAM //  (0x00000080U)      DMA Parameter Error
//
//#define IMU_ERRORS_CALLBACK 	HAL_I2C_ERROR_INVALID_CALLBACK  // (0x00000100U)    /*!< Invalid Callback error */
//#define IMU_ERRORS_PARAM 		HAL_I2C_ERROR_INVALID_PARAM     // (0x00000200U)    /*!< Invalid Parameters error  */

//lIBRARY RELATED ERRORS
#define IMU_ERRORS_MAG_NOT_INIT		0X500U	// Magnetometer not initialized
#define IMU_ERRORS_ACCEL_NOT_INIT	0X501U	// Accelerometer not initialized
#define IMU_ERRORS_GYRO_NOT_INIT	0X502U	// Gyroscope not initialized
#define IMU_ERRORS_ACCGYRO_NOT_INIT	0X503U	// Accelerometer and Gyroscope not initialized
#define IMU_ERRORS_MAG_POWER_DOWN	0X504U	// Magnetometer not initialized

////////////////////////////////////////////////////
/// IMU Handle type
/// ////////////////////////////////////////////////

// Callback functions model for I2C
typedef uint8_t (*sendI2cDataCallback_t)(uint8_t address, uint8_t* buffer, int length, long timeout, bool stopBit);
typedef uint8_t (*receiveI2cDataCallback_t)(uint8_t address, uint8_t* buffer, int length, long timeout);

// Callback functions model for chipSelect
typedef void (*chipSelectControlCallback_t)(uint8_t state);

typedef struct {
	uint8_t accelerationScale;
	uint8_t gyroScale;
	uint8_t magnetoMeterScale;

	// Set this value to match you hardware configuration.
	// Default value 1: SDO is pulled up on imu boards, if you use a imu module with SDO unconnected, use: 1
	// To use "chip select" functionality or if your IMU board SDO pin is connected to ground, use 0
	uint8_t AddressSelectSdo;


	// Callback for I2C send and receive functions
	sendI2cDataCallback_t 		sendI2cCallback;
	receiveI2cDataCallback_t	receiveI2cCallback;

	// If you wish to use chip select functionality, put your pin an port here.
	// Otherwise set to NULL
	chipSelectControlCallback_t chipSelectCallback;

}imuUserConfig_t;


typedef struct{

	uint8_t reg1;
	uint8_t reg2;
	uint8_t reg3;
	uint8_t reg4;
	uint8_t reg5;
	uint8_t reg6;
	uint8_t reg7;
	uint8_t reg8;
	uint8_t reg9;
	uint8_t reg10;

}agControlRegisters_t;

typedef struct{

	uint8_t reg1;
	uint8_t reg2;
	uint8_t reg3;
	uint8_t reg4;
	uint8_t reg5;

}magControlRegisters_t;

typedef struct {
	uint16_t gAddress;
	uint16_t mAddress;

	// Accel/Gyro config
	uint8_t agInitDone;
	uint8_t accelInitDone;
	uint8_t gyroInitDone;
	float accelSensitivityFactor;
	float gyroSensitivityFactor;
	agControlRegisters_t agCtrlReg;

	// Magnetometer config
	uint8_t magInitDone;
	float magnetoSensitivityFactor;
	magControlRegisters_t magCtrlReg;

}lsm9ds1LibConfig_t;


typedef struct{

	// Only modify values in userConfig
	imuUserConfig_t userConfig;

	// Used by library, do not modify
	lsm9ds1LibConfig_t libConfig;

}lsm9ds1Handle_t;


////////////////////////////////////////////////////
/// User Functions
///////////////////////////////////////////////////

/**
 * @fn int imuInit(imuHandle_t*)
 * @brief Initialize the complete IMU ( Accel, Gyro, Mag) and send config to device
 *
 * @param imuHandle	handle to the imu
 * @return	0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_imuInit(lsm9ds1Handle_t * imuHandle);

/**
 * @fn int accelInit(lsm9ds1Handle_t*)
 * @brief Initialize only the Accelerometer and send config to device
 *
 * @param imuHandle handle to the imu
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_accelInit(lsm9ds1Handle_t * imuHandle);

/**
 * @fn int gyroInit(lsm9ds1Handle_t*)
 * @brief  Initialize only the Gyroscope and send config to device
 *
 * @param imuHandle handle to the imu
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_gyroInit(lsm9ds1Handle_t * imuHandle);

/**
 * @fn int accelGyroInit(imuHandle_t*)
 * @brief Initialize only the Gyroscope and Accelerometer and send config to device
 *
 * @param imuHandle handle to the imu
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_accelGyroInit(lsm9ds1Handle_t * imuHandle);

/**
 * @fn int magInit(imuHandle_t*)
 * @brief Initialize only the Magnetometer and send config to device
 *
 * @param imuHandle handle to the imu
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_magInit(lsm9ds1Handle_t * imuHandle);

/**
 * @fn int magSetOperatingMode(imuHandle_t*, uint8_t)
 * @brief This function is mainly used to power p/down magnometer to save power if not used
 *
 * @param imuHandle handle to the IMU
 * @param operatingMode \see defines MAG_OPMODE_

 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_magSetOperatingMode(lsm9ds1Handle_t * imuHandle, uint8_t operatingMode);


/**
 * @fn uint8_t lsm9ds1_magGetOperatingMode(lsm9ds1Handle_t*)
 * @brief This function is used to get the current operating mode
 *
 * @param imuHandle imuHandle handle to the IMU
 * @return operatingMode \see defines MAG_OPMODE_
 */
uint8_t lsm9ds1_magGetOperatingMode(lsm9ds1Handle_t * imuHandle);
/**
 * @fn int getAccelerationM(imuHandle_t*, float*)
 * @brief  Get acceleration in meters/seconds2
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getAccelerationM(lsm9ds1Handle_t * imuHandle, float *receivedData);

/**
 * @fn int getAccelerationG(imuHandle_t*, float*)
 * @brief
 *
 * @param imuHandle handle to the IMU
 * @param receivedData
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getAccelerationG(lsm9ds1Handle_t * imuHandle, float *receivedData);

/**
 * @fn int getGyroscope(imuHandle_t*, float*)
 * @brief The the scaled gyroscope value
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return				0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getGyroscope(lsm9ds1Handle_t * imuHandle, float *receivedData);

/**
 * @fn int getMagnetometer(imuHandle_t*, float*)
 * @brief Get the scaled value from the magnetoMeter
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return				0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getMagnetometer(lsm9ds1Handle_t * imuHandle, float *receivedData);


////////////////////////////////////////////////////
/// Lower level Functions
///////////////////////////////////////////////////

/**
 * @fn int getRawAcceleration(imuHandle_t*, int*)
 * @brief Raw acceleration data received from the sensor in meters per seconds
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return				0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getRawAcceleration(lsm9ds1Handle_t * imuHandle, int *receivedData);

/**
 * @fn int getRawGyroscope(imuHandle_t*, int*)
 * @brief Raw Gyroscope data received from the sensor
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return				0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getRawGyroscope(lsm9ds1Handle_t * imuHandle, int *receivedData);

/**
 * @fn int getRawMagnetometer(imuHandle_t*, int*)
 * @brief Raw Magnetometer data received from the sensor
 *
 * @param imuHandle handle to the IMU
 * @param receivedData	Pointer to the float buffer to receive the data
 * @return				0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_getRawMagnetometer(lsm9ds1Handle_t * imuHandle, int *receivedData);

/**
 * @fn int imuSendRegister(imuHandle_t*, uint8_t, uint8_t, uint8_t)
 * @brief Send register data to the IMU
 *
 * @param imuHandle	handle to the IMU
 * @param xlgMagSelect	select magnetometer register or accel/gy with: XLG_SELECT/ MAG_SELECT
 * @param regID ID 	of the register
 * @param regValue	value of the register
 * @return 0 = no errors, see IMU_ERRORS_ for error codes
 */
int lsm9ds1_imuSendRegister(lsm9ds1Handle_t * imuHandle, uint8_t xlgMagSelect, uint8_t regID, uint8_t regValue);

/**
 * @fn void tryChipSelect(imuHandle_t*)
 * @brief Toggle pin to select active IMU if chip select logic is enabled
 *
 * @param imuHandle
 */
void lsm9ds1_tryChipSelect(lsm9ds1Handle_t * imuHandle);

/**
 * @fn void tryChipDeselect(imuHandle_t*)
 * @brief Toggle pin to deselect active IMU if chip select logic is enabled
 *
 * @param imuHandle handle to the IMU
 */
void lsm9ds1_tryChipDeSelect(lsm9ds1Handle_t * imuHandle);






#ifdef __cplusplus
}
#endif

#endif



