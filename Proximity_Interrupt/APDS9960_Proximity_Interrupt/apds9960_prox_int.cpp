/*
 * APDS9960_Proximity.cpp
 *
 * Created: 05.08.2016 11:57:22
 * Author : Jo
 */ 


// last edited on 2018-06-28



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "APDS9960_AVR.h"

volatile uint8_t interrupt = 0;


ISR(INT3_vect)
{
	cli();							// disable global interrupts	
	interrupt = 1;					// interrupt event happening	
}


int main(void)
{
	//LED outputs
	DDRD |= (1 << DDD6) | (1 << DDD7);		// Pins PD6 and PD7 as OUTPUT for LEDs
	PORTD &= ~((1 << PD6) | (1 << PD7));	// On beginning both LEDs turned OFF

	//Interrupt setup	
	DDRD &= ~(1 << DDD3);			// input pin
	PORTD &= ~(1 << PD3);			// activate internal pull-up
	EICRA &= ~(1 << ISC30);			// interrupt detected
	EICRA |= (1 << ISC31);			// on falling edge
	EIMSK = (1 << INT3);			// enable external interrupt on INT3 pin PD3
	
	cli();							// before program starts make sure no interrupt is happening
	
	// APDS-9960 initialization	
	APDS9960_AVR sensor = APDS9960_AVR();
	sensor.set_power_on();
	sensor.set_pulseCount();			// Pulselength (PPLEN) and number (PPULSE) of radiated IR-beam pulses
	sensor.set_ControlRegisterOne();	// LED drive strength (LDRIVE 7:6), Proximity Gain Control (PGAIN 3:2)
	sensor.set_ConfigRegisterTwo();		// Additional LED drive current (LED_BOOST) during proximity LED pulses
	sensor.set_piht(DEFAULT_PIHT);		// Proximity Interrupt High Threshold
	sensor.set_pilt(DEFAULT_PILT);		// Proximity Interrupt Low Threshold
	sensor.set_pien();					//enable Proximity Detection and Proximity Interrupt
	
	sei();								// activate global interrupts
	uint8_t prox = 0;
	uint8_t max_piht = 0;
	
	
    while (1) 
    {
		if(interrupt)
		{
			prox = sensor.getProximity();
			// if PDATA exceeds DEFAULT_PIHT, turn ON LED green and set PIHT to maximum
			if((prox > DEFAULT_PIHT) && (!max_piht))
			{
				PORTD |= (1 << PD7);
				PORTD &= ~(1 << PD6);
				sensor.set_piht(MAX_PIHT);
				sensor.set_pilt(HYST_PILT);
				max_piht = 1;
				
			}
			else if((prox < DEFAULT_PIHT) && (max_piht))
			{
				max_piht = 0;
				PORTD &= ~(1 << PD7);
				PORTD |= (1 << PD6);
				sensor.set_piht(DEFAULT_PIHT);
				sensor.set_pilt(DEFAULT_PILT);
			}
			//_delay_ms(1000);
			//PORTD &= ~((1 << PD6) | (1 << PD7));
		
			
			prox = 0;
			interrupt = 0;
			sensor.clear_pint();
			sei();
		}
	}
}

