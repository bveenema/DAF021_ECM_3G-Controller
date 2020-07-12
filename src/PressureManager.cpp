#include "PressureManager.h"
#include "config.h"
#include "globals.h"

pressureManager PressureManager;


// Set initial on and off pressures, initialize the pump and valve
void pressureManager::init(VDW_MCP23017 *ioExp, uint8_t pumpPin, uint8_t valvePin, VDW_PressureSensor_Honeywell_HSC_SSC *Blue, VDW_PressureSensor_Honeywell_HSC_SSC *Red)
{
    _ioExp = ioExp;
    _pumpPin = pumpPin;
    _valvePin = valvePin;
    _pBlue = Blue;
    _pRed = Red;

    // Initialize Pumps OFF (Only Blue Pump used for now)
    _ioExp->setMode(_pumpPin, OUTPUT);
    setPump(OFF);

    // Close Air Exhaust Valve (Valve 1)
    _ioExp->setMode(_valvePin, OUTPUT);
    setValve(CLOSED);
}

// Check pressure, turn pump on/off, open/close air exhaust valve
void pressureManager::update()
{
    int32_t pressure = _pBlue->getPressureAverage();

    // Handle Pump
    if(Settings.valid)
    {
        if(pressure <= _onPressure)
            setPump(ON);
        else if(pressure >= _offPressure)
        {
            setPump(OFF);
            _isCharged = true;
        }
        if(pressure < CONFIG_MinValidPressure)
            _isCharged = false;
    }
    else
    {
        setPump(OFF);
    }
    

}

// Set new on and off pressures
int32_t pressureManager::setOnPressure(int32_t pressure)
{
    if(pressure > CONFIG_MaxChargePressure)
        pressure = CONFIG_MaxChargePressure;

    _onPressure = pressure;

    return _onPressure;
}
int32_t pressureManager::setOffPressure(int32_t pressure)
{
    if(pressure > CONFIG_MaxChargePressure)
        pressure = CONFIG_MaxChargePressure;

    _offPressure = pressure;

    return _offPressure;
}

bool pressureManager::charged()
{
    return _isCharged;
}

// Turn Pump on(1) or off(0)
void pressureManager::setPump(bool state)
{
    _ioExp->writePin(_pumpPin, state);
}

// Open(1)/Close(0) Valve
void pressureManager::setValve(bool state)
{
    _ioExp->writePin(_valvePin, !state);
}