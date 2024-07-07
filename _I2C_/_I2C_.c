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

void hmc5883l(uint8_t* tx, uint8_t buff_size){
	I2C1_INIT_WRITE(HMC5883L);

	for(int i=0; i<buff_size; i++)I2C1_SEND_CONTINUE(tx[i]);


	I2C1_STOP();
}

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
	txBuff[1] = 0x1 | (0x2<<3) | (0x00<<4) | (0x2<<6); // continuous measurement mode
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

void hmc5883l_get(hmc5883l_t* sense){
	int rxBuff[6];

	hmc5883l_update(rxBuff);

	sense->mx = (int16_t)(rxBuff[0] | rxBuff[1] << 8);
	sense->my= (int16_t)(rxBuff[2] | rxBuff[3] << 8);
	sense->mz= (int16_t)(rxBuff[4] | rxBuff[5] << 8);

	sense->azimuth= atan2(sense->my,sense->mx) * 180.0 / PI;
	sense->azimuth+=sense->magnetic_declination;
	//if(sense->azimuth<0){
	//	sense->azimuth +=360;
	//}
}

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
		PRINTF("%s : -%d\r\n",text, num);
		return;
	}
	else{
		PRINTF("%s : %d\r\n",text, num);
		return;
	}
}


#endif /* I2C__C_ */
