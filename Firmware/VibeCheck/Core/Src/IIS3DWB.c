/*
 * IIS3DWB.c
 *
 *  Created on: Jul 16, 2024
 *      Author: johnt
 */

#include "IIS3DWB.h"
#include "fast_spi.h"

uint8_t IIS3DWB_Init(IIS3DWB* sensor, IIS3DWB_Config* config, SPI_HandleTypeDef* spi,
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

	sensor->accel_x_g = 0.0f;
	sensor->accel_y_g = 0.0f;
	sensor->accel_z_g = 0.0f;

	/* verify that we can communicate with the sensor */
	uint8_t err_num = 0;
	HAL_StatusTypeDef status;
	uint8_t reg_data;

	status = IIS3DWB_ReadRegister(sensor, IIS3DWB_REG_WHO_AM_I, &reg_data);
	err_num += (status != HAL_OK);
	if (reg_data != IIS3DWB_DEVICE_ID)
	{
		return 255;  /* return with error code if we receive unexpected device ID */
	}

	/* power down the device (p. 32) */
	reg_data = IIS3DWB_ODR_DISABLE;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL1_XL, &reg_data);
	err_num += (status != HAL_OK);

	/* disable the I2C interface (p. 34) */
	reg_data = 0x04;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL4_C, &reg_data);
	err_num += (status != HAL_OK);

	/* select three axis sampling mode and weight of user offsets (p. 35) */
	reg_data = 0x00;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL6_C, &reg_data);
	err_num += (status != HAL_OK);

	/* set the data-ready interrupt to pulse mode (p. 30) */
	reg_data = 0x80;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_COUNTER_BDR_REG1, &reg_data);
	err_num += (status != HAL_OK);

	/* enable the data-ready interrupt on INT1 pin (p. 31) */
	reg_data = 0x01;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_INT1_CTRL, &reg_data);
	err_num += (status != HAL_OK);

	/* enable the low pass filter with ODR/20 cutoff (p. 36) */
	/* TODO: set the LPF cutoff in the user configuration */
	reg_data = 0x40;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL8_XL, &reg_data);
	err_num += (status != HAL_OK);

	/* enable the user offset correction block (p. 35) */
	reg_data = 0x02;
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL7_C, &reg_data);
	err_num += (status != HAL_OK);

	/* TODO: write the user offsets */

	/* enable the sensor with the configured range (p. 32) */
	uint8_t range_data;
	switch(sensor->config->g_range)
	{
	case 2:
		range_data = IIS3DWB_G_RANGE_2;
		break;
	case 4:
		range_data = IIS3DWB_G_RANGE_4;
		break;
	case 8:
		range_data = IIS3DWB_G_RANGE_8;
		break;
	case 16:
		range_data = IIS3DWB_G_RANGE_16;
		break;
	default:
		range_data = IIS3DWB_G_RANGE_2;
		break;
	}

	reg_data = (IIS3DWB_ODR_26667HZ | range_data);
	status = IIS3DWB_WriteRegister(sensor, IIS3DWB_REG_CTRL1_XL, &reg_data);
	err_num += (status != HAL_OK);

	return err_num;
}

void IIS3DWB_ReadAcceleration(IIS3DWB* sensor)
{
	/* read multiple bytes corresponding to the raw accelerometer data */
	uint8_t tx_buf[7] = {(IIS3DWB_REG_OUTX_L_XL | 0x80), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t rx_buf[7];

	/* use our fast SPI implementation instead of the HAL call */
	SPI_TxRx_Fast(tx_buf, rx_buf, 7, sensor->spi->Instance, sensor->cs_port, sensor->cs_pin);

	/* convert the raw readings to physical units */
	int16_t raw_data_x = ((int16_t)(rx_buf[1])) | (((int16_t)(rx_buf[2])) << 8);
	int16_t raw_data_y = ((int16_t)(rx_buf[3])) | (((int16_t)(rx_buf[4])) << 8);
	int16_t raw_data_z = ((int16_t)(rx_buf[5])) | (((int16_t)(rx_buf[6])) << 8);

	sensor->accel_x_g = sensor->config->g_range * (float)raw_data_x / (float)(1 << (IIS3DWB_RESOLUTION - 1));
	sensor->accel_y_g = sensor->config->g_range * (float)raw_data_y / (float)(1 << (IIS3DWB_RESOLUTION - 1));
	sensor->accel_z_g = sensor->config->g_range * (float)raw_data_z / (float)(1 << (IIS3DWB_RESOLUTION - 1));

}

HAL_StatusTypeDef IIS3DWB_ReadRegister(IIS3DWB* sensor, uint8_t reg, uint8_t* data)
{
	uint8_t tx_buf[2] = {(reg | 0x80), 0x00};  // set the first bit to indicate a read communication
	uint8_t rx_buf[2];

	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(sensor->spi, tx_buf, rx_buf, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

	*data = rx_buf[1];

	return status;
}

HAL_StatusTypeDef IIS3DWB_WriteRegister(IIS3DWB* sensor, uint8_t reg, uint8_t* data)
{
	uint8_t tx_buf[2] = {reg, *data};

	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(sensor->spi, tx_buf, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

	return status;
}

uint8_t IIS3DWB_GetInt1State(IIS3DWB* sensor)
{
	return GPIO_PIN_SET == HAL_GPIO_ReadPin(sensor->int1_port, sensor->int1_pin);
}

uint8_t IIS3DWB_GetInt2State(IIS3DWB* sensor)
{
	return GPIO_PIN_SET == HAL_GPIO_ReadPin(sensor->int2_port, sensor->int2_pin);
}
