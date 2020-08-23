#ifndef CONTROLLER_H
#define CONTROLLER_H
// Motor State Machine function pointer.
extern void (*do_controller)(void);

// Motor Mode
enum _MotorMode
{
	MIX,
	FLUSH
};
extern _MotorMode MotorMode;

// States

/// INIT
/// Sets the motor accelerations
void state_INIT();

/// IDLE
/// Runs the motor in reverse at Idle Speed. Waits for user input to run cycles. After first cycle, calls KEEP OPEN state periodically.
void state_IDLE();

/// MIX
/// Runs the motors forward on ratio for a full mix cycle defined by Settings.MixRate and Settings.Volume
void state_MIX();

/// SHORT_SHOT
/// Runs the motors a short distance forward on ratio defined by CONFIG_ShortShotVolume and CONFIG_ShortShotRate
void state_SHORT_SHOT();

/// SUCK BACK
/// Runs the motors backwards a short distance on ratio defined by CONFIG_SuckBackVolume and CONFIG_SuckBackRate
void state_SUCK_BACK();

/// KEEP OPEN
/// Periodically runs the motors forwards on ratio a short distance defined by CONFIG_KeepOpenVolume and
/// CONFIG_KeepOpenRate
void state_KEEP_OPEN();

/// FLUSH PURGE
/// Runs the motors forward at 1:1 ratio for a distance define by CONFIG_FlushFirstBolusVolume or 
/// CONFIG_FlushFinalBolusVolume at CONFIG_FlushRate
void state_FLUSH_PURGE();

/// FLUSH_BACK_AND_FORTH
/// Runs the motors backwards and forwards at CONFIG_FlushRate for CONFIG_FlushForwardVolume or 
/// CONFIG_FlushBackwardVolume, pausing for CONFIG_FlushPauseTime
void state_FLUSH_BACK_AND_FORTH();

/// END_CYCLE
/// Resets LastRunTime and FirstFlush
void state_END_CYCLE();

#endif