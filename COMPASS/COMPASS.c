/*
 * COMPASS.c
 *
 *  Created on: 7 jul. 2024
 *      Author: Franco
 */

#include "COMPASS.h"

hmc5883l_t magnetometer;

void hmc5883l_init(void){
	uint8_t txBuff[2] = {0,0};

//	txBuff[0] = 0x02;
//	txBuff[1] = 0x00; // continuous measurement mode
//	hmc5884l_init(txBuff, 2);

	txBuff[0] = 0xA;
	txBuff[1] = 0x80; // SET/RESET
	hmc5883l(txBuff, 2);

	txBuff[0] = 0x0B;
	txBuff[1] = 0x1; // SET/RESET
	hmc5883l(txBuff, 2);

	txBuff[0] = 0x09; //Config
	txBuff[1] = 0x1 | (0x2<<2) | (0x01<<4) | (0x0<<6); // continuous measurement mode
//	txBuff[1] = 0x1 | (0x2<<3) | (0x01<<4) | (0x0<<6); // continuous measurement mode
//	txBuff[1] = 0x1D;
	hmc5883l(txBuff, 2);

//	txBuff[0] = 0x00;
//	txBuff[1] = 0x70; // continuous measurement mode
//	hmc5883l(txBuff, 2);
//
//	txBuff[0] = 0x01;
//	txBuff[1] = 0xA0; // continuous measurement mode
//	hmc5883l(txBuff, 2);
//
//	txBuff[0] = 0x02;
//	txBuff[1] = 0x00; // continuous measurement mode
//	hmc5883l(txBuff, 2);
}

void hmc5883l(uint8_t* tx, uint8_t buff_size){
	I2C1_INIT_WRITE(HMC5883L);

	for(int i=0; i<buff_size; i++)I2C1_SEND_CONTINUE(tx[i]);


	I2C1_STOP();
}

void hmc5883l_update(int32_t *rxBuff){
	uint8_t txBuff[6] = {0x00, 0x1, 0x2, 0x3, 0x4, 0x5};
	for(int i = 0; i < 6; i++)
		{

		I2C1_INIT_WRITE(HMC5883L);

		hmc5883l(&txBuff[i],1);

		I2C1_INIT_READ(HMC5883L);

		I2C1_RECEIVE_CONTINUE(&rxBuff[i], 1);

		I2C1_STOP();

		}

}

void hmc5883l_azimuth(hmc5883l_t* sense){
	sense->azimuth= atan2((float)sense->my,(float)sense->mz) * 180.0 / PI;
	sense->azimuth+=sense->magnetic_declination;
		//sense->set_angle_objective -=
	if(sense->azimuth<0){
		sense->azimuth +=360;
	}
	sense->delta_angle = sense->set_angle_objective - sense->azimuth ; //Angular distance at desired orientation
}

void hmc5883l_get(hmc5883l_t* sense){
	int rxBuff[6];

	hmc5883l_update(rxBuff);

	sense->mx = (int16_t)(rxBuff[0] | rxBuff[1] << 8);
	sense->my= (int16_t)(rxBuff[2] | rxBuff[3] << 8);
	sense->mz= (int16_t)(rxBuff[4] | rxBuff[5] << 8);

}

//hmc5883l_t compass;
//
//void hmc5883l(uint8_t* tx, uint8_t buff_size){
//	I2C1_INIT_WRITE(HMC5883L);
//
//	for(int i=0; i<buff_size; i++)I2C1_SEND_CONTINUE(tx[i]);
//
//
//	I2C1_STOP();
//}
//
//
//void hmc5883l_init(void){
//	uint8_t txBuff[2] = {0,0};
//	//datasheet: https://qstcorp.com/upload/pdf/202202/13-52-04%20QMC5883L%20Datasheet%20Rev.%20A(1).pdf
//	uint8_t Mode = 0x01;
//	uint8_t ODR = 0x02;
//	uint8_t RNG = 0x01;
//	uint8_t OSR = 0x00;
////	txBuff[0] = 0x02;
////	txBuff[1] = 0x00; // continuous measurement mode
////	hmc5884l_init(txBuff, 2);
//
//	txBuff[0] = 0xA;
//	txBuff[1] = 0x80; // SET/RESET
//	hmc5883l(txBuff, 2);
//
//	txBuff[0] = 0x0B;
//	txBuff[1] = 0x1; // SET/RESET
//	hmc5883l(txBuff, 2);
//
//	txBuff[0] = 0x09; //Config
////	txBuff[1] =(OSR<<6) | (RNG<<4) | (ODR<<2) | (Mode);//
//	txBuff[1] = 0x1 | (0x2<<3) | (0x01<<4) | (0x0<<6); // continuous measurement mode
////	txBuff[1] = 0x1D;
//	hmc5883l(txBuff, 2);
//
////	txBuff[0] = 0x00;
////	txBuff[1] = 0x70; // continuous measurement mode
////	hmc5883l(txBuff, 2);
////
////	txBuff[0] = 0x01;
////	txBuff[1] = 0xA0; // continuous measurement mode
////	hmc5883l(txBuff, 2);
////
////	txBuff[0] = 0x02;
////	txBuff[1] = 0x00; // continuous measurement mode
////	hmc5883l(txBuff, 2);
//}
//
//void hmc5883l_update(int32_t *rxBuff){
//	uint8_t txBuff[6] = {0x00, 0x1, 0x2, 0x3, 0x4, 0x5};
//	for(int i = 0; i < 6; i++)
//		{
//
//		I2C1_INIT_WRITE(HMC5883L);
//
//		hmc5883l(&txBuff[i],1);
//
//		I2C1_INIT_READ(HMC5883L);
//
//		I2C1_RECEIVE_CONTINUE(&rxBuff[i], 1);
//
//		I2C1_STOP();
//
//		}
//
//}
//
//void hmc5883l_get(hmc5883l_t* sense){
//	int rxBuff[6];
//
//	hmc5883l_update(rxBuff);
//
//	sense->mx = (int16_t)(rxBuff[0] | rxBuff[1] << 8);
//	sense->my= (int16_t)(rxBuff[2] | rxBuff[3] << 8);
//	sense->mz= (int16_t)(rxBuff[4] | rxBuff[5] << 8);
//
//}
//
//void hmc5883l_azimuth(hmc5883l_t* sense){
//	sense->azimuth= atan2((float)sense->my,(float)sense->mz) * 180.0 / PI;
//	sense->azimuth+=sense->magnetic_declination;
//		//sense->set_angle_objective -=
//	if(sense->azimuth<0){
//		sense->azimuth +=360;
//	}
//	sense->delta_angle = sense->set_angle_objective - sense->azimuth ; //Angular distance at desired orientation
//}
//
//void SET_ANGLE_OBJECTIVE(pint_pin_int_t pintr, uint32_t pmatch_status){
//	hmc5883l_get(&compass);
//	hmc5883l_azimuth(&compass);
//	compass.set_angle_objective = compass.azimuth;
//
//	GPIO_PinWrite(GPIO, 1, 0, 0);
//	delay_ms(500);
//	GPIO_PinWrite(GPIO, 1, 0, 1);
//}
//
//
