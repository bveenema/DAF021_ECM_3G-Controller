#include "LiquidSensor.h"

liquidSensor LiquidSensor_Red;
liquidSensor LiquidSensor_Blue;

liquidSensor::liquidSensor(){}

void liquidSensor::init(VDW_MCP23017 *ioExp, uint8_t pin)
{
	_ioExp = ioExp;
	_pin = pin;

	// Setup pin
	_ioExp->setMode(_pin, INPUT);
	_ioExp->setInterrupt(_pin, CHANGE);
}

void liquidSensor::update()
{
	// If signal is low, there is no liquid, only look if detection is enabled
	if(_detectionEnabled && !_ioExp->readPin(_pin))
	{
		_hasLiquid = false;

		// if liquid has been previously detected, set the LiquidError chime
		if(_liquidErrorEnabled)
		{
			// CHIME_LiquidError.setStatus(Active);
			_liquidErrorEnabled = false;
		}
			
	}
		

	// If there is no liquid and detection is enabled, look for liquid to return for 5 seconds
	if(_detectionEnabled && _hasLiquid == false)
	{
		// Mark the time liquid is first detected
		if(_liquidReDetected == false && _ioExp->readPin(_pin))
		{
			_liquidReDetectedTime = millis();
			_liquidReDetected = true;
		}

		// Reset if Liquid dissapears again
		if(_ioExp->readPin(_pin) == false)
		{
			_liquidReDetected = false;
		}

		// If liquid is still present after 500 ms, re-enable _hasLiquid
		if(_liquidReDetected && millis() - _liquidReDetectedTime > 1000)
		{
			_hasLiquid = true;
			// if(_liquidErrorEnabled == false)
			// 	CHIME_ShortShotSuccess.setStatus(Active);
			_liquidErrorEnabled = true;
		}
	}
}

void liquidSensor::enableDetection(bool enable)
{
	_detectionEnabled = enable;
}

bool liquidSensor::hasLiquid()
{
	// if(!settings.enableLiquidSensors) return true;
	return _hasLiquid;
}
