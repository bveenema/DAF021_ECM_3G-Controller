#include "motor.h"

void MOTOR_init()
{
    Serial.printlnf("Intializing Motor Controller");
    uint StartingRegister = Motor_EN_Reg;

    Wire.beginTransmission(4);
    Wire.write(StartingRegister);
    Wire.write(0b00000000); // Motor_EN_Reg - Disable all 3 motors
    Wire.endTransmission();
}


void MOTOR_update()
{
    // Tell the motor to move every 5 seconds
    static uint WaitToMove = 0;
    if(millis() - WaitToMove > 5000)
    {
        WaitToMove = millis();
    }

    // Check status every second
    static uint CheckStatus = 0;
    if(millis() - CheckStatus > 1000)
    {
        CheckStatus = millis();
    }

}

void MOTOR_SetAcceleration(const MotorName motor, const int acceleration)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motor == Blue)
		reg = Motor_1_Acceleration_1_Reg;
	else if(motor == Red)
		reg = Motor_2_Acceleration_1_Reg;
	else if(motor == CoBlend)
		reg = Motor_3_Acceleration_1_Reg;
	else
    {
        Serial.println("Bad Motor number");
        return;
    }

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

void MOTOR_SetSpeed(const MotorName motor, const int speed)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motor == Blue)
		reg = Motor_1_Speed_1_Reg;
	else if(motor == Red)
		reg = Motor_2_Speed_1_Reg;
	else if(motor == CoBlend)
		reg = Motor_3_Speed_1_Reg;
	else
    {
        Serial.println("Bad Motor number");
        return;
    }

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

void MOTOR_SetTarget(const MotorName motor, const int target)
{
    // Determine the starting register, motor must be 1,2 or 3, anything else is invalid
	uint reg = 0;
	if(motor == Blue)
		reg = Motor_1_Steps_1_Reg;
	else if(motor == Red)
		reg = Motor_2_Steps_1_Reg;
	else if(motor == CoBlend)
		reg = Motor_3_Steps_1_Reg;
	else
    {
        Serial.println("Bad Motor number");
        return;
    }

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

void MOTOR_SetDirection(const MotorName motor, const Direction dir)
{
    // Determine the bit to update register, motor must be 1,2 or 3, anything else is invalid
	uint bit = 0;
	if(motor == Blue)
		bit = 0;
	else if(motor == Red)
		bit = 1;
	else if(motor == CoBlend)
		bit = 2;
	else
    {
        Serial.println("Bad Motor number");
        return;
    }
		

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
    bitWrite(DirectionRegister, bit, dir);
    Serial.printlnf("New Direction Register: %d", DirectionRegister);

    // Update the Direction Register
    Wire.beginTransmission(MotorControllerAddress);
    Wire.write(Motor_DIR_Reg);
    Wire.write(DirectionRegister);
    Wire.endTransmission();
}

bool MOTOR_CheckMoveReg()
{
    // Set the Regsiter Pointer
    Wire.beginTransmission(MotorControllerAddress);
    Wire.write(Motor_MOVE_Reg);
    Wire.endTransmission();

    // Read the register back
    char MoveRegister = 0;
    Wire.requestFrom(4,1);
    while(Wire.available()) {
        MoveRegister = Wire.read();
        Serial.printlnf("Move Register: %d", MoveRegister);
    }

    return MoveRegister;
}

void MOTOR_StopAllMotors()
{
    // Set the Regsiter Pointer
    Wire.beginTransmission(MotorControllerAddress);
    Wire.write(Motor_MOVE_Reg);
    Wire.write(0);
    Wire.endTransmission();
}