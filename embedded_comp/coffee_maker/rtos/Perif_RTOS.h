/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PERIF_H
#define __PERIF_H

#include <stdint.h>

//You can adjust the following constants to make the temperature/pressure change faster.
#define BREW_FLOW 1u //MUST BE LESS THAN PUMP_POWER
#define PUMP_POWER 2u
#define HEAT_LOSS 1u //MUST BE LESS THAN HEAT_POWER
#define HEAT_POWER 2u

//You can adjust the following, but there's little reason to do so.
#define TEMP_AMBIENT 20u //MUST BE GREATER THAN HEAT_LOSS

//************IMPORTANT***************
//The following function must be made into a thread at the beginning of your
//program. Only a single copy of this function must be made, but once it's
//made, no interaction is required. No arguments are needed.
void hardwareSimulationThreadFunction(void * arg);

//The following functions control the valve. Names are self-explanitory.
void valveVent(void);
void valveOpen(void);
void valveClose(void);

//The following functions control the bioler. Names are self-explanitory.
void boilerTurnOn(void);
void boilerTurnOff(void);

//The following functions control the bioler. Names are self-explanitory.
void pumpTurnOn(void);
void pumpTurnOff(void);

//The following functions return the current temperature and pressure.
uint32_t readTemperature(void);
uint32_t readPressure(void);

#endif /* __PERIF_H */

