/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PERIF_H
#define __PERIF_H

#include <stdint.h>

//You can adjust the following constants to make the temperature/pressure change faster.
#define BREW_FLOW 1u //MUST BE LESS THAN PUMP_POWER
#define PUMP_POWER 1u
#define HEAT_LOSS 1u //MUST BE LESS THAN HEAT_POWER
#define HEAT_POWER 1u

//You can adjust the following, but there's little reason to do so.
#define TEMP_AMBIENT 20u //MUST BE GREATER THAN HEAT_LOSS
   

#endif /* __PERIF_H */

//The following functions control the valve. Names are self-explanitory.
void valveVent();
void valveOpen();
void valveClose();

//The following functions control the bioler. Names are self-explanitory.
void boilerTurnOn();
void boilerTurnOff();

//The following functions control the bioler. Names are self-explanitory.
void pumpTurnOn();
void pumpTurnOff();

//The following functions return the current temperature and pressure.
uint32_t readTemperature();
uint32_t readPressure();