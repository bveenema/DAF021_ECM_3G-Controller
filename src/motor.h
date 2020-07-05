#ifndef MOTOR_H
#define MOTOR_H

// Motor.h is set of functions for interacting with motor driver peripheral (Argon)
#include "Particle.h"

const uint8_t NumRegisters = 40;
const uint8_t MotorControllerAddress = 0x04;
enum MotorDriverRegister
{
    Motor_EN_Reg,
    Motor_DIR_Reg,
    Motor_MOVE_Reg,
    Motor_ERROR_Reg,
    Motor_1_Speed_1_Reg,
    Motor_1_Speed_2_Reg,
    Motor_1_Speed_3_Reg,
    Motor_1_Speed_4_Reg,
    Motor_1_Acceleration_1_Reg,
    Motor_1_Acceleration_2_Reg,
    Motor_1_Acceleration_3_Reg,
    Motor_1_Acceleration_4_Reg,
    Motor_1_Steps_1_Reg,
    Motor_1_Steps_2_Reg,
    Motor_1_Steps_3_Reg,
    Motor_1_Steps_4_Reg,
    Motor_2_Speed_1_Reg,
    Motor_2_Speed_2_Reg,
    Motor_2_Speed_3_Reg,
    Motor_2_Speed_4_Reg,
    Motor_2_Acceleration_1_Reg,
    Motor_2_Acceleration_2_Reg,
    Motor_2_Acceleration_3_Reg,
    Motor_2_Acceleration_4_Reg,
    Motor_2_Steps_1_Reg,
    Motor_2_Steps_2_Reg,
    Motor_2_Steps_3_Reg,
    Motor_2_Steps_4_Reg,
    Motor_3_Speed_1_Reg,
    Motor_3_Speed_2_Reg,
    Motor_3_Speed_3_Reg,
    Motor_3_Speed_4_Reg,
    Motor_3_Acceleration_1_Reg,
    Motor_3_Acceleration_2_Reg,
    Motor_3_Acceleration_3_Reg,
    Motor_3_Acceleration_4_Reg,
    Motor_3_Steps_1_Reg,
    Motor_3_Steps_2_Reg,
    Motor_3_Steps_3_Reg,
    Motor_3_Steps_4_Reg,
};

enum Direction
{
    BACKWARD,
    FORWARD
};

/// init
// Intialize Motor Settings
void MOTOR_init();

/// update
// Check on the motor. Call every loop
void MOTOR_update();

// Set Acceleration
/// Updates the acceleration for the given motor
/// \param[const uint] motorNum - The motor number to set the acceleration
/// \param[const int] acceleration - the new acceleration value (steps/sec^2)
void MOTOR_SetAcceleration(const uint motorNum, const int acceleration);

// Set Speed
/// Update the speed for the given motor
/// \param[const uint] motorNum - The motor number to set the acceleration
/// \param[const int] speed - the new speed value (steps/sec)
void MOTOR_SetSpeed(const uint motorNum, const int speed);

// Set Target
/// Update the target for the given motor
/// \param[const uint] motorNum - The motor number to set the acceleration
/// \param[const int] target - the new target value (steps)
void MOTOR_SetTarget(const uint motorNum, const int target);

// Set Direction
/// Update the direction for the given motor
/// \param[const uint] motorNum - The motor number to set the acceleration
/// \param[const Direction] dir - the new direction (BACKWARD/FORWARD)
void MOTOR_SetDirection(const uint motorNum, const Direction dir);

#endif