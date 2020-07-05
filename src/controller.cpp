#include "controller.h"
#include "config.h"
#include "motor.h"
#include "LiquidSensor.h"
#include "rf_remote.h"

// Define State Machine Pointer
void (*do_controller)() = state_INIT;
void (*next_state)() = state_IDLE; // Some functions use a "next state" pointer so they may be re-used. The function that calls a function that uses next_state must set the next_state pointer (ex. state_PAUSE)

// LastRunTime is used to determine when a keep open should run and is updated every time a keep open, mix or flush is finished
unsigned LastRunTime = 0;

void state_INIT()
{
    // Configure Motors
    MOTOR_SetAcceleration(Blue, CONFIG_BlueMotorAcceleration);
    MOTOR_SetAcceleration(Red, CONFIG_RedMotorAcceleration);

	do_controller = state_IDLE;
}

void state_IDLE()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
		Serial.println("IDLE");
		START_STATE = false;
	}

    // If settings have not been received, do nothing
    if(Settings.valid == false) return;

	// Handle ChangeState - state becomes charge
    PressType input = Remote.getStatus();
	if(input == ShortPress)
	{
		// Liquid Sensor detection is disabled until the first button press
		LiquidSensor_Blue.enableDetection();
    	LiquidSensor_Red.enableDetection();

		// Run a Short Shot if no liquid is present for a mix
		if(!LiquidSensor_Blue.hasLiquid() || !LiquidSensor_Red.hasLiquid()) // Check for Liquid presence if MIX mode, ignore for flush
			do_controller = state_SHORT_SHOT;
		else
		{
			do_controller = state_MIX;
			LastRunTime = millis();
		}
	}
    else if(input == LongPress)
    {
        do_controller = state_FLUSH_PURGE;
        LastRunTime = millis();
    }

	// Exit State Clean-up
	if(do_controller != state_IDLE)
		START_STATE = true;
}

void state_MIX()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
        // Signal Start of Cycle
	    CHIME_StartStop.setStatus(Active);
		Serial.println("MIX");

        // Enable Motors
        Wire.beginTransmission(4);
        Wire.write(Motor_EN_Reg);
        Wire.write(0b00000111); // Motor_EN_Reg - Disable all 3 motors
        Wire.endTransmission();

        // Set Directions
        MOTOR_SetDirection(Blue, FORWARD);
        MOTOR_SetDirection(Red, FORWARD);

        // Set Steps
        /// Steps = mGal * Cu-in/gal * Steps/Rev / mCu-in/Rev
        uint TotalSteps = Settings.Volume * CubicInchesPerGallon * CONFIG_StepsPerRev / CONFIG_MilliCubicInchesPerRevolution;
        uint BlueSteps = TotalSteps * Settings.Ratio / (Settings.Ratio + 100);
        uint RedSteps = TotalSteps * 100 / (Settings.Ratio + 100);

        Serial.printlnf("Total Steps: %d, Blue Steps: %d, Red Steps: %d", TotalSteps, BlueSteps, RedSteps);

        MOTOR_SetTarget(Blue, BlueSteps);
        MOTOR_SetTarget(Red, RedSteps);

        // Set Speeds
        /// Steps/Sec = mGPM * Cu-in/gal * Steps/Rev / mCu-in/Rev / 60
        uint TotalStepsPerSecond = CONFIG_MilliGallonsPerMintue * CubicInchesPerGallon * CONFIG_StepsPerRev / CONFIG_MilliCubicInchesPerRevolution / 60;
        uint BlueStepsPerSecond = TotalStepsPerSecond * Settings.Ratio / (Settings.Ratio + 100);
        uint RedStepsPerSecond = TotalStepsPerSecond * 100 / (Settings.Ratio + 100);

        Serial.printlnf("Total Steps/Sec: %d, Blue Steps/Sec: %d, Red Steps/Sec: %d", TotalStepsPerSecond, BlueStepsPerSecond, RedStepsPerSecond);

        MOTOR_SetSpeed(Blue, BlueStepsPerSecond);
        MOTOR_SetSpeed(Red, RedStepsPerSecond);

        // Move Motors
        Wire.beginTransmission(4);
        Wire.write(Motor_MOVE_Reg);
        Wire.write(0b00000011);
        Wire.endTransmission();

		START_STATE = false;
	}

    // Monitor MOVE Register for move completion
    static uint LastMoveRegCheck = 0;
    if(millis() - LastMoveRegCheck > 1000)
    {
        if(MOTOR_CheckMoveReg() == 0)
        {
            // do_controller = state_SUCK_BACK;
            do_controller = state_IDLE;
        }
        LastMoveRegCheck = millis();
    }

    // Exit State Clean-up
	if(do_controller != state_MIX)
		START_STATE = true;
}

void state_SHORT_SHOT()
{
    do_controller = state_IDLE;
}

void state_FLUSH_PURGE()
{
    do_controller = state_IDLE;
}