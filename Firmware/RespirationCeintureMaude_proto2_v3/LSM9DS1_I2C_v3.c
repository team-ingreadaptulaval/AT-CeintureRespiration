
#include "LSM9DS1_i2c_v3.h"

#include <stddef.h>

/////////////////////////////////////////
// LSM9DS1 Accel/Gyro (XL/G) Registers //
/////////////////////////////////////////
#define ACT_THS_i2c				0x04
#define ACT_DUR_i2c				0x05
#define INT_GEN_CFG_XL_i2c		0x06
#define INT_GEN_THS_X_XL_i2c	0x07
#define INT_GEN_THS_Y_XL_i2c	0x08
#define INT_GEN_THS_Z_XL_i2c	0x09
#define INT_GEN_DUR_XL_i2c		0x0A
#define REFERENCE_G_i2c			0x0B
#define INT1_CTRL_i2c			0x0C
#define INT2_CTRL_i2c			0x0D
#define WHO_AM_I_XG_i2c			0x0F
#define CTRL_REG1_G_i2c			0x10
#define CTRL_REG2_G_i2c			0x11
#define CTRL_REG3_G_i2c			0x12
#define ORIENT_CFG_G_i2c		0x13
#define INT_GEN_SRC_G_i2c		0x14
#define OUT_TEMP_L_i2c			0x15
#define OUT_TEMP_H_i2c			0x16
#define STATUS_REG_0_i2c		0x17
#define OUT_X_L_G_i2c			0x18
#define OUT_X_H_G_i2c			0x19
#define OUT_Y_L_G_i2c			0x1A
#define OUT_Y_H_G_i2c			0x1B
#define OUT_Z_L_G_i2c			0x1C
#define OUT_Z_H_G_i2c			0x1D
#define CTRL_REG4_i2c			0x1E
#define CTRL_REG5_XL_i2c		0x1F
#define CTRL_REG6_XL_i2c		0x20
#define CTRL_REG7_XL_i2c		0x21
#define CTRL_REG8_i2c			0x22
#define CTRL_REG9_i2c			0x23
#define CTRL_REG10_i2c			0x24
#define INT_GEN_SRC_XL_i2c		0x26
#define STATUS_REG_1_i2c		0x27
#define OUT_X_L_XL_i2c			0x28
#define OUT_X_H_XL_i2c			0x29
#define OUT_Y_L_XL_i2c			0x2A
#define OUT_Y_H_XL_i2c			0x2B
#define OUT_Z_L_XL_i2c			0x2C
#define OUT_Z_H_XL_i2c			0x2D
#define FIFO_CTRL_i2c			0x2E
#define FIFO_SRC_i2c			0x2F
#define INT_GEN_CFG_G_i2c		0x30
#define INT_GEN_THS_XH_G_i2c	0x31
#define INT_GEN_THS_XL_G_i2c	0x32
#define INT_GEN_THS_YH_G_i2c	0x33
#define INT_GEN_THS_YL_G_i2c	0x34
#define INT_GEN_THS_ZH_G_i2c	0x35
#define INT_GEN_THS_ZL_G_i2c	0x36
#define INT_GEN_DUR_G_i2c		0x37

///////////////////////////////
// LSM9DS1 Magneto Registers //
///////////////////////////////
#define OFFSET_X_REG_L_M_i2c	0x05
#define OFFSET_X_REG_H_M_i2c	0x06
#define OFFSET_Y_REG_L_M_i2c	0x07
#define OFFSET_Y_REG_H_M_i2c	0x08
#define OFFSET_Z_REG_L_M_i2c	0x09
#define OFFSET_Z_REG_H_M_i2c	0x0A
#define WHO_AM_I_M_i2c			0x0F
#define CTRL_REG1_M_i2c			0x20
#define MAG_TEMP_COMP_ENABLE	  (1<<7)
#define MAG_OPMODE_XY_LOW_POWER	  (0<<5)
#define MAG_OPMODE_XY_MED_POWER   (1<<5)
#define MAG_OPMODE_XY_HIGH_POWER  (2<<5)
#define MAG_OPMODE_XY_ULTRA_POWER (3<<5)

#define MAG_OUTPUT_RATE_0_625	(0 << 2)
#define MAG_OUTPUT_RATE_1_25	(1 << 2)
#define MAG_OUTPUT_RATE_2_5		(2 << 2)
#define MAG_OUTPUT_RATE_5		(3 << 2)
#define MAG_OUTPUT_RATE_10		(4 << 2)
#define MAG_OUTPUT_RATE_20		(5 << 2)
#define MAG_OUTPUT_RATE_40		(6 << 2)
#define MAG_OUTPUT_RATE_80		(7 << 2)
#define MAG_FAST_ODR_ENABLE		(1<<1)

#define CTRL_REG2_M_i2c			0x21
#define CTRL_REG3_M_i2c			0x22
#define CTRL_REG4_M_i2c			0x23
#define CTRL_REG5_M_i2c			0x24
#define STATUS_REG_M_i2c		0x27
#define OUT_X_L_M_i2c			0x28
#define OUT_X_H_M_i2c			0x29
#define OUT_Y_L_M_i2c			0x2A
#define OUT_Y_H_M_i2c			0x2B
#define OUT_Z_L_M_i2c			0x2C
#define OUT_Z_H_M_i2c			0x2D
#define INT_CFG_M_i2c			0x30
#define INT_SRC_M_i2c			0x30
#define INT_THS_L_M_i2c			0x32
#define INT_THS_H_M_i2c			0x33

////////////////////////////////
// LSM9DS1 WHO_AM_I Responses //
////////////////////////////////
#define WHO_AM_I_AG_RSP_i2c		0x68
#define WHO_AM_I_M_RSP_i2c		0x3D


//////////////////////////////////////
///// Global variables for settings //
//////////////////////////////////////
//float gAccelSensitivityFactor 	= SENSITIVITY_ACCELEROMETER_2;
//float gGyroSensitivityFactor 	= SENSITIVITY_GYROSCOPE_245;
//float gMagnetoSensitivityFactor = SENSITIVITY_MAGNETOMETER_4;


////////////////////////////////////////////////////
/// Private Functions
///////////////////////////////////////////////////

void lsm9ds1_tryChipSelect(lsm9ds1Handle_t * imuHandle)
{
	if(imuHandle->userConfig.chipSelectCallback != NULL)
	{
		// If chip select is used
	 	imuHandle->userConfig.chipSelectCallback(imuHandle->userConfig.AddressSelectSdo);
	}
}

void lsm9ds1_tryChipDeSelect(lsm9ds1Handle_t * imuHandle)
{
	if(imuHandle->userConfig.chipSelectCallback != NULL)
	{
		// If chip select is used
		imuHandle->userConfig.chipSelectCallback(!imuHandle->userConfig.AddressSelectSdo);
	}
}

////////////////////////////////////////////////////
/// Public Functions
///////////////////////////////////////////////////

int lsm9ds1_imuInit(lsm9ds1Handle_t * imuHandle)
{
	int readerror=0;
	int statusReturn = 0;

	readerror = lsm9ds1_accelGyroInit(imuHandle);
	if(readerror)
	{
		statusReturn = readerror;
	}

	readerror = lsm9ds1_magInit(imuHandle);
	if(readerror)
	{
		statusReturn = readerror;
	}

	return statusReturn;
}

int lsm9ds1_accelInit(lsm9ds1Handle_t * imuHandle)
{
	int readerror=0;
	int statusReturn = 0;

	// Set I2C addresses
	if(imuHandle->userConfig.AddressSelectSdo)
	{
		imuHandle->libConfig.gAddress = IMU_I2C_ADRESS_ACC2 << 1;
	}
	else
	{
		imuHandle->libConfig.gAddress = IMU_I2C_ADRESS_ACC1 << 1;
	}

	// Set conversion factors
	switch(imuHandle->userConfig.accelerationScale)
	{
		case ACCEL_SCALE_2G:
			imuHandle->libConfig.accelSensitivityFactor = SENSITIVITY_ACCELEROMETER_2;
			break;

		case ACCEL_SCALE_4G:
			imuHandle->libConfig.accelSensitivityFactor = SENSITIVITY_ACCELEROMETER_4;
			break;

		case ACCEL_SCALE_8G:
			imuHandle->libConfig.accelSensitivityFactor = SENSITIVITY_ACCELEROMETER_8;
			break;

		case ACCEL_SCALE_16G:
			imuHandle->libConfig.accelSensitivityFactor = SENSITIVITY_ACCELEROMETER_16;
			break;

		default:
			break;
	}

	lsm9ds1_tryChipSelect(imuHandle);

	// Set register values
	imuHandle->libConfig.agCtrlReg.reg6 = imuHandle->userConfig.accelerationScale;	// Set acceleration scale

	// Send register config values to IMU IC
	readerror = lsm9ds1_imuSendRegister(imuHandle, XLG_SELECT, CTRL_REG6_XL_i2c, imuHandle->libConfig.agCtrlReg.reg6);

	if(readerror)
	{
		statusReturn = readerror;
	}

	lsm9ds1_tryChipDeSelect(imuHandle);

	if(statusReturn == 0)
	{
		imuHandle->libConfig.accelInitDone = 1;
	}

	return statusReturn;


}

int lsm9ds1_gyroInit(lsm9ds1Handle_t * imuHandle)
{
	int readerror=0;
	int statusReturn = 0;

	// Set I2C addresses
	if(imuHandle->userConfig.AddressSelectSdo)
	{
		imuHandle->libConfig.gAddress = IMU_I2C_ADRESS_ACC2 << 1;
	}
	else
	{
		imuHandle->libConfig.gAddress = IMU_I2C_ADRESS_ACC1 << 1;
	}

	// Set conversion factors
	switch(imuHandle->userConfig.gyroScale)
	{
		case GYRO_SCALE_245_DPS:
			imuHandle->libConfig.gyroSensitivityFactor = SENSITIVITY_GYROSCOPE_245;
			break;

		case GYRO_SCALE_500_DPS:
			imuHandle->libConfig.gyroSensitivityFactor = SENSITIVITY_GYROSCOPE_500;
			break;

		case GYRO_SCALE_2000_DPS:
			imuHandle->libConfig.gyroSensitivityFactor = SENSITIVITY_GYROSCOPE_2000;
			break;

		default:
			break;
	}

	lsm9ds1_tryChipSelect(imuHandle);

	// Set register values
	imuHandle->libConfig.agCtrlReg.reg1 = imuHandle->userConfig.gyroScale; 			// Set Gyroscope scale

	// Send register config values to IMU IC
	readerror = lsm9ds1_imuSendRegister(imuHandle, XLG_SELECT,CTRL_REG1_G_i2c,imuHandle->libConfig.agCtrlReg.reg1);
	if(readerror)
	{
		statusReturn = readerror;
	}

	lsm9ds1_tryChipDeSelect(imuHandle);

	if(statusReturn == 0)
	{
		imuHandle->libConfig.gyroInitDone = 1;
	}

	return statusReturn;


}

int lsm9ds1_accelGyroInit(lsm9ds1Handle_t * imuHandle)
{
	int readerror=0;
	//int statusReturn = 0;

	readerror = lsm9ds1_accelInit(imuHandle);

	if(readerror)
	{
		return readerror;
	}

	readerror = lsm9ds1_gyroInit(imuHandle);
	if(readerror)
	{
		return readerror;
	}

	return IMU_ERRORS_NONE;


}

int lsm9ds1_magInit(lsm9ds1Handle_t * imuHandle)
{
	int readerror=0;
	int statusReturn = 0;

	// Set I2C addresses
	if(imuHandle->userConfig.AddressSelectSdo)
	{
		imuHandle->libConfig.mAddress = IMU_I2C_ADRESS_MAG2 << 1;
	}
	else
	{
		imuHandle->libConfig.mAddress = IMU_I2C_ADRESS_MAG1 << 1;
	}

	// Set conversion factors
	switch(imuHandle->userConfig.magnetoMeterScale)
	{
		case MAGNOMETER_SCALE_4:
			imuHandle->libConfig.magnetoSensitivityFactor = SENSITIVITY_MAGNETOMETER_4;
			break;

		case MAGNOMETER_SCALE_8:
			imuHandle->libConfig.magnetoSensitivityFactor = SENSITIVITY_MAGNETOMETER_8;
			break;

		case MAGNOMETER_SCALE_12:
			imuHandle->libConfig.magnetoSensitivityFactor = SENSITIVITY_MAGNETOMETER_12;
			break;

		case MAGNOMETER_SCALE_16:
			imuHandle->libConfig.magnetoSensitivityFactor = SENSITIVITY_MAGNETOMETER_16;
			break;
		default:
			break;
	}

	lsm9ds1_tryChipSelect(imuHandle);


	// Set register values
	imuHandle->libConfig.magCtrlReg.reg1 = MAG_TEMP_COMP_ENABLE | MAG_OUTPUT_RATE_10; // 0b10010000;
	imuHandle->libConfig.magCtrlReg.reg2 = MAGNOMETER_SCALE_4; // Configure Magnometer scale

	// Send register config values to IMU IC
	readerror = lsm9ds1_imuSendRegister(imuHandle, MAG_SELECT,CTRL_REG1_M_i2c,imuHandle->libConfig.magCtrlReg.reg1);
	if(readerror)
	{
		statusReturn = readerror;
	}

	readerror = lsm9ds1_imuSendRegister(imuHandle, MAG_SELECT,CTRL_REG2_M_i2c,MAGNOMETER_SCALE_4);
	if(readerror)
	{
		statusReturn = readerror;
	}

	lsm9ds1_tryChipDeSelect(imuHandle);

	if(statusReturn == 0)
	{
		imuHandle->libConfig.magInitDone= 1;
	}

	return statusReturn;

}


int lsm9ds1_imuSendRegister(lsm9ds1Handle_t * imuHandle, uint8_t xlgMagSelect, uint8_t regID, uint8_t regValue)
{
	int readError;
	uint8_t data[1] ;
	uint8_t address;

	if(xlgMagSelect == XLG_SELECT)
	{
		address = imuHandle->libConfig.gAddress;
	}
	else
	{
		address = imuHandle->libConfig.mAddress;
	}

	// Configure Accelerometer scale
	data[0] = regID;
	data[1] = regValue;

	readError = imuHandle->userConfig.sendI2cCallback(address,data,2,500,true);

	return readError;
}


int lsm9ds1_magSetOperatingMode(lsm9ds1Handle_t * imuHandle, uint8_t operatingMode)
{
	int readError;
	uint8_t origRegValue;

	if(!imuHandle->libConfig.magInitDone)
	{
		return IMU_ERRORS_MAG_NOT_INIT;
	}

	origRegValue = imuHandle->libConfig.magCtrlReg.reg3 & 0b11111100;		// Get the buffer value and clear the operatingMode part.
	imuHandle->libConfig.magCtrlReg.reg3 = origRegValue | operatingMode;	// Put new op mode in

	lsm9ds1_tryChipSelect(imuHandle);

	readError = lsm9ds1_imuSendRegister(imuHandle, MAG_SELECT, CTRL_REG3_M_i2c, imuHandle->libConfig.magCtrlReg.reg3); //send

	lsm9ds1_tryChipDeSelect(imuHandle);

	return readError;
}

uint8_t lsm9ds1_magGetOperatingMode(lsm9ds1Handle_t * imuHandle)
{
	return imuHandle->libConfig.magCtrlReg.reg3 & 0b00000011;
}

int lsm9ds1_getRawAcceleration(lsm9ds1Handle_t * imuHandle, int *receivedData)
{
	int readError;
	uint8_t buf[10];
	uint8_t data[1] ;

	if(!imuHandle->libConfig.accelInitDone)
	{
		return IMU_ERRORS_ACCGYRO_NOT_INIT;
	}

	data[0] = OUT_X_L_XL_i2c; // Indicate that we want to read Acceleration value register

	lsm9ds1_tryChipSelect(imuHandle);

	readError = imuHandle->userConfig.sendI2cCallback(imuHandle->libConfig.gAddress, data, 1, 500, false);


	if(readError == IMU_ERRORS_NONE)
	{
		imuHandle->userConfig.receiveI2cCallback(imuHandle->libConfig.gAddress, buf, 6, 500);

		*receivedData = (buf[0]&0xff) + ((buf[1]&0xff)<<8);
		if(*receivedData & 0x8000) *receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[2]&0xff) + ((buf[3]&0xff)<<8);
		if(*receivedData & 0x8000) *receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[4]&0xff) + ((buf[5]&0xff)<<8);
		if(*receivedData & 0x8000) *receivedData|=0xffff0000;
	}
	
	lsm9ds1_tryChipDeSelect(imuHandle);

	return readError;
}


int lsm9ds1_getRawGyroscope(lsm9ds1Handle_t * imuHandle, int *receivedData)
{
	int readError;
	uint8_t buf[6];
	uint8_t data[1] ;

	if(!imuHandle->libConfig.gyroInitDone)
	{
		return IMU_ERRORS_ACCGYRO_NOT_INIT;
	}

	data[0] = OUT_X_L_G_i2c; // Indicate that we want to read Gyroscope value register

	lsm9ds1_tryChipSelect(imuHandle);

	readError = imuHandle->userConfig.sendI2cCallback(imuHandle->libConfig.gAddress,data,1,500, false);

	if(readError == IMU_ERRORS_NONE)
	{
		imuHandle->userConfig.receiveI2cCallback(imuHandle->libConfig.gAddress, buf, 6, 500);

		// Concatenate two 8 bits to make one 16 bits buf[1]buf[0]
		*receivedData = (buf[0]&0xff) + ((buf[1]&0xff)<<8);
		if(*receivedData&0x8000) *receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[2]&0xff) + ((buf[3]&0xff)<<8);
		if(*receivedData&0x8000) *receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[4]&0xff) + ((buf[5]&0xff)<<8);
		if(*receivedData&0x8000) *receivedData|=0xffff0000;
	}
	
	lsm9ds1_tryChipDeSelect(imuHandle);

	return readError;

}


int lsm9ds1_getRawMagnetometer(lsm9ds1Handle_t * imuHandle, int *receivedData)
{
	int readError;
	uint8_t buf[6];
	uint8_t data[1];

	if(!imuHandle->libConfig.magInitDone)
	{
		return IMU_ERRORS_MAG_NOT_INIT;
	}

	if(lsm9ds1_magGetOperatingMode(imuHandle) == MAG_OPMODE_POWER_DOWN)
	{
		return IMU_ERRORS_MAG_POWER_DOWN;
	}

	data[0] = OUT_X_L_M_i2c; // Indicate that we want to read Magnetometer value register

	lsm9ds1_tryChipSelect(imuHandle);

	readError = imuHandle->userConfig.sendI2cCallback(imuHandle->libConfig.mAddress, data, 1, 500, false); // Send request with sub address

	if(readError == IMU_ERRORS_NONE)
	{

		imuHandle->userConfig.receiveI2cCallback(imuHandle->libConfig.mAddress, buf, 6, 500); // Receive 6 bytes of Data

		// Concatenate two 8 bits to make one 16 bits buf[1]buf[0]
		*receivedData = (buf[0]&0xff) + ((buf[1]&0xff)<<8);
		if(*receivedData&0x8000)*receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[2]&0xff) + ((buf[3]&0xff)<<8);
		if(*receivedData&0x8000)*receivedData|=0xffff0000;
		receivedData++;

		*receivedData = (buf[4]&0xff) + ((buf[5]&0xff)<<8);
		if(*receivedData&0x8000)*receivedData|=0xffff0000;
	}
	
	lsm9ds1_tryChipDeSelect(imuHandle);

	return readError;
}


int lsm9ds1_getAccelerationG(lsm9ds1Handle_t * imuHandle, float *receivedData)
{
	int dataInteger[3];
	int status;

	status = lsm9ds1_getRawAcceleration(imuHandle, dataInteger);
	if(status == IMU_ERRORS_NONE)
	{
		receivedData[X_AXIS] = (float)dataInteger[X_AXIS] * imuHandle->libConfig.accelSensitivityFactor;
		receivedData[Y_AXIS] = (float)dataInteger[Y_AXIS] * imuHandle->libConfig.accelSensitivityFactor;
		receivedData[Z_AXIS] = (float)dataInteger[Z_AXIS] * imuHandle->libConfig.accelSensitivityFactor;
	}
	else
	{
		receivedData[X_AXIS] = 0;
		receivedData[Y_AXIS] = 0;
		receivedData[Z_AXIS] = 0;
	}

	return status;
}

int lsm9ds1_getAccelerationM(lsm9ds1Handle_t * imuHandle, float *receivedData)
{
	int dataInteger[3];
	int status;

	status = lsm9ds1_getRawAcceleration(imuHandle, dataInteger);
	if(status == IMU_ERRORS_NONE)
	{
		receivedData[X_AXIS] = (float)dataInteger[X_AXIS] * imuHandle->libConfig.accelSensitivityFactor * 9.80665;
		receivedData[Y_AXIS] = (float)dataInteger[Y_AXIS] * imuHandle->libConfig.accelSensitivityFactor * 9.80665;
		receivedData[Z_AXIS] = (float)dataInteger[Z_AXIS] * imuHandle->libConfig.accelSensitivityFactor * 9.80665;
	}
	else
	{
		receivedData[X_AXIS] = 0;
		receivedData[Y_AXIS] = 0;
		receivedData[Z_AXIS] = 0;
	}

	return status;
}

int lsm9ds1_getGyroscope(lsm9ds1Handle_t * imuHandle, float *receivedData)
{
	int dataInteger[3];
	int status;

	status = lsm9ds1_getRawGyroscope(imuHandle, dataInteger);
	if(status == IMU_ERRORS_NONE)
	{
		receivedData[X_AXIS] = (float)dataInteger[X_AXIS] * imuHandle->libConfig.gyroSensitivityFactor;
		receivedData[Y_AXIS] = (float)dataInteger[Y_AXIS] * imuHandle->libConfig.gyroSensitivityFactor;
		receivedData[Z_AXIS] = (float)dataInteger[Z_AXIS] * imuHandle->libConfig.gyroSensitivityFactor;
	}
	else
	{
		receivedData[X_AXIS] = 0;
		receivedData[Y_AXIS] = 0;
		receivedData[Z_AXIS] = 0;
	}

	return status;
}

int lsm9ds1_getMagnetometer(lsm9ds1Handle_t * imuHandle, float *receivedData)
{
	int dataInteger[3];
	int status;

	status = lsm9ds1_getRawMagnetometer(imuHandle, dataInteger);
	if(status == IMU_ERRORS_NONE)
	{
		receivedData[X_AXIS] = (float)dataInteger[X_AXIS] * imuHandle->libConfig.magnetoSensitivityFactor;
		receivedData[Y_AXIS] = (float)dataInteger[Y_AXIS] * imuHandle->libConfig.magnetoSensitivityFactor;
		receivedData[Z_AXIS] = (float)dataInteger[Z_AXIS] * imuHandle->libConfig.magnetoSensitivityFactor;
	}
	else
	{
		receivedData[X_AXIS] = 0;
		receivedData[Y_AXIS] = 0;
		receivedData[Z_AXIS] = 0;
	}

	return status;
}



