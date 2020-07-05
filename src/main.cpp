/*
 * Project DAF021_ECM_3G-Boron
 * Description: Primary controller for DAF021_ECM_3G
 * Author: Veenema Design Works
 */

#include "Particle.h"
#include "globals.h"
#include "PailSensor.h"
#include "LiquidSensor.h"
#include "PressureManager.h"
#include "rf_remote.h"
#include "ble-uart.h"
#include "motor.h"

void setup();
void loop();

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void setup()
{
    Serial.begin(57000);
    Serial.println("Begin Program");

    // Initialize Pins
    pinMode(I2C_RESET_PIN, OUTPUT);
    pinMode(IO_2_INT_PIN, INPUT);
    digitalWrite(I2C_RESET_PIN, HIGH);

    // Reset I2C
    digitalWrite(I2C_RESET_PIN, LOW);
    delay(10);
    digitalWrite(I2C_RESET_PIN, HIGH);
    delay(750); // Allow time for I2C devices to initialize

    // Initialize I2C
    Wire.begin();

    // Initialize BLE
    BLE_UART_init();

    // Initialize I/O Expanders
    IOEXP1.init();
    IOEXP1.configureInterrupts(1, 0, 1); // Mirror Register A/B, Active Driver Output, Active High
    IOEXP1.enableInterruptMode();
    IOEXP1.update();

    IOEXP2.init();
    IOEXP2.configureInterrupts(1, 0, 1); // Mirror Register A/B, Active Driver Output, Active High
    IOEXP2.enableInterruptMode();
    IOEXP2.update();

    // Initialize Sensors
    PailSensor.init();
    LiquidSensor_Blue.init(&IOEXP2, IO2_LIQUID_SENSOR_BLUE);
    LiquidSensor_Red.init(&IOEXP2, IO2_LIQUID_SENSOR_RED);
    LiquidSensor_Blue.enableDetection();
    LiquidSensor_Red.enableDetection();
    PressureSensor_Blue.init(PRESSURE_BLUE_PIN, 5000, 0, 3300, 10);
    PressureSensor_Red.init(PRESSURE_RED_PIN, 5000, 0, 3300, 10);

    // Initialize Modules
    PressureManager.init(&IOEXP2, IO2_AIR_BLUE_EN, IO2_VALVE_1_EN, &PressureSensor_Blue, &PressureSensor_Red);
    PressureManager.setOffPressure(0);
    PressureManager.setOnPressure(0);
    Remote.init(&IOEXP1, IO1_USER_REMOTE);
    MOTOR_init();
    Chime.init();

    // Wait for Pressure Sensor to stabalize, then zero
    uint32_t InitialTime = millis();
    while(millis() - InitialTime < 500)
    {
        PressureSensor_Blue.update();
        PressureSensor_Red.update();
    }
    PressureSensor_Blue.zero();
    PressureSensor_Red.zero();
}

void loop()
{
    // Update I/O Expanders
    IOEXP1.update();
    IOEXP2.update();

    // Update BLE
    BLE_UART_update();

    // Update Sensors
    PailSensor.update();
    LiquidSensor_Blue.update();
    LiquidSensor_Red.update();
    PressureSensor_Blue.update();
    PressureSensor_Red.update();

    // Update Modules
    PressureManager.update();
    Remote.update();
    Chime.update();
    MOTOR_update();

    // Handle Settings Updated
    if(FLAG_SettingsUpdated)
    {
        if(Settings.Pressure == 18)
        {
            PressureManager.setOffPressure(CONFIG_PumpOffPressure_18);
            PressureManager.setOnPressure(CONFIG_PumpOnPressure_18);
        }
        else if(Settings.Pressure == 30)
        {
            PressureManager.setOffPressure(CONFIG_PumpOffPressure_30);
            PressureManager.setOnPressure(CONFIG_PumpOnPressure_30);
        }

        Settings.valid = true;
        FLAG_SettingsUpdated = false;
    }
    
    static uint LastPrintTime = 0;
    if(millis() - LastPrintTime > 1000)
    {
        // Test Pail Sensor
        // Serial.printlnf("%s - Value: %d", (PailSensor.getState()) ? "Pail Present" : "No Pail", PailSensor.getCurrentReading());

        // Test Liquid Sensor
        // Serial.printlnf("Liquid Sensor:  Blue - %d, Red - %d", LiquidSensor_Blue.hasLiquid(), LiquidSensor_Red.hasLiquid());

        // Test Pressure Sensor
        // Serial.printlnf("Pressure Sensor: Blue - %i, Red - %i", PressureSensor_Blue.getPressureAverage(), PressureSensor_Red.getPressureAverage());

        // Test Remote
        // Serial.printlnf("Remote Pressed: %d", Remote.getStatus());

        LastPrintTime = millis();
    }
}