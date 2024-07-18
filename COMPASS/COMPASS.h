/*
 * COMPASS.h
 *
 *  Created on: 7 jul. 2024
 *      Author: Franco A. López
 */

/**
 * @file    COMPASS.h
 * @brief   HMC5883L Handler.
 */

#ifndef COMPASS_H_
#define COMPASS_H_
#define HMC5883L  0x0D

#include "_I2C_.h"

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

/**
 * @brief Init communication and config
 *
 * @return void
 * */
void hmc5883l_init(void);

/**
 * @brief Call hmc5883l and send data
 *
 * @param tx               pointer to send buffer
 * @param buff_size buffer size
 *
 * @return void
 */
void hmc5883l(uint8_t*, uint8_t);

/**
 * @brief Update all measurements
 *
 * @param rxBuff Pointer to rx buffer
 *
 */
void hmc5883l_update(int32_t *);

/**
 * @brief Get azimuth
 *
 * @param sense Pointer to variable to storage
 */
void hmc5883l_azimuth(hmc5883l_t*);

/**
 * @brief Get the measurements
 */
void hmc5883l_get(hmc5883l_t*);

#endif /* COMPASS_H_ */
