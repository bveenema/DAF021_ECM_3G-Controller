#ifndef CONFIG_H
#define CONFIG_H

// Cycle Settings
#define CONFIG_ShortShotEnable 0 // 0: Disable Short Shot, 1: Enable Short Shot

// Pail Sensor Settings
#define CONFIG_PailDetectionThreshold 1200 // ADC Value (out of 4085)

// Pressure Settings
#define CONFIG_SkipPressureTest 0 // 0: Use reservoir sizing and leak testing 1: Skip the reservoir sizing and leak testing (uses Small Reservoir Settings)

#define CONFIG_MinValidPressure 500 // mPSI (500)
#define CONFIG_MaxChargePressure 4000 // mPSI (4000)
#define CONFIG_PumpOnPressure_Small 1400 // mPSI (1400)
#define CONFIG_PumpOffPressure_Small 1800 // mPSI (1800)
#define CONFIG_PumpOnPressure_Large 2500 // mPSI (2500)
#define CONFIG_PumpOffPressure_Large 3000 // mPSI (3000)
#define CONFIG_EmptyPressure 100 // mPSI - the pressure of an empty reservoir
#define CONFIG_TestPressure 1500 // mPSI - the pressure to which the reservoir is charged for determining size and leak test
#define CONFIG_LeakPercentage 50 // % - the % amount the pressure can decrease after settling during leak testing
#define CONFIG_MaxChargeTime 60000 // mSec - the max amount of time for the reservoir to reach Test Pressure
#define CONFIG_SmallReservoirTime 5000 // mSec - the threshold time for a small reservoir (< SmallReservoirTime = Small Reservoir, > SmallReservoirTime = Large Reservoir)
#define CONFIG_SettleTime 2000 // mSec - the amount of time after the reservoir reaches the test pressure to settle
#define CONFIG_LeakTestTime 5000 // mSec - the total Leak Test time (pressure for leaks after Settle Time)

// Volume Settings
#define CONFIG_MilliCubicInchesPerRevolution 1000 // mCu-in/Rev
#define CONFIG_SuckBackVolume 50 // mGallons
#define CONFIG_ShortShotVolume 50 // mGallons
#define CONFIG_FlushFirstBolusVolume 1200 // mGallons
#define CONFIG_FlushFinalBolusVolume 600 // mGallons
#define CONFIG_FlushForwardVolume 28 // mGallons
#define CONFIG_FlushBackwardVolume 14 // mGallons

// Rate Settings
#define CONFIG_MixRate 2000 //mGallons/min
#define CONFIG_SuckBackRate 1000 // mGallons/min
#define CONFIG_ShortShotRate 2500 // mGallons/min
#define CONFIG_FlushRate 2500 // mGallons/min

// Flush Settings
#define CONFIG_FlushBackAndForthCycles 300 // number of complete cyles
#define CONFIG_FlushPauseTime 0 // ms - Pause between back and forth

// Keep Open Settings
#define CONFIG_KeepOpenVolume 120 // mGallons
#define CONFIG_KeepOpenRate 2500 // mGallons/min
#define CONFIG_KeepOpenInterval 300000 // ms (300000 = 5 minutes)

// Motor Settings
#define CONFIG_StepsPerRev 200 // steps/rev
#define CONFIG_BlueMotorAcceleration 10000 // steps/sec^2
#define CONFIG_RedMotorAcceleration 10000 // steps/sec^2

// Conversions
#define CubicInchesPerGallon 231 // Cubic Inches to Gallons

#endif