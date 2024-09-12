/*
 * IIS3DWB.h
 *
 *  Created on: Jul 16, 2024
 *      Author: johnt
 */

#ifndef INC_IIS3DWB_H_
#define INC_IIS3DWB_H_

/* refactor for the LSM6DS3 */

#include "main.h"

/* device defines */
#define IIS3DWB_DEVICE_ID 0x7B  /* fixed value of WHO_AM_I register */
#define IIS3DWB_RESOLUTION 16  /* bit depth of sensor */

/* device register addresses (p.26) */
#define IIS3DWB_REG_WHO_AM_I 		 0x0F
#define IIS3DWB_REG_INT1_CTRL 		 0x0D
#define IIS3DWB_REG_CTRL1_XL  		 0x10
#define IIS3DWB_REG_CTRL4_C  		 0x13
#define IIS3DWB_REG_CTRL6_C 		 0x15
#define IIS3DWB_REG_CTRL7_C 		 0x16
#define IIS3DWB_REG_CTRL8_XL         0x17
#define IIS3DWB_REG_COUNTER_BDR_REG1 0x0B
#define IIS3DWB_REG_STATUS_REG       0x1E
#define IIS3DWB_REG_OUTX_L_XL 0x28  /* accelerometer output registers */
#define IIS3DWB_REG_OUTX_H_XL 0x29
#define IIS3DWB_REG_OUTY_L_XL 0x2A
#define IIS3DWB_REG_OUTY_H_XL 0x2B
#define IIS3DWB_REG_OUTZ_L_XL 0x2C
#define IIS3DWB_REG_OUTZ_H_XL 0x2D
#define IIS3DWB_REG_X_OFS_USR 0x73  /* user offsets */
#define IIS3DWB_REG_Y_OFS_USR 0x74
#define IIS3DWB_REG_Z_OFS_USR 0x75

/* register values */
#define IIS3DWB_ODR_DISABLE 0b00000000  /* CTRL1_XL register */
#define IIS3DWB_ODR_26667HZ 0b10100010  /* also enables the LPF2 path */
#define IIS3DWB_G_RANGE_2  0b00000000  /* CTRL1_XL register */
#define IIS3DWB_G_RANGE_4  0b00001000
#define IIS3DWB_G_RANGE_8  0b00001100
#define IIS3DWB_G_RANGE_16 0b00000100

typedef struct
{
	/* TODO: add user offsets and LPF settings */
	uint32_t g_range; /* 2, 4, 8, or 16 */

} IIS3DWB_Config;

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
	IIS3DWB_Config* config;

	/* acceleration reading */
	volatile float accel_x_g;
	volatile float accel_y_g;
	volatile float accel_z_g;

} IIS3DWB;

uint8_t IIS3DWB_Init(IIS3DWB* sensor, IIS3DWB_Config* config, SPI_HandleTypeDef* spi,
		GPIO_TypeDef* cs_port, uint16_t cs_pin,
		GPIO_TypeDef* int1_port, uint16_t int1_pin,
		GPIO_TypeDef* int2_port, uint16_t int2_pin);
void IIS3DWB_ReadAcceleration(IIS3DWB* sensor);

HAL_StatusTypeDef IIS3DWB_ReadRegister(IIS3DWB* sensor, uint8_t reg, uint8_t* data);
HAL_StatusTypeDef IIS3DWB_WriteRegister(IIS3DWB* sensor, uint8_t reg, uint8_t* data);

uint8_t IIS3DWB_GetInt1State(IIS3DWB* sensor);  /* read the state of the chip interrupt pins */
uint8_t IIS3DWB_GetInt2State(IIS3DWB* sensor);


#endif /* INC_IIS3DWB_H_ */
