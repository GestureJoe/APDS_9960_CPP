/*
 * APDS9960_AVR.cpp
 *
 * Created: 05.08.2016 11:58:03
 *  Author: Jo
 */ 

#include "APDS9960_AVR.h"
#include <avr/io.h>

APDS9960_AVR::APDS9960_AVR()
{
	/*****************************************
	*
	* Initialize the I2C Interface of the MCU
	*
	*****************************************/
			
	// initialize TWI clock: 
	
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));								// no prescaler 
	TWBR = ((16000000UL/SCL_CLOCK)-16)/2;  // must be > 10 for stable operation 
	
	
	//Initialize Errorcode LED
	/*DDRF |= (1<<DDF0);*/
	
	//and Error Flag
	error_i2c = false;
	
	//Initialize all class members
	enable = 0;
	status = 0;
	
	pers= 0;
	pulseCount = 0;
	controlOne = 0;
	configTwo = 0;
	
	pilt = 0;
	piht = 0;
	ppers = 0;
	apers = 0;
	pplen = 0;
	ppulse = 0;
	ldrive = 0;
	pgain = 0;
	led_boost = 0;
	pint = 0;
	aint = 0;
	gint = 0;
	pvalid = 0;
	pData = 0;
	poffset_ur = 0;
	poffset_dl = 0;
	
	
	
	fifoData[128] = {0};
	gfifolvl = 0;
	gfifo_up[32] = {0};
	gfifo_down[32] = {0};
	gfifo_left[32] = {0};
	gfifo_right[32] = {0};
}

APDS9960_AVR::~APDS9960_AVR()
{
		
}


bool APDS9960_AVR::get_errorStatus()
{
	return error_i2c;
}


void APDS9960_AVR::initForProximity()
{
	if(!set_power_on())
	{
		
	}
	
	if(!set_pulseCount())
	{
		
	}
	
	if(!set_ControlRegisterOne())
	{
		
	}
	
	if(!set_ConfigRegisterTwo())
	{
		
	}
	
	if(!set_proxOff_UR())
	{
		
	}
	
	if(!set_proxOff_DL())
	{
		
	}
	
	if(!enable_proximity())
	{
		
	}
}
	
	
void APDS9960_AVR::initForGesture()
{
	
}
	
	
void APDS9960_AVR::initForALS()
{
	
}
	
	
bool APDS9960_AVR::enable_proximity()
{
	if(!i2c_avr_write_byte_data(APDS9960_ENABLE, ENABLE_PROXIMITY))
	{
		return false;
	}
	else
	{
		return true;
	}	
}
	
	
bool APDS9960_AVR::enable_als()
{
	if(!i2c_avr_write_byte_data(APDS9960_ENABLE, ENABLE_ALS))
	{
		return false;
	}
	else
	{
		return true;
	}	
}
	
	
bool APDS9960_AVR::enable_gesture()
{
	if(!i2c_avr_write_byte_data(APDS9960_ENABLE, ENABLE_GESTURE))
	{
		return false;
	}
	else
	{
		return true;
	}	
}
	
	
bool APDS9960_AVR::set_power_on()
{
	if(!i2c_avr_write_byte_data(APDS9960_ENABLE, ENABLE_POWERON))
	{
		return false;
	}
	else
	{
		return true;
	}	
}


//Read out Status Register (0x93)
//PINT <5>
//AINT <4>
//GINT <2>
//PVALID <1>
uint8_t APDS9960_AVR::get_status()
{
	status = i2c_avr_read_byte_data(APDS9960_STATUS);
	
	return status;
}
	

//---------------------------------------------------------------------------------------/	
//                    Proximity Engine related                                           /
//---------------------------------------------------------------------------------------/	

//Sets the Proximity Interrupt Low Threshold
//Register 0x89 <7:0>
bool APDS9960_AVR::set_pilt()
{
	if(!i2c_avr_write_byte_data(APDS9960_PILT, DEFAULT_PILT))
	{
		return false;
	}
	else
	{
		return true;
	}
}

uint8_t APDS9960_AVR::get_pilt()
{
	pilt = i2c_avr_read_byte_data(APDS9960_PILT);
	
	return pilt;
}




//Sets the Proximity Interrupt High Threshold
//Register 0x8B <7:0>
bool APDS9960_AVR::set_piht()
{
	if(!i2c_avr_write_byte_data(APDS9960_PIHT, DEFAULT_PIHT))
	{
		return false;
	}
	else
	{
		return true;
	}
}

uint8_t APDS9960_AVR::get_piht()
{
	piht = i2c_avr_read_byte_data(APDS9960_PIHT);
	
	return piht;
}




//Sets the number of PDATA values that have to be out of range before Interrupt is generated
//Register 0x8C <7:4>
bool APDS9960_AVR::set_pers()
{
	if(!i2c_avr_write_byte_data(APDS9960_PERS, DEFAULT_PPERS))
	{
		return false;
	}

	return true;
	
}

uint8_t APDS9960_AVR::get_pers()
{
	pers = i2c_avr_read_byte_data(APDS9960_PERS);
	
	return pers;
}




//Sets the IR-LED pulse length <7:6> and number of pulses <5:0>
//Register 0x8E
bool APDS9960_AVR::set_pulseCount()
{
	if(!i2c_avr_write_byte_data(APDS9960_PPULSE, DEFAULT_PPULSE))
	{
		return false;
	}
	else
	{
		return true;
	}
		
}

uint8_t APDS9960_AVR::get_pulseCount()
{
	pulseCount = i2c_avr_read_byte_data(APDS9960_PPULSE);
	
	return pulseCount;
}
	

	
	
//Sets the IR-LED current <7:6> and Proximity Gain <3:2>	
//Register 0x8F
bool APDS9960_AVR::set_ControlRegisterOne()
{
	if(!i2c_avr_write_byte_data(APDS9960_CONTROL, DEFAULT_CONTROL))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Gets the IR-LED current <7:6> and Proximity Gain <3:2>	
//Register 0x8F
uint8_t APDS9960_AVR::get_ControlRegisterOne()
{
	controlOne = i2c_avr_read_byte_data(APDS9960_CONTROL);
	
	return controlOne;
}
	

	
	
//Sets the IR-LED Boost <5:4>
//Register 0x90	
bool APDS9960_AVR::set_ConfigRegisterTwo()
{
	if(!i2c_avr_write_byte_data(APDS9960_CONFIG2, DEFAULT_CONFIG2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Gets the IR-LED Boost <5:4>
//Register 0x90	
uint8_t APDS9960_AVR::get_ConfigRegisterTwo()
{
	configTwo = i2c_avr_read_byte_data(APDS9960_CONFIG2);
	
	return configTwo;
}




//Read out Proximity value
uint8_t APDS9960_AVR::getProximity()
{
	pData = i2c_avr_read_byte_data(APDS9960_PDATA);
	
	return pData;
}	


//Sets the Proximity Offset of UP/RIGHT	
bool APDS9960_AVR::set_proxOff_UR()
{
	if(!i2c_avr_write_byte_data(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Gets the Proximity Offset of UP/RIGHT	
uint8_t APDS9960_AVR::get_proxOff_UR()
{
	poffset_ur = i2c_avr_read_byte_data(APDS9960_POFFSET_UR);
	
	return poffset_ur;
}
	

	
	
//Sets the Proximity Offset of DOWN/LEFT
bool APDS9960_AVR::set_proxOff_DL()
{
	if(!i2c_avr_write_byte_data(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL))
	{
		return false;
	}
	else
	{
		return true;
	}	
}

//Gets the Proximity Offset of DOWN/LEFT
uint8_t APDS9960_AVR::get_proxOff_DL()
{
	poffset_dl = i2c_avr_read_byte_data(APDS9960_POFFSET_DL);
	
	return poffset_dl;
}
	
	
//ALS Engine related

//---------------------------------------------------------------------------------------/	
//                    Gesture Engine related                                           /
//---------------------------------------------------------------------------------------/	

bool APDS9960_AVR::set_genth()
{return true;}


bool APDS9960_AVR::set_gexth()
{return true;}

bool APDS9960_AVR::set_gconf_one()
{return true;}

bool APDS9960_AVR::set_gconf_two()
{return true;}

bool APDS9960_AVR::set_goffset_u()
{return true;}

bool APDS9960_AVR::set_goffset_d()
{return true;}

bool APDS9960_AVR::set_goffset_l()
{return true;}

bool APDS9960_AVR::set_goffset_r()
{return true;}

bool APDS9960_AVR::set_gPulseCount()
{return true;}

bool APDS9960_AVR::set_gconf_four()
{return true;}

bool APDS9960_AVR::get_gstatus()
{
	return true;
}



/************************************************************
*
* Stop I2C Transmission to release bus
*
*************************************************************/
void APDS9960_AVR::i2c_stop(void)
{
    // send stop condition 
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}// i2c_stop 


/**********************************************************************************
*
* Error subroutine for errors occured during TWI communication
*
***********************************************************************************/

void APDS9960_AVR::i2c_error()
{
	PORTF |= (1<<PF0);
	_delay_ms(250);
	PORTF &= ~(1<<PF0);
	_delay_ms(250);
}


//I2C Communication
bool APDS9960_AVR::i2c_avr_write_byte_data(uint8_t reg, uint8_t command)
{
	uint8_t twi_status;		//holds the status of the TWI bus
	

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while (!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;//TW_STATUS & 0xF8;
	if ((twi_status != TW_START) && (twi_status != TW_REP_START)) 
	{
		//error_i2c = true;
		PORTD |= (1 << PD4);
		i2c_stop();
		return false;
	}
	
	// send device slave-address
	TWDR = (APDS9960_I2C_ADDR<<1) | TW_WRITE;
	//TWDR = (APDS9960_I2C_ADDR+TW_WRITE);
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;
	if (twi_status != TW_MT_SLA_ACK)// || (twi_status != TW_MR_SLA_ACK) )
	{
		//error_i2c = true;
		PORTD |= (1 << PD5);
		i2c_stop();
		return false;
	} 

	
	// address register of the previously addressed device
	TWDR = reg;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twi_status = TW_STATUS & 0xF8;
	if( twi_status != TW_MT_DATA_ACK) 
	{
		//error_i2c = true;
		PORTD |= (1 << PD6);
		i2c_stop();
		return false;
	}

	
	// send command to the previously addressed register of addressed device
	TWDR = command;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twi_status = TW_STATUS & 0xF8;
	if( twi_status != TW_MT_DATA_ACK)
	{
		//error_i2c = true;
		PORTD |= (1 << PD7);
		i2c_stop();
		return false;
	}
	
	error_i2c = false;
	i2c_stop();
	return true;	
}
	
	
uint8_t APDS9960_AVR::i2c_avr_read_byte_data(uint8_t reg)
{
	uint8_t twi_status;			//holds the status of the TWI bus
	uint8_t reg_value = 0;		//holds the data(s) provided by the APDS-9960 Slave
	

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;
	if(twi_status != TW_START)
	{
		error_i2c = true;
		i2c_stop();
	}
		

	// send device slave-address + WRITE-BIT
	TWDR = (APDS9960_I2C_ADDR<<1) | TW_WRITE;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;
	if(twi_status != TW_MT_SLA_ACK)
	{
		error_i2c = true;
		i2c_stop();
	} 

	
	// send register address of slave addressed before
	TWDR = reg;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twi_status = TW_STATUS & 0xF8;
	if(twi_status != TW_MT_DATA_ACK) 
	{
		error_i2c = true;
		i2c_stop();
	}

	
	// send repeated START-condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;
	if (twi_status != TW_REP_START)
	{
		error_i2c = true;
		i2c_stop();
	}

	
	// send Slave Address + Read-Bit
	TWDR = (APDS9960_I2C_ADDR<<1) | TW_READ;
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twi_status = TW_STATUS & 0xF8;
	if(twi_status != TW_MR_SLA_ACK)
	{
		error_i2c = true;
		i2c_stop();
	} 

	
	//Read one byte from Slave's register addressed before
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	reg_value = TWDR;
	error_i2c = false;	
	return reg_value;	
}
	
	
uint8_t APDS9960_AVR::i2c_avr_read_block_data(uint8_t register, uint8_t length, uint8_t *data)
{
	return 0;	
}
