#ifndef CONFIG_H
#define CONFIG_H

// Pail Sensor Settings
#define CONFIG_PailDetectionThreshold 1200 // ADC Value (out of 4085)

// Pressure Settings
#define CONFIG_MinValidPressure 500 // mPSI (500)
#define CONFIG_MaxChargePressure 4000 // mPSI (4000)
#define CONFIG_PumpOnPressure_18 1400 // mPSI (1400)
#define CONFIG_PumpOffPressure_18 1800 // mPSI (1800)
#define CONFIG_PumpOnPressure_30 2500 // mPSI (2500)
#define CONFIG_PumpOffPressure_30 3000 // mPSI (3000)

// Volume Settings
#define CONFIG_MilliCubicInchesPerRevolution 1100 // mCu-in/Rev
#define CONFIG_SuckBackVolume 50 // mGallons
#define CONFIG_ShortShotVolume 50 // mGallons
#define CONFIG_FlushFirstBolusVolume 100 // mGallons
#define CONFIG_FlushFinalBolusVolume 100 // mGallons
#define CONFIG_FlushForwardVolume 14 // mGallons
#define CONFIG_FlushBackwardVolume 10 // mGallons

// Rate Settings
#define CONFIG_MixRate 2500 // mGallons/min
#define CONFIG_SuckBackRate 1000 // mGallons/min
#define CONFIG_ShortShotRate 1500 // mGallons/min
#define CONFIG_FlushRate 820 // mGallons/min

// Flush Settings
#define CONFIG_FlushBackAndForthCycles 30 // number of complete cyles
#define CONFIG_FlushPauseTime 0 // ms - Pause between back and forth

// Keep Open Settings
#define CONFIG_KeepOpenVolume 50 // mGallons
#define CONFIG_KeepOpenRate 1000 // mGallons/min
#define CONFIG_KeepOpenInterval 600000 // ms (600000 = 10 minutes)

// Motor Settings
#define CONFIG_StepsPerRev 200 // steps/rev
#define CONFIG_BlueMotorAcceleration 10000 // steps/sec^2
#define CONFIG_RedMotorAcceleration 10000 // steps/sec^2

// Conversions
#define CubicInchesPerGallon 231 // Cubic Inches to Gallons

#endif