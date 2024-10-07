/*
 * LSM6DS3.h
 *
 *  Created on: Jul 16, 2024
 *      Author: johnt
 */

#ifndef INC_LSM6DS3_H_
#define INC_LSM6DS3_H_


#include "main.h"


/* device defines */
#define LSM6DS3_DEVICE_ID 			0b01101010 /* fixed value of WHO_AM_I register */
#define LSM6DS3_RESOLUTION 			16  /* bit depth of sensor */

/* device register addresses (p.49) */
#define LSM6DS3_REG_INT1_CTRL		0x0D
#define LSM6DS3_REG_INT2_CTRL 		0x0E

#define LSM6DS3_REG_WHO_AM_I 		0x0F

#define LSM6DS3_REG_CTRL1_XL  		0x10
#define LSM6DS3_REG_CTRL2_G  		0x11
#define LSM6DS3_REG_CTRL3_G  		0x12
#define LSM6DS3_REG_CTRL4_C  		0x13
#define LSM6DS3_REG_CTRL5_C  		0x14
#define LSM6DS3_REG_CTRL6_C 		0x15
#define LSM6DS3_REG_CTRL7_G 		0x16
#define LSM6DS3_REG_CTRL8_XL        0x17
#define LSM6DS3_REG_CTRL9_XL        0x18
#define LSM6DS3_REG_CTRL10_C        0x19

#define LSM6DS3_REG_STATUS_REG      0x1E

#define LSM6DS3_REG_OUT_TEMP_L 		0x20  /* temperature sensor data */
#define LSM6DS3_REG_OUT_TEMP_H 		0x21

#define LSM6DS3_REG_OUTX_L_G 		0x22  /* gyroscope output registers */
#define LSM6DS3_REG_OUTX_H_G 		0x23
#define LSM6DS3_REG_OUTY_L_G 		0x24
#define LSM6DS3_REG_OUTY_H_G 		0x25
#define LSM6DS3_REG_OUTZ_L_G 		0x26
#define LSM6DS3_REG_OUTZ_H_G 		0x27

#define LSM6DS3_REG_OUTX_L_XL 		0x28  /* accelerometer output registers */
#define LSM6DS3_REG_OUTX_H_XL 		0x29
#define LSM6DS3_REG_OUTY_L_XL 		0x2A
#define LSM6DS3_REG_OUTY_H_XL 		0x2B
#define LSM6DS3_REG_OUTZ_L_XL 		0x2C
#define LSM6DS3_REG_OUTZ_H_XL 		0x2D

#define LSM6DS3_REG_X_OFS_USR 		0x73  /* user offsets */
#define LSM6DS3_REG_Y_OFS_USR 		0x74
#define LSM6DS3_REG_Z_OFS_USR 		0x75

/* register values */
#define LSM6DS3_ACCEL_ODR_DISABLE 0b00000000  /* CTRL1_XL register */
#define LSM6DS3_ACCEL_ODR_13HZ    0b00010000
#define LSM6DS3_ACCEL_ODR_26HZ    0b00100000
#define LSM6DS3_ACCEL_ODR_52HZ    0b00110000
#define LSM6DS3_ACCEL_ODR_104HZ   0b01000000
#define LSM6DS3_ACCEL_ODR_208HZ   0b01010000
#define LSM6DS3_ACCEL_ODR_416HZ   0b01100000
#define LSM6DS3_ACCEL_ODR_833HZ   0b01110000
#define LSM6DS3_ACCEL_ODR_1660HZ  0b10000000
#define LSM6DS3_ACCEL_ODR_3330HZ  0b10010000
#define LSM6DS3_ACCEL_ODR_6660HZ  0b10100000

#define LSM6DS3_G_RANGE_2  			0b00000000  /* CTRL1_XL register */
#define LSM6DS3_G_RANGE_4  			0b00001000
#define LSM6DS3_G_RANGE_8  			0b00001100
#define LSM6DS3_G_RANGE_16 			0b00000100

#define LSM6DS3_GYRO_ODR_DISABLE 0b00000000  /* CTRL2_G register */
#define LSM6DS3_GYRO_ODR_13HZ    0b00010000
#define LSM6DS3_GYRO_ODR_26HZ    0b00100000
#define LSM6DS3_GYRO_ODR_52HZ    0b00110000
#define LSM6DS3_GYRO_ODR_104HZ   0b01000000
#define LSM6DS3_GYRO_ODR_208HZ   0b01010000
#define LSM6DS3_GYRO_ODR_416HZ   0b01100000
#define LSM6DS3_GYRO_ODR_833HZ   0b01110000
#define LSM6DS3_GYRO_ODR_1660HZ  0b10000000
#define LSM6DS3_GYRO_ODR_3330HZ  0b10010000
#define LSM6DS3_GYRO_ODR_6660HZ  0b10100000

#define LSM6DS3_DPS_RANGE_125  0b00000010  /* CTRL2_G register */
#define LSM6DS3_DPS_RANGE_245  0b00000000
#define LSM6DS3_DPS_RANGE_500  0b00000100
#define LSM6DS3_DPS_RANGE_1000 0b00001000
#define LSM6DS3_DPS_RANGE_2000 0b00001100

typedef struct
{
	/* disable the HPF, set the LPFs so everything is Nyquist frequency ODR/2, implement filters upstream if desired */
	/* functions to put in user offset values, but calibration done through JavaScript UI */

	float usr_offset_x, usr_offset_y, usr_offset_z;  /* DC offsets in g */

	uint32_t accel_odr_hz;  /* 13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660 */
	uint32_t g_range;  /* 2, 4, 8, or 16 */

	uint32_t gyro_odr_hz;  /* 13, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660 */
	uint32_t dps_range;  /* 125, 245, 500, 1000, 2000 */

} LSM6DS3_Config;

typedef struct
{
	/* SPI handle */
	SPI_HandleTypeDef* spi;

	/* chip select pin */
	GPIO_TypeDef* cs_port;
	uint16_t cs_pin;

	/* interrupt pins */
	GPIO_TypeDef* int1_port;
	uint16_t int1_pin;
	GPIO_TypeDef* int2_port;
	uint16_t int2_pin;

	/* pointer to object that stores configuration parameters */
	LSM6DS3_Config* config;

} LSM6DS3;

void LSM6DS3_Init(LSM6DS3* sensor, LSM6DS3_Config* config, SPI_HandleTypeDef* spi,
		GPIO_TypeDef* cs_port, uint16_t cs_pin,
		GPIO_TypeDef* int1_port, uint16_t int1_pin,
		GPIO_TypeDef* int2_port, uint16_t int2_pin);

uint32_t LSM6DS3_TestCommunication(LSM6DS3* sensor);

void LSM6DS3_Enable(LSM6DS3* sensor);
void LSM6DS3_Disable(LSM6DS3* sensor);

void LSM6DS3_WriteOffsets(LSM6DS3* sensor);
void LSM6DS3_StartAccel(LSM6DS3* sensor);
void LSM6DS3_StartGyro(LSM6DS3* sensor);

void LSM6DS3_ReadAccel(LSM6DS3* sensor, float* x, float* y, float * z);  /* get the acceleration in g */
void LSM6DS3_ReadGyro(LSM6DS3* sensor, float* x, float* y, float * z);  /* get the rotation rate in degrees per second */

HAL_StatusTypeDef LSM6DS3_ReadRegister(LSM6DS3* sensor, uint8_t reg, uint8_t* data);
HAL_StatusTypeDef LSM6DS3_WriteRegister(LSM6DS3* sensor, uint8_t reg, uint8_t data);

uint32_t LSM6DS3_GetInt1State(LSM6DS3* sensor);  /* read the state of the chip interrupt pins */
uint32_t LSM6DS3_GetInt2State(LSM6DS3* sensor);


#endif /* INC_LSM6DS3_H_ */
