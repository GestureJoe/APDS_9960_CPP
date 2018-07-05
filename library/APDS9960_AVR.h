/*
 * APDS9960_AVR.h
 *
 * Created: 05.08.2016 11:57:47
 *  Author: Jo
 */ 


#ifndef APDS9960_AVR_H_
#define APDS9960_AVR_H_

#define F_CPU 32000000

//I2C clock in Hz
#define SCL_CLOCK  400000L

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdio.h>

//APDS-9960 I2C Address
#define APDS9960_I2C_ADDR 0x39

//Constants for APDS-9960 Register Addressses
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_AILTL          0x84
#define APDS9960_AILTH          0x85
#define APDS9960_AIHTL          0x86
#define APDS9960_AIHTH          0x87
#define APDS9960_PILT           0x89
#define APDS9960_PIHT           0x8B
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONTROL        0x8F
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_STATUS         0x93
#define APDS9960_CDATAL         0x94
#define APDS9960_CDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_PDATA          0x9C
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F
#define APDS9960_GPENTH         0xA0
#define APDS9960_GEXTH          0xA1
#define APDS9960_GCONF1         0xA2
#define APDS9960_GCONF2         0xA3
#define APDS9960_GOFFSET_U      0xA4
#define APDS9960_GOFFSET_D      0xA5
#define APDS9960_GOFFSET_L      0xA7
#define APDS9960_GOFFSET_R      0xA9
#define APDS9960_GPULSE         0xA6
#define APDS9960_GCONF3         0xAA
#define APDS9960_GCONF4         0xAB
#define APDS9960_GFLVL          0xAE
#define APDS9960_GSTATUS        0xAF
#define APDS9960_IFORCE         0xE4
#define APDS9960_PICLEAR        0xE5
#define APDS9960_CICLEAR        0xE6
#define APDS9960_AICLEAR        0xE7
#define APDS9960_GFIFO_U        0xFC
#define APDS9960_GFIFO_D        0xFD
#define APDS9960_GFIFO_L        0xFE
#define APDS9960_GFIFO_R        0xFF

//Bit fields for ENABLE Register
#define ENABLE_GESTURE		0x45
#define ENABLE_PROXIMITY	0x05
#define ENABLE_ALS			0x03
#define ENABLE_PROX_INT		0x25
#define ENABLE_ALS_INT		0x13
#define ENABLE_POWERON		0x01

//Constants for Sensor system Parameters
//Proximity Engine Constants
#define DEFAULT_PPULSE		0x90	//16 Proximity Pulses, 16µs pulse length
#define DEFAULT_CONTROL		0x08	//100mA, 4x PS Gain
#define DEFAULT_CONFIG2		0x30	//LED-Boost 300%
#define DEFAULT_POFFSET_UR	6
#define DEFAULT_POFFSET_DL	6
#define DEFAULT_PILT		0
#define DEFAULT_PIHT		50
#define DEFAULT_PPERS		1

//Gesture Engine Constants
#define DEFAULT_GPENTH		45
#define DEFAULT_GEXTH		50
#define DEFAULT_GCONF1		0x40
#define DEFAULT_GCONF2		0x40
#define DEFAULT_GOFFSET_U	1
#define DEFAULT_GOFFSET_D	1
#define DEFAULT_GOFFSET_L	1
#define DEFAULT_GOFFSET_R	1
#define DEFAULT_GLED		0xD0
#define DEFAULT_GCONF4		0x02

//Gesture Engine bit fields
#define APDS9960_GVALID 0b00000001
#define APDS9960_GFOV	0b00000010


class APDS9960_AVR
{
public:
	APDS9960_AVR();
	~APDS9960_AVR();
	
	void initForProximity();
	void initForGesture();
	void initForALS();
	
	bool enable_proximity();
	bool enable_als();
	bool enable_gesture();
	bool set_power_on();
	
	uint8_t get_status();
	
	//Proximity Engine related functions
		//Setter functions
	bool set_pilt();
	bool set_piht();
	bool set_pers();
	bool set_pulseCount();
	bool set_ControlRegisterOne();
	bool set_ConfigRegisterTwo();
	bool set_proxOff_UR();
	bool set_proxOff_DL();
	
		//Getter functions
	uint8_t get_pilt();
	uint8_t get_piht();
	uint8_t get_pers();
	uint8_t get_pulseCount();
	uint8_t get_ControlRegisterOne();
	uint8_t get_ConfigRegisterTwo();
	uint8_t get_proxOff_UR();
	uint8_t get_proxOff_DL();
	uint8_t getProximity();
	
	
	//ALS Engine related functions
	
	//Gesture Engine related functions
	bool set_genth();
	bool set_gexth();
	bool set_gconf_one();
	bool set_gconf_two();
	bool set_goffset_u();
	bool set_goffset_d();
	bool set_goffset_l();
	bool set_goffset_r();
	bool set_gPulseCount();
	bool set_gconf_four();
	bool get_gstatus();
	
	
	//I2C Communication
	void i2c_avr_init();
	bool i2c_avr_write_byte_data(uint8_t reg, uint8_t command);
	uint8_t i2c_avr_read_byte_data(uint8_t reg);
	uint8_t i2c_avr_read_block_data(uint8_t reg, uint8_t length, uint8_t *data);
	void i2c_error();
	void i2c_stop(void);
	bool get_errorStatus();
		
	
private:
	//General Flags
	bool error_i2c;
	uint8_t enable;
	uint8_t status;
	
	uint8_t pers;
	uint8_t pulseCount;
	uint8_t controlOne;
	uint8_t configTwo;
	
	//Proximity related Flags/Variables
	uint8_t pilt;
	uint8_t piht;
	uint8_t ppers;
	uint8_t pplen;
	uint8_t ppulse;
	uint8_t ldrive;
	uint8_t pgain;
	uint8_t led_boost;
	uint8_t pint;
	uint8_t pvalid;
	uint8_t pData;
	uint8_t poffset_ur;
	uint8_t poffset_dl;
	
	//Gesture related Flags/Variables
	uint8_t gint;
	uint8_t fifoData[128];
	uint8_t gfifolvl;
	uint8_t gfifo_up[32];
	uint8_t gfifo_down[32];
	uint8_t gfifo_left[32];
	uint8_t gfifo_right[32];
	
	//ALS related Flags/Variables
	uint8_t aint;
	uint8_t apers;
	uint8_t avalid;
	
};





#endif /* APDS9960_AVR_H_ */
