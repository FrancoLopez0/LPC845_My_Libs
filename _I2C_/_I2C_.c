/*
 * _I2C_.c
 *
 *  Created on: 6 jul. 2024
 *      Author: Franco
 */


#ifndef I2C__C_
#define I2C__C_
#include "_I2C_.h"

int32_t rxBuff[14];
//hmc5883l_t magnetometer;



void I2C1_INIT_WRITE(uint8_t address){
	while ((I2C1->STAT & 0x1) == 0); //WAIT STATE PENDING
	I2C1->MSTDAT = (address << 1);   //MPU6050 ADRESS
	I2C1->MSTCTL = 0x2;			     //START (MSTART)
	delay_ms(I2C_WAIT);
}

void I2C1_INIT_READ(uint8_t address){
	while ((I2C1->STAT & 0x1) == 0); //WAIT STATE PENDING
	I2C1->MSTDAT = (address << 1) | 1;//BIT READ
	I2C1->MSTCTL = 0x2;			     //START (MSTART)
	delay_ms(I2C_WAIT);
}

void I2C1_SEND_CONTINUE(uint8_t data){
	while ((I2C1->STAT & 0x1) == 0);
	I2C1->MSTDAT = data;				//DATA TO SEND
	I2C1->MSTCTL = 1;				//CONTINUE (MSTCONTINUE)
	delay_ms(I2C_WAIT);
}

void I2C1_RECEIVE_CONTINUE(int32_t *rx, uint8_t buff_size){
	while ((I2C1->STAT & 0x1) == 0);
	for(int i=0; i< buff_size; i++)
	{
		rx[i] = I2C1->MSTDAT;				//DATA TO SEND
		I2C1->MSTCTL = 1;				//CONTINUE (MSTCONTINUE)

	}
	delay_ms(I2C_WAIT);
}

void I2C1_STOP(void){
	while((I2C1->STAT & 0x1)==0);
	I2C1->MSTCTL = 0x4;				//STOP (MSTSTOP)
	while((I2C1->STAT & 0x1) == 0);
}

void mpu6050(uint8_t* tx, uint8_t buff_size){
	I2C1_INIT_WRITE(MPU6050);

	for(int i=0; i<buff_size; i++)I2C1_SEND_CONTINUE(tx[i]);

	I2C1_STOP();
}
////
//void hmc5883l(uint8_t* tx, uint8_t buff_size){
//	I2C1_INIT_WRITE(HMC5883L);
//
//	for(int i=0; i<buff_size; i++)I2C1_SEND_CONTINUE(tx[i]);
//
//
//	I2C1_STOP();
//}

void mpu6050_init(void){

	uint8_t txBuff[2] = {0,0};

	txBuff[0] = PWR_MNG_MPU6050;
	txBuff[1] = RESET_MPU6050;
	mpu6050(txBuff, 2);

	txBuff[0] = RST_SIGNAL_CFG_MPU6050;
	txBuff[1] = RST_ALL_SIGNALS_MPU6050;
	mpu6050(txBuff,2);

	txBuff[0] = PWR_MNG_MPU6050;
	txBuff[1] = 0x00; //Internal Clock
	mpu6050(txBuff,2);

	txBuff[0] = 0x1C;
	txBuff[1] = 0x08;
	mpu6050(txBuff,2);

	txBuff[0] = 0x1B;
	txBuff[1] = 0x02<<3;
	//txBuff[1] = 0x08;
	mpu6050(txBuff,2);

	txBuff[0] = 0x23;
	txBuff[1] = 0x08;
	mpu6050(txBuff,2); //ENABLE FIFO ACC
}

//void hmc5883l_init(void){
//	uint8_t txBuff[2] = {0,0};
//
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
//	txBuff[1] = 0x1 | (0x2<<2) | (0x01<<4) | (0x0<<6); // continuous measurement mode
////	txBuff[1] = 0x1 | (0x2<<3) | (0x01<<4) | (0x0<<6); // continuous measurement mode
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

void mpu6050_update(int32_t *rxBuff, uint8_t buff_size){
	uint8_t txBuff[6] = {0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40};

	for(int i = 0; i < 6; i++)
	{

	I2C1_INIT_WRITE(MPU6050);

	mpu6050(&txBuff[i],1);

	I2C1_INIT_READ(MPU6050);

	I2C1_RECEIVE_CONTINUE(&rxBuff[i], 1);

	I2C1_STOP();

	}
}

void I2C1_INIT(void){
	 SYSCON->PRESETCTRL0 &= ~0x200000; 			// reset del I2C1
	 SYSCON->PRESETCTRL0 |= 0x200000; 			// permite el uso I2C1
	 SYSCON->SYSAHBCLKCTRL0 |= 0x200000;
	 SYSCON->FCLKSEL[6] = 0;
	 SYSCON->SYSAHBCLKCTRL0 |= 0x80;
	 SWM0->PINASSIGN.PINASSIGN9 = 0x1112FFFF;    //SCL:0x11 = pin 17 SDA:0x12 = pin 18
	 SYSCON->SYSAHBCLKCTRL0 |= ~(0x80);

	 I2C1->CFG = 1;
	 I2C1->CLKDIV = 2;
}

void _printf_var_int(char* text,int num){
	if(num<0){
		num = ~num;
		PRINTF("\"%s\" : \"-%d\"", text, num);
//		PRINTF("%s : -%d\r\n",text, num);
		return;
	}
	else{
//		PRINTF("%s : %d\r\n",text, num);
		PRINTF("\"%s\" : \"%d\"", text, num);
		return;
	}
}

//@brief Example
//
// int main(void) {
//	BOARD_InitBootPins();
//	BOARD_InitBootClocks();
//	BOARD_InitBootPeripherals();
//	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
//	        /* Init FSL debug console. */
//	    BOARD_InitDebugConsole();
//	#endif
//
//	SYSTICK_INIT();
//
//    I2C1_INIT();
//
//    hmc5883l_init();
//
//    hmc5883l_update(rxBuff);
//
//    //mpu6050_init();
//
//    //mpu6050_update(rxBuff,14);
//    // mpu6050_t mpu;
//
//   // mpu.aY = 23.0;
//    hmc5883l_t magnetometer;
//    magnetometer.magnetic_declination = -10.2;
//    while(1) {
//
//    	hmc5883l_get(&magnetometer);
//
//    	//mpu6050_update(rxBuff,14);
//
////        read = rxBuff[0]<<8 | rxBuff[1];
////        read = rxBuff[0]<<8 | rxBuff[1];
////        mpu.aX = read / 8192;
//
////        read = rxBuff[2]<<8 | rxBuff[3];
////        mpu.aY = ((float)read) / 8192;
//
//        //PRINTF("==========================\r\n");
//
////
////        PRINTF("AX: %f", mpu.aX);
////        PRINTF("AY: %f", mpu.aY);
////        PRINTF("X_H:%d\r\n", rxBuff[0]);
////        PRINTF("X_L:%d\r\n", rxBuff[1]);
////
////        PRINTF("Y_H:%d\r\n", rxBuff[2]);
////        PRINTF("Y_L:%d\r\n", rxBuff[3]);
////
////        PRINTF("Z_H:%d\r\n", rxBuff[4]);
////        PRINTF("Z_L:%d\r\n", rxBuff[5]);
////    	PRINTF("RAW x: %d\t", raw[0]);
//    	_printf_var_int("RAW x", magnetometer.mx);
//    	//PRINTF("RAW y: %d\t", raw[1]);
//    	_printf_var_int("RAW y", magnetometer.my);
//    	//PRINTF("RAW CA2 x: -%d\t", raw[1]);
//
////    	PRINTF("RAW z: %d\t", raw[2]);
//    	_printf_var_int("RAW z", magnetometer.mz);
//    	_printf_var_int("AZIMUTH", magnetometer.azimuth);
//    	//PRINTF("HEADING: %d\r\n", heading);
//
////
////        PRINTF("Y_TOT:%f\r\n", mpu.aY);
////
////        PRINTF("==========================\r\n");
//        //PRINTF("acc_z:%d\r\n", rxBuff[0]);
//        //delay_ms(100);
//    }
//    return 0 ;
//}
//
#endif /* I2C__C_ */
