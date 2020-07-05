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
#define CONFIG_CubicInchesPerRevolution 1100 // mCu-in/Rev

// Conversions
#define CubicInchesPerGallon 231 // Cubic Inches to Gallons
#define MilliGallonsPerCubicInch 4 // MilliGallons to Cubic Inches

#endif