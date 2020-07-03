#ifndef CONFIG_H
#define CONFIG_H

// Pail Sensor Settings
#define CONFIG_PailDetectionThreshold 600 // ADC Value (out of 4085)

// Pressure Settings
#define CONFIG_MaxChargePressure 4000 // mPSI
#define CONFIG_VentOpenPressure 1500 // mPSI (2700)
#define CONFIG_VentClosePressure 500 // mPSI (1800)
#define CONFIG_PumpOnPressure 500 // mPSI (1800)
#define CONFIG_PumpOffPressure 1000 // mPSI (2250)

#endif