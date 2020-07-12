#include "globals.h"

// IO Expanders
VDW_MCP23017 IOEXP1(0x20, IO_INT_PIN);
VDW_MCP23017 IOEXP2(0x24, IO_2_INT_PIN);

// functions for read and writing IO expanders that can be passed to other functions
bool readIO1(uint8_t pin){ return IOEXP1.readPin(pin); }
bool readIO2(uint8_t pin){ return IOEXP2.readPin(pin); }
void writeIO1(uint8_t pin, bool dir){ IOEXP1.writePin(pin, dir); }
void writeIO2(uint8_t pin, bool dir){ IOEXP2.writePin(pin, dir); }

// Chimes
VDW_StatusChimeTarget Chime(HORN_EN_PIN);
VDW_ChimeStatus CHIME_StartStop(&Chime, StatusChime_Priority_Med, StatusChime_Pattern_Pulse, StatusChime_Speed_Slow, 1);
VDW_ChimeStatus CHIME_RevLimit(&Chime, StatusChime_Priority_High, StatusChime_Pattern_Pulse, StatusChime_Speed_Fast);
VDW_ChimeStatus CHIME_LiquidError(&Chime, StatusChime_Priority_High, StatusChime_Pattern_Pulse, StatusChime_Speed_Medium, 3);
VDW_ChimeStatus CHIME_StartFlush(&Chime, StatusChime_Priority_Med, StatusChime_Pattern_Pulse, 4000, 2000, 1);
VDW_ChimeStatus CHIME_NoBucket(&Chime, StatusChime_Priority_Low, StatusChime_Pattern_Pulse, 3000, 200);
VDW_ChimeStatus CHIME_BLE_Confirm(&Chime, StatusChime_Priority_High, StatusChime_Pattern_Pulse, 1000, 500, 2);
VDW_ChimeStatus CHIME_ShortShotSuccess(&Chime, StatusChime_Priority_High, StatusChime_Pattern_Pulse, 1, 2000, 1);
VDW_ChimeStatus CHIME_SettingsInvalid(&Chime, StatusChime_Priority_Low, StatusChime_Pattern_Pulse, 10000, 350);

// Pressure Sensors
VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor_Blue;
VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor_Red;

// Settings
settings Settings;

// Flags
bool FLAG_SettingsUpdated = false;