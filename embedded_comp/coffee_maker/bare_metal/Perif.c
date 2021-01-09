#include "Perif.h"

uint8_t heaterStatus = 0;
uint8_t pumpStatus = 0;
uint8_t valveStatus = 0;
#define VALVE_VENT 0u
#define VALVE_OPEN 1u
#define VALVE_CLOSE 2u

uint32_t temperature = 0;
uint32_t presure = 0x100;

uint16_t lfsr = 0x1;

uint16_t randBit(){
	uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5));
	lfsr = (lfsr >> 1) | (bit << 15);
	return bit;
}

void update() {
	//Update pressure.
	if (valveStatus == VALVE_VENT){ //Value vent.
		presure = 0x100;
	} else {
		if(pumpStatus){
			presure += (randBit()) ? PUMP_POWER : 0u;
		}
		if(valveStatus == VALVE_OPEN){ //Valve open (otherwise, closed)
			presure -= (randBit()) ? BREW_FLOW : 0u;
		}
	}
	
	//Update temperature.
	if (heaterStatus){
		temperature += (randBit()) ? HEAT_POWER : 0u;
	} else if (temperature > TEMP_AMBIENT){
		temperature -= (randBit()) ? HEAT_LOSS : 0u;
	}
}

//The following functions control the valve. Names are self-explanitory.
void valveVent(){
	valveStatus = VALVE_VENT;
	update();
}
void valveOpen(){
	valveStatus = VALVE_OPEN;
	update();
}
void valveClose(){
	valveStatus = VALVE_CLOSE;
	update();
}

//The following functions control the bioler. Names are self-explanitory.
void boilerTurnOn(){
	heaterStatus = 1u;
	update();
}
void boilerTurnOff(){
	heaterStatus = 0u;
	update();
}

//The following functions control the bioler. Names are self-explanitory.
void pumpTurnOn(){
	pumpStatus = 1u;
	update();
}
void pumpTurnOff(){
	pumpStatus = 0u;
	update();
}

//The following functions return the current temperature and pressure.
uint32_t readTemperature(){
	update();
	return temperature;
}
uint32_t readPressure(){
	update();
	return presure;
}
