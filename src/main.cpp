/*
 * Project DAF021_ECM_3G-Boron
 * Description: Primary controller for DAF021_ECM_3G
 * Author: Veenema Design Works
 */

#include "Particle.h"
#include "motor.h"
#include "pins.h"

void setup();
void loop();

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void setup()
{
    Serial.begin(57000);
    delay(1000);
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
    MOTOR_init();    
}

void loop()
{
    MOTOR_update();
    delay(5000);
}