/*
 * APDS9960_Proximity.cpp
 *
 * Created: 05.08.2016 11:57:22
 * Author : Jo
 */ 

#include <avr/io.h>
#include "APDS9960_AVR.h"

int main(void)
{
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
	
	APDS9960_AVR sensor = APDS9960_AVR();
	sensor.set_power_on();
	sensor.set_pulseCount();
	sensor.set_ControlRegisterOne();
	sensor.set_ConfigRegisterTwo();
	sensor.enable_proximity();
    while (1) 
    {
		
	}

return 0;

}

