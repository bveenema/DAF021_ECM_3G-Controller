#ifndef GLOBALS_H
#define GLOBALS_H

#include "Particle.h"
#include "pins.h"
#include "config.h"
#include "VDW_MCP23017.h"
#include "VDW_StatusChime.h"
#include "VDW_PressureSensor_HoneyWell_HSC_SSC.h"

// IO Expanders
extern VDW_MCP23017 IOEXP1;
extern VDW_MCP23017 IOEXP2;

// functions for read and writing IO expanders that can be passed to other functions
bool readIO1(uint8_t pin);
bool readIO2(uint8_t pin);
bool readIO3(uint8_t pin);
void writeIO1(uint8_t pin, bool dir);
void writeIO2(uint8_t pin, bool dir);
void writeIO3(uint8_t pin, bool dir);

// Chimes
extern VDW_StatusChimeTarget Chime;
extern VDW_ChimeStatus CHIME_StartStop;
extern VDW_ChimeStatus CHIME_RevLimit;
extern VDW_ChimeStatus CHIME_LiquidError;
extern VDW_ChimeStatus CHIME_ChangeMode;
extern VDW_ChimeStatus CHIME_NoBucket;
extern VDW_ChimeStatus CHIME_BLE_Confirm;

// Pressure Sensors
extern VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor_Blue;
extern VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor_Red;

// Settings
struct settings
{
    uint Ratio; // XXX:100 Blue:Red
    uint Pressure; //mPSI
    uint Volume; //mGal
    bool valid; // true when settings have been set
};

extern settings Settings;

// Flags
extern bool FLAG_SettingsUpdated;

#endif