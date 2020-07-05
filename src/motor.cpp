#include "motor.h"

void MOTOR_init()
{
    Serial.printlnf("Intializing Motor Controller");
    uint StartingRegister = Motor_EN_Reg;
    Wire.beginTransmission(4);
    Wire.write(StartingRegister);
    Wire.write(0b00000111); // Motor_EN_Reg - Enable all 3 motors
    Wire.write(0b00000111); // Motor_DIR_Reg - Set motor directions
    Wire.write(0b00000000); // Motor_MOVE_Reg - Leave unset
    // Set as 3850 for speed
    Wire.write(0x00); // Motor_1_Speed_1_Reg - High Byte
    Wire.write(0x00); // Motor_1_Speed_2_Reg - 2nd Highest Byte
    Wire.write(0x0F); // Motor_1_Speed_3_Reg - 2nd Lowest Byte
    Wire.write(0x0A); // Motor_1_Speed_4_Reg - Low Byte
    // Set as 10000 for acceleration
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x27);
    Wire.write(0x10);
    // Set as 8000 for steps
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x1F);
    Wire.write(0x40);

    Wire.endTransmission();
}


void MOTOR_update()
{
    // Tell the motor to move every 5 seconds
    static uint WaitToMove = 0;
    if(millis() - WaitToMove > 5000)
    {
        Serial.printlnf("Moving Motor");
        // Enable MOVE Register
        Wire.beginTransmission(4);
        Wire.write(Motor_MOVE_Reg);
        Wire.write(0b00000001);
        Wire.endTransmission();

        WaitToMove = millis();
    }

    // Check status every second
    static uint CheckStatus = 0;
    if(millis() - CheckStatus > 1000)
    {
        // Set the Regsiter Pointer
        Wire.beginTransmission(MotorControllerAddress);
        Wire.write(Motor_MOVE_Reg);
        Wire.endTransmission();

        // Read the register back
        int NumBytesReceived =  Wire.requestFrom(4,1);
        while(Wire.available()) { // loop through all but the last
            uint8_t c = Wire.read();       // receive byte as a character
            Serial.println(c);            // print the character
        }
    }

}