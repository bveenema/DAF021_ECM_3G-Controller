#include "controller.h"
#include "config.h"
#include "motor.h"
#include "LiquidSensor.h"
#include "rf_remote.h"
#include "PailSensor.h"
#include "PressureManager.h"
#include "globals.h"

// Define State Machine Pointer
void (*do_controller)() = state_INIT;
void (*next_state)() = state_IDLE; // Some functions use a "next state" pointer so they may be re-used. The function that calls a function that uses next_state must set the next_state pointer (ex. state_PAUSE)

// LastRunTime is used to determine when a keep open should run and is updated every time a keep open, mix or flush is finished
uint LastRunTime = 0;
// FirstMix is set to true after the first Mix cycle is initiated and allows keep open when set to true
bool FirstMix = false;

// Flush purge is used for initial and final bolus. FirstFlush tracks which purge should be used.
bool FirstFlush = true;

// Utility Functions
void SetupMotors(const Direction Direction, const uint Volume, const uint Rate, const uint Ratio); // Calls below setup motors, used for non-mix
void SetupMotors(const Direction Direction, const uint RevsBlue, const uint RevsRed, const uint Rate,  const uint Ratio); // runs motors for mix
bool EarlyCancel(bool CheckLiquid);

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
		Serial.println("\nIDLE\n");
		START_STATE = false;
	}

    // Always read remote so button presses aren't stored before settings are valid;
    PressType input = Remote.getStatus();

    // If settings have not been received, do nothing
    if(Settings.valid == false) return;

    // Check if Keep Open should run
    if(FirstMix && millis() - LastRunTime > CONFIG_KeepOpenInterval)
        do_controller = state_KEEP_OPEN;

	// Handle ChangeState - state becomes charge
	if(input == ShortPress)
	{
        Serial.println("ShortPress");
        // Run a Short Shot if no liquid is present for a mix
		if(!LiquidSensor_Blue.hasLiquid() || !LiquidSensor_Red.hasLiquid()) // Check for Liquid presence if MIX mode, ignore for flush
        {
            Serial.println("NO LIQUID");
            if(CONFIG_ShortShotEnable == 1) // Check if Short Shot is enabled
            {
                do_controller = state_SHORT_SHOT;
            }
        }
		else
		{
			do_controller = state_MIX;
			LastRunTime = millis();
		}
	}
    else if(input == LongPress)
    {
        // CHIME_StartFlush.setStatus(Active);
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
		Serial.println("\nMIX\n");

        SetupMotors(FORWARD, Settings.RevolutionsBlue, Settings.RevolutionsRed, CONFIG_MixRate, Settings.Ratio);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

        // Set FirstMix so Keep Open can be run
        FirstMix = true;

		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        do_controller = state_END_CYCLE;
    }
    else if(InterruptReason == PositionError)
    {
        Serial.println("MIX RATIO ERROR");

        MOTOR_StopAllMotors();

        // Disable Motors
        Wire.beginTransmission(4);
        Wire.write(Motor_EN_Reg);
        Wire.write(0b00000000); // Motor_EN_Reg - Disable all 3 motors
        Wire.endTransmission();

        do_controller = state_END_CYCLE;
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(true))
    {
        MOTOR_StopAllMotors(); 
        do_controller = state_END_CYCLE;
    }  

    // Exit State Clean-up
	if(do_controller != state_MIX)
    {
        CHIME_StartStop.setStatus(Active);
        START_STATE = true;
    }	
}

void state_SUCK_BACK()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
        // Signal Start of Cycle
		Serial.println("\nSUCK BACK\n");

        SetupMotors(BACKWARD, CONFIG_SuckBackVolume, CONFIG_SuckBackRate, Settings.Ratio);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        do_controller = state_END_CYCLE;
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(false))
    {
        MOTOR_StopAllMotors(); 
        do_controller = state_END_CYCLE;
    }
    

    // Exit State Clean-up
	if(do_controller != state_SUCK_BACK)
		START_STATE = true;
}

void state_SHORT_SHOT()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
        // Signal Start of Cycle
        // CHIME_StartStop.setStatus(Active);
		Serial.println("\nSHORT SHOT\n");

        SetupMotors(FORWARD, CONFIG_ShortShotVolume, CONFIG_ShortShotRate, Settings.Ratio);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        // if(LiquidSensor_Blue.hasLiquid() && LiquidSensor_Red.hasLiquid())
        //     CHIME_ShortShotSuccess.setStatus(Active);
        do_controller = state_END_CYCLE;
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(false))
    {
        MOTOR_StopAllMotors(); 
        do_controller = state_END_CYCLE;
    }
    

    // Exit State Clean-up
	if(do_controller != state_SHORT_SHOT)
		START_STATE = true;
}

void state_KEEP_OPEN()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
        // Signal Start of Cycle
        // CHIME_StartStop.setStatus(Active);
		Serial.println("\nSHORT SHOT\n");

        SetupMotors(FORWARD, CONFIG_KeepOpenVolume, CONFIG_KeepOpenRate, Settings.Ratio);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        do_controller = state_END_CYCLE;
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(true))
    {
        MOTOR_StopAllMotors(); 
        do_controller = state_END_CYCLE;
    }
    

    // Exit State Clean-up
	if(do_controller != state_KEEP_OPEN)
		START_STATE = true;
}

void state_FLUSH_PURGE()
{
	static bool START_STATE = true;
	if(START_STATE)
	{
        // Signal Start of Cycle
		Serial.printlnf("\nFLUSH_PURGE %s\n", (FirstFlush) ? "FIRST" : "FINAL");

        if(FirstFlush)
            SetupMotors(FORWARD, CONFIG_FlushFirstBolusVolume, CONFIG_FlushRate, 100);
        else
            SetupMotors(FORWARD, CONFIG_FlushFinalBolusVolume, CONFIG_FlushRate, 100);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

 		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        if(FirstFlush)
        {
            FirstFlush = false;
            do_controller = state_FLUSH_BACK_AND_FORTH;
        }
        else
        {
            FirstFlush = true;
            do_controller = state_END_CYCLE;
        }
        
        
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(false))
    {
        MOTOR_StopAllMotors();
        FirstFlush = true;
        do_controller = state_END_CYCLE;
    }
         
    

    // Exit State Clean-up
	if(do_controller != state_FLUSH_PURGE)
		START_STATE = true;
}

void state_FLUSH_BACK_AND_FORTH()
{
    
	static bool START_STATE = true;
    static uint CycleCounter = 0;
    static bool dir = 1; // 0: Backward (Back) 1: Forward (FORTH)
	if(START_STATE)
	{
        // Signal Start of Cycle
		Serial.printlnf("\nFLUSH %s: CYCLE: %d\n", (dir) ? "FORTH" : "BACK", CycleCounter);

        if(dir)
            SetupMotors(FORWARD, CONFIG_FlushForwardVolume, CONFIG_FlushRate, 100);
        else
            SetupMotors(BACKWARD, CONFIG_FlushBackwardVolume, CONFIG_FlushRate, 100);

        // Reset Motor Interrupt Trigger
        MOTOR_Monitor(true);

 		START_STATE = false;
	}

    // Monitor Motor for move completion
    MotorInterruptReason InterruptReason = MOTOR_Monitor();
    if(InterruptReason == MoveCompleted)
    {
        if(dir)
        {
            dir = 0;
            START_STATE = true;
            delay(CONFIG_FlushPauseTime);
        }
        else
        {
             if(CycleCounter < CONFIG_FlushBackAndForthCycles)
            {
                dir = 1;
                CycleCounter += 1;
                START_STATE = true;
                delay(CONFIG_FlushPauseTime);
            }
            else
            {
                // Reset Cycle Counter
                CycleCounter = 0;
                do_controller = state_FLUSH_PURGE;
            }
        }
    }

    // Moniter Early Cancel Inputs
    if(EarlyCancel(false))
    {
        MOTOR_StopAllMotors();
        CycleCounter = 0;
        dir = 1;
        do_controller = state_END_CYCLE;
    }
        
    // Exit State Clean-up
	if(do_controller != state_FLUSH_BACK_AND_FORTH)
		START_STATE = true;
}

void state_END_CYCLE()
{
    Serial.println("\nEND CYCLE\n");
    // CHIME_StartStop.setStatus(Active);
    LastRunTime = millis();
    FirstFlush = true;
    do_controller = state_IDLE;
}

void SetupMotors(const Direction Direction, const uint Volume, const uint Rate, const uint Ratio)
{
    // Calculate RevsRed and RevsBlue
    /// Revs = mGal * Cu-in/gal / mCu-in/Rev
    uint TotalRevs = Volume * CubicInchesPerGallon / CONFIG_MilliCubicInchesPerRevolution;
    uint RevsBlue = (uint)((float)TotalRevs / (Ratio + 10000) * Ratio);
    uint RevsRed = (uint)((float)TotalRevs / (Ratio + 10000) * 10000);

    Serial.printlnf("Total Revs: %d, Blue Revs: %d, Red Revs: %d", TotalRevs, RevsBlue, RevsRed);

    SetupMotors(Direction, RevsRed, RevsBlue, Rate, Ratio);
}

void SetupMotors(const Direction Direction, const uint RevsBlue, const uint RevsRed, const uint Rate,  const uint Ratio)
{
    // Enable Motors
    Wire.beginTransmission(4);
    Wire.write(Motor_EN_Reg);
    Wire.write(0b00000111); // Motor_EN_Reg - Enable all 3 motors
    Wire.endTransmission();
    
    // Set Directions
    MOTOR_SetDirection(Blue, Direction);
    MOTOR_SetDirection(Red, Direction);

    // Set Steps
    /// Steps = Revolutions * Steps/Revolution
    uint BlueSteps = RevsBlue * CONFIG_StepsPerRev;
    uint RedSteps = RevsRed * CONFIG_StepsPerRev;

    Serial.printlnf("Blue Steps: %d, Red Steps: %d", BlueSteps, RedSteps);

    MOTOR_SetTarget(Blue, BlueSteps);
    MOTOR_SetTarget(Red, RedSteps);

    // Set Speeds
    /// Steps/Sec = mGPM * Cu-in/gal * Steps/Rev / mCu-in/Rev / 60
    uint TotalStepsPerSecond = (uint)((float)Rate * CubicInchesPerGallon * CONFIG_StepsPerRev / CONFIG_MilliCubicInchesPerRevolution / 60 * 1000);
    uint BlueStepsPerSecond = (uint)((float)TotalStepsPerSecond / (Ratio + 10000) * Ratio);
    uint RedStepsPerSecond = (uint)((float)TotalStepsPerSecond / (Ratio + 10000) * 10000);

    Serial.printlnf("Total Steps/sec: %d,  Blue Steps/Sec: %d, Red Steps/Sec: %d", TotalStepsPerSecond, BlueStepsPerSecond, RedStepsPerSecond);

    MOTOR_SetSpeed(Blue, BlueStepsPerSecond);
    MOTOR_SetSpeed(Red, RedStepsPerSecond);

    // Move Motors
    Wire.beginTransmission(4);
    Wire.write(Motor_MOVE_Reg);
    Wire.write(0b00000011);
    Wire.endTransmission();
}

bool EarlyCancel(bool CheckLiquid)
{
    if( Remote.getStatus() == ShortPress ||
        PailSensor.getState() == 0 ||
        PressureManager.charged() == 0
      )
    {
        
        Serial.printlnf("\nEARLY CANCEL: %s", (PailSensor.getState() == 0) ? "Pail" : "Pressure");
        return true;
    }
    if(CheckLiquid)
    {
        if( LiquidSensor_Blue.hasLiquid() == 0 ||
            LiquidSensor_Red.hasLiquid() == 0
          )
        {
            Serial.println("\nEARLY CANCEL: Liquid");
            return true;
        }
    }

    return false;
}