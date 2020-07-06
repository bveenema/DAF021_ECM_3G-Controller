#ifndef CONFIG_H
#define CONFIG_H

// Pail Sensor Settings
#define CONFIG_PailDetectionThreshold 600 // ADC Value (out of 4085)

// Pressure Settings
#define CONFIG_MaxChargePressure 4000 // mPSI
#define CONFIG_VentOpenPressure 1500 // mPSI (2700)
#define CONFIG_VentClosePressure 500 // mPSI (1800)
#define CONFIG_PumpOnPressure_18 1400 // mPSI
#define CONFIG_PumpOffPressure_18 1800 // mPSI
#define CONFIG_PumpOnPressure_30 2500 // mPSI
#define CONFIG_PumpOffPressure_30 3000 // mPSI

// Volume Settings
#define CONFIG_MilliCubicInchesPerRevolution 1100 // mCu-in/Rev
#define CONFIG_SuckBackVolume 50 // mGallons
#define CONFIG_ShortShotVolume 100 // mGallons
#define CONFIG_FlushFirstBolusVolume 100 // mGallons
#define CONFIG_FlushFinalBolusVolume 90 // mGallons
#define CONFIG_FlushForwardVolume 10 // mGallons
#define CONFIG_FlushBackwardVolume 7 // mGallons

// Rate Settings
#define CONFIG_MixRate 1000 // mGallons/min
#define CONFIG_SuckBackRate 1000 // mGallons/min
#define CONFIG_ShortShotRate 500 // mGallons/min
#define CONFIG_FlushRate 500 // mGallons/min

// Flush Settings
#define CONFIG_FlushBackAndForthCycles 10 // number of complete cyles
#define CONFIG_FlushPauseTime 0 // ms - Pause between back and forth

// Keep Open Settings
#define CONFIG_KeepOpenVolume 20 // mGallons
#define CONFIG_KeepOpenRate 1000 // mGallons/min
#define CONFIG_KeepOpenInterval 60000 // ms (60000 = 1 minute)

// Motor Settings
#define CONFIG_StepsPerRev 400 // steps/rev
#define CONFIG_BlueMotorAcceleration 10000 // steps/sec^2
#define CONFIG_RedMotorAcceleration 10000 // steps/sec^2

// Conversions
#define CubicInchesPerGallon 231 // Cubic Inches to Gallons

#endif