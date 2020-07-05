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
/// Initializes the Stepper Motor
/// 	Proceeds To: IDLE
///		ChangeState: ignores
///		Require next_state: false
void state_INIT();

/// IDLE
/// Stops the motor. Waits for user input to run cycles. After first cycle, calls KEEP OPEN state periodically.
///		Proceeds To: self
///		ChangeState: Proceed to CHARGE
///		Require next_state: false
void state_IDLE();

/// CHARGE
/// Checks for Air pump to fully charge or timeout
///		Proceeds To: CALCULATE_FOR_MIX or CALCULATE_FOR_INITIAL_FLUSH depending on MotorMode
///		ChangeState: Proceed to IDLE
///		Require next_state: false
void state_CHARGE();

/// MIX
/// Calculates distance to move based on Rev Limit. Sets 0 position of motor. Signals start of cycle.
///		Proceeds To: RUN_DISTANCE, then SUCK_BACK
///		ChangeState: ignores
///		Require next_state: false
void state_MIX();

/// SHORT_SHOT
/// Calculates distance to move for a Short Shot. Sets 0 position of motor.
///     Proceeds To: RUN_DISTANCE -> IDLE
///     ChangeState: ignores
///     Require next_state: false
void state_SHORT_SHOT();

/// SUCK BACK
/// Runs the motors backwards a short distance on ratio defined by SuckBackDistance and SuckBackRPM and RPM Ratio
///		Proceeds To: RUN_DISTANCE, then END_CYCLE unless next_state defined
///		ChangeState: ignores
///		Require next_state: false
void state_SUCK_BACK();

/// KEEP OPEN
/// Calculates and sets speeds and accelerations and sets short distance move to keep the mixer from clogging
///		Proceeds To: RUN_FOR_DISTANCE -> SUCK_BACK -> IDLE
///		ChangeState: ignores
///		Require next_state: false
void state_KEEP_OPEN();

/// PAUSE
/// Delays next step for PauseTime (ms)
///		Proceeds To: next_state
///		ChangeState: Proceed to END_CYCLE
///		Require next_state: true
void state_PAUSE();

/// FLUSH PURGE
/// Calculates and sets speeds and accelerations and sets practically indefinite target. Sets Start/Stop Chime active
///		Proceeds To: RUN_FOR_DISTANCE
///		ChangeState: ignores
///		Require next_state: false
void state_FLUSH_PURGE();

/// FLUSH_BACK_AND_FORTH
/// Causes motor to rotate back and forth. Calculates distance to move. Ends Cycle after time elapses
///		Proceeds To: RUN_FOR_DISTANCE
///		ChangeState: Proceed to END_CYCLE
///		Require next_state: false
void state_FLUSH_BACK_AND_FORTH();

/// RUN_FOR_DISTANCE
/// Calls run() functions until distanceToGo returns 0
///		Proceeds To: PAUSE
///		ChangeState: Proceed to END_CYLE
///		Require next_state: true
void state_RUN_FOR_DISTANCE();

/// END_CYCLE
/// Sets Start/Stop Chime Active.
///		Proceeds To: IDLE
///		ChangeState: ignores
///		Require next_state: false
void state_END_CYCLE();

#endif