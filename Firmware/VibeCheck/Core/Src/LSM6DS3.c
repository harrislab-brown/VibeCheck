/*
 * LSM6DS3.c
 *
 *  Created on: Jul 16, 2024
 *      Author: johnt
 */

#include "LSM6DS3.h"
#include "fast_spi.h"


void LSM6DS3_Init(LSM6DS3* sensor, LSM6DS3_Config* config, SPI_HandleTypeDef* spi,  /* set the configuration parameters that need to be set once */
		GPIO_TypeDef* cs_port, uint16_t cs_pin,
		GPIO_TypeDef* int1_port, uint16_t int1_pin,
		GPIO_TypeDef* int2_port, uint16_t int2_pin)
{
	sensor->config = config;
	sensor->spi = spi;
	sensor->cs_port = cs_port;
	sensor->cs_pin = cs_pin;
	sensor->int1_port = int1_port;
	sensor->int1_pin = int1_pin;
	sensor->int2_port = int2_port;
	sensor->int2_pin = int2_pin;
}


uint32_t LSM6DS3_TestCommunication(LSM6DS3* sensor)  /* check that the sensor is connected by querying its device ID */
{
	uint8_t reg_data;
	HAL_StatusTypeDef status = LSM6DS3_ReadRegister(sensor, LSM6DS3_REG_WHO_AM_I, &reg_data);
	if (status == HAL_OK && reg_data == LSM6DS3_DEVICE_ID)
		return 1;
	return 0;
}


void LSM6DS3_Enable(LSM6DS3* sensor)  /* configure the sensor and start gathering data */
{
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_INT1_CTRL, 0x01);  /* INT1 set when accel data ready (p. 59) */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_INT2_CTRL, 0x02);  /* INT2 set when gyro data ready (p. 60) */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL4_C, 0b00000100);  /* disable the I2C interface, also disables the gyro LPF1 (p. 64)  TODO: handle the gyro LPF */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL6_C, 0b00000000);  /* sets the user offset weights and the gyro LPF bandwidth (p. 66)  TODO: handle the gyro LPF */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL7_G, 0b00000000);  /* handles gyro HPF (p. 67)  TODO: implement this in the configuration */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL8_XL, 0b00000000);  /* acceleration filters (p. 67)  TODO: implement this in the configuration */

	/* TODO: write the user offsets */

	/* enable the sensor with the configured range (p. 32) */
//	uint8_t range_data;
//	switch(sensor->config->g_range)
//	{
//	case 2:
//		range_data = LSM6DS3_G_RANGE_2;
//		break;
//	case 4:
//		range_data = LSM6DS3_G_RANGE_4;
//		break;
//	case 8:
//		range_data = LSM6DS3_G_RANGE_8;
//		break;
//	case 16:
//		range_data = LSM6DS3_G_RANGE_16;
//		break;
//	default:
//		range_data = LSM6DS3_G_RANGE_2;
//		break;
//	}
//
//	reg_data = (LSM6DS3_ODR_26667HZ | range_data);
//	status = LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL1_XL, &reg_data);
//	err_num += (status != HAL_OK);
//
//	return err_num;
}


void LSM6DS3_Disable(LSM6DS3* sensor)
{
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL1_XL, LSM6DS3_ODR_DISABLE);  /* power down accel. (p. 61) */
	(void)LSM6DS3_WriteRegister(sensor, LSM6DS3_REG_CTRL2_G, LSM6DS3_ODR_DISABLE);  /* power down gyro. (p. 62) */
}


void LSM6DS3_ReadAcceleration(LSM6DS3* sensor, float* x, float* y, float * z)
{
	/* get the acceleration in g */

	/* read multiple bytes corresponding to the raw accelerometer data */
	uint8_t tx_buf[7] = {(LSM6DS3_REG_OUTX_L_XL | 0x80), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t rx_buf[7];

	/* use our fast SPI implementation instead of the HAL call */
	SPI_TxRx_Fast(tx_buf, rx_buf, 7, sensor->spi->Instance, sensor->cs_port, sensor->cs_pin);

	/* convert the raw readings to physical units */
	int16_t raw_data_x = ((int16_t)(rx_buf[1])) | (((int16_t)(rx_buf[2])) << 8);
	int16_t raw_data_y = ((int16_t)(rx_buf[3])) | (((int16_t)(rx_buf[4])) << 8);
	int16_t raw_data_z = ((int16_t)(rx_buf[5])) | (((int16_t)(rx_buf[6])) << 8);

	*x = sensor->config->g_range * (float)raw_data_x / (float)(1 << (LSM6DS3_RESOLUTION - 1));
	*y = sensor->config->g_range * (float)raw_data_y / (float)(1 << (LSM6DS3_RESOLUTION - 1));
	*z = sensor->config->g_range * (float)raw_data_z / (float)(1 << (LSM6DS3_RESOLUTION - 1));

}


void LSM6DS3_ReadGyro(LSM6DS3* sensor, float* x, float* y, float * z)
{
	/* get the rotation rate in degrees per second */

	/* read multiple bytes corresponding to the raw gyroscope data */
	uint8_t tx_buf[7] = {(LSM6DS3_REG_OUTX_L_G | 0x80), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t rx_buf[7];

	/* use our fast SPI implementation instead of the HAL call */
	SPI_TxRx_Fast(tx_buf, rx_buf, 7, sensor->spi->Instance, sensor->cs_port, sensor->cs_pin);

	/* convert the raw readings to physical units */
	int16_t raw_data_x = ((int16_t)(rx_buf[1])) | (((int16_t)(rx_buf[2])) << 8);
	int16_t raw_data_y = ((int16_t)(rx_buf[3])) | (((int16_t)(rx_buf[4])) << 8);
	int16_t raw_data_z = ((int16_t)(rx_buf[5])) | (((int16_t)(rx_buf[6])) << 8);

	*x = sensor->config->dps_range * (float)raw_data_x / (float)(1 << (LSM6DS3_RESOLUTION - 1));
	*y = sensor->config->dps_range * (float)raw_data_y / (float)(1 << (LSM6DS3_RESOLUTION - 1));
	*z = sensor->config->dps_range * (float)raw_data_z / (float)(1 << (LSM6DS3_RESOLUTION - 1));
}



HAL_StatusTypeDef LSM6DS3_ReadRegister(LSM6DS3* sensor, uint8_t reg, uint8_t* data)
{
	uint8_t tx_buf[2] = {(reg | 0x80), 0x00};  // set the first bit to indicate a read communication
	uint8_t rx_buf[2];

	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(sensor->spi, tx_buf, rx_buf, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

	*data = rx_buf[1];

	return status;
}


HAL_StatusTypeDef LSM6DS3_WriteRegister(LSM6DS3* sensor, uint8_t reg, uint8_t data)
{
	uint8_t tx_buf[2] = {reg, data};

	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(sensor->spi, tx_buf, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

	return status;
}


uint32_t LSM6DS3_GetInt1State(LSM6DS3* sensor)
{
	return GPIO_PIN_SET == HAL_GPIO_ReadPin(sensor->int1_port, sensor->int1_pin);
}


uint32_t LSM6DS3_GetInt2State(LSM6DS3* sensor)
{
	return GPIO_PIN_SET == HAL_GPIO_ReadPin(sensor->int2_port, sensor->int2_pin);
}
