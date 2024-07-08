/*
 * _I2C_.h
 *
 *  Created on: 6 jul. 2024
 *      Author: Franco
 */
#define I2C_WAIT 10 //ms
#define I2C_MASTER_CLK 100000

#define PI 3.14

#define MPU6050 0x68
#define HMC5883L  0x0D
//#define QMC5883
#define RESET_MPU6050 0b10000000
#define PWR_MNG_MPU6050 0x6B
#define RST_SIGNAL_CFG_MPU6050 0x68
#define RST_ALL_SIGNALS_MPU6050 0x7
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "math.h"
#include "DELAY.h"
#include "fsl_pint.h"
#include "fsl_gpio.h"
extern int32_t rxBuff[14];

typedef struct{
	int mx;
	int my;
	int mz;
	float azimuth;
	float magnetic_declination;
	int set_angle_objective;
	int delta_angle;
}hmc5883l_t;

extern hmc5883l_t magnetometer;
typedef struct{
	float aX;
	float aY;
	float aZ;
}mpu6050_t;



void I2C1_INIT_WRITE(uint8_t);

void I2C1_INIT_READ(uint8_t);

void I2C1_SEND_CONTINUE(uint8_t);

void I2C1_RECEIVE_CONTINUE(int32_t *, uint8_t);

void I2C1_STOP(void);

void mpu6050(uint8_t*, uint8_t);

void hmc5883l(uint8_t*, uint8_t);

void mpu6050_init(void);

void hmc5883l_init(void);

void hmc5883l_azimuth(hmc5883l_t*);

void hmc5883l_update(int32_t *);

void SET_ANGLE_OBJECTIVE(pint_pin_int_t, uint32_t);

void hmc5883l_get(hmc5883l_t*);

void mpu6050_update(int32_t *, uint8_t );

void I2C1_INIT(void);

void _printf_var_int(char*, int);
