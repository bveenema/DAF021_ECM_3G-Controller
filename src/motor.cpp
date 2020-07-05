#include "motor.h"

void MOTOR_init()
{
    Serial.printlnf("Intializing Motor Controller");
    uint StartingRegister = Motor_EN_Reg;

    Wire.beginTransmission(4);
    Wire.write(StartingRegister);
    Wire.write(0b00000111); // Motor_EN_Reg - Enable all 3 motors
    Wire.write(0b00000110); // Motor_DIR_Reg - Set motor directions
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
        int target = 16000;
        int speed = 1500;
        int acceleration = 7000;
        bool direction = 1;
        Serial.printlnf("Moving Motor 1 - Direction: %d Target: %d, Speed: %d, Acceleration: %d", direction, target, speed, acceleration);
        
        MOTOR_SetDirection(1, BACKWARD);
        MOTOR_SetTarget(1, target);
        MOTOR_SetSpeed(1, speed);
        MOTOR_SetAcceleration(1, acceleration);

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
        Wire.requestFrom(4,1);
        while(Wire.available()) { // loop through all but the last
            uint8_t c = Wire.read();       // receive byte as a character
            Serial.println(c);            // print the character
        }

        CheckStatus = millis();
    }

}

void MOTOR_SetAcceleration(const uint motorNum, const int acceleration)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motorNum == 1)
		reg = Motor_1_Acceleration_1_Reg;
	else if(motorNum == 2)
		reg = Motor_2_Acceleration_1_Reg;
	else if(motorNum == 3)
		reg = Motor_3_Acceleration_1_Reg;
	else
		return;

    // Seperate the acceleration into bytes
    char byte1 = (acceleration >> 24) & 255;
    char byte2 = (acceleration >> 16) & 255;
    char byte3 = (acceleration >> 8) & 255;
    char byte4 = acceleration & 255;

    // Send the bytes
    Wire.beginTransmission(4);
    Wire.write(reg);
    Wire.write(byte1); // High Byte
    Wire.write(byte2);
    Wire.write(byte3);
    Wire.write(byte4); // Low Byte
    Wire.endTransmission();
}

void MOTOR_SetSpeed(const uint motorNum, const int speed)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motorNum == 1)
		reg = Motor_1_Speed_1_Reg;
	else if(motorNum == 2)
		reg = Motor_2_Speed_1_Reg;
	else if(motorNum == 3)
		reg = Motor_3_Speed_1_Reg;
	else
		return;

    // Seperate the speed into bytes
    char byte1 = (speed >> 24) & 255;
    char byte2 = (speed >> 16) & 255;
    char byte3 = (speed >> 8) & 255;
    char byte4 = speed & 255;

    // Send the bytes
    Wire.beginTransmission(4);
    Wire.write(reg);
    Wire.write(byte1); // High Byte
    Wire.write(byte2);
    Wire.write(byte3);
    Wire.write(byte4); // Low Byte
    Wire.endTransmission();
}

void MOTOR_SetTarget(const uint motorNum, const int target)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motorNum == 1)
		reg = Motor_1_Steps_1_Reg;
	else if(motorNum == 2)
		reg = Motor_2_Steps_1_Reg;
	else if(motorNum == 3)
		reg = Motor_3_Steps_1_Reg;
	else
		return;

    // Seperate the target into bytes
    char byte1 = (target >> 24) & 255;
    char byte2 = (target >> 16) & 255;
    char byte3 = (target >> 8) & 255;
    char byte4 = target & 255;

    // Send the bytes
    Wire.beginTransmission(4);
    Wire.write(reg);
    Wire.write(byte1); // High Byte
    Wire.write(byte2);
    Wire.write(byte3);
    Wire.write(byte4); // Low Byte
    Wire.endTransmission();
}

void MOTOR_SetDirection(const uint motorNum, const Direction dir)
{
    // Determine the bit to update register, motor must be 1,2 or 3, anything else is invalid
	uint bit = 0;
	if(motorNum == 1)
		bit = 0;
	else if(motorNum == 2)
		bit = 1;
	else if(motorNum == 3)
		bit = 2;
	else
		return;

    // Get the current direction register
    /// Set the Regsiter Pointer
    Wire.beginTransmission(MotorControllerAddress);
    Wire.write(Motor_DIR_Reg);
    Wire.endTransmission();

    /// Read the register back
    Wire.requestFrom(4,1);
    char DirectionRegister = 0;
    while(Wire.available()) {
        DirectionRegister = Wire.read();       // receive byte as a character
    }
    Serial.printlnf("Current Direction Register: %d", DirectionRegister);
    DirectionRegister = DirectionRegister | (dir << bit);
    Serial.printlnf("New Direction Register: %d", DirectionRegister);

    // Update the Direction Register
    Wire.beginTransmission(MotorControllerAddress);
    Wire.write(Motor_DIR_Reg);
    Wire.write(DirectionRegister);
    Wire.endTransmission();
}