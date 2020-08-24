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

    PM_State = PM_Init;
}

// Check pressure, test reservoirs turn pump on/off, open/close air exhaust valve
PressureManagerState pressureManager::update()
{
    int32_t pressure = _pBlue->getPressureAverage();

    static uint TestTimer = 0;
    static char ErrorCause[32];
    enum ReservoirSize
    {
        Undetermined,
        Small,
        Large,
    };
    static ReservoirSize R_Size = Undetermined;

    // Handle Pump
    /// Initialize the PressureManager state machine
    if(PM_State == PM_Init)
    {
        if(CONFIG_SkipPressureTest)
        {
            Serial.println("PM_Ready");
            PM_State = PM_Ready;
        }
        else
        {
            TestTimer = millis();
            Serial.println("PM_Empty");
            PM_State = PM_Empty;
        }
    }

    /// Make sure the reservoir is empty
    if(PM_State == PM_Empty)
    {
        setValve(OPEN);
        if(millis() - TestTimer > 1000)
        {
            if(pressure <= _emptyPressure)
            {
                setValve(CLOSED);
                setPump(ON);
                TestTimer = millis();
                Serial.println("PM_Charge");
                PM_State = PM_Charge;
            }
        }
    }

    /// Charge Reservoir to 1.5 PSI
    if(PM_State == PM_Charge)
    {
        if(millis() - TestTimer > _maxChargeTime)
        {
            setPump(OFF);
            snprintf(ErrorCause, 32, "Failed to Reach Test Pressure");
            Serial.println("PM_Error");
            PM_State = PM_Error;
        }
        if(pressure >= _testPressure)
        {
            setPump(OFF);
            Serial.println("PM_DetermineSize");
            PM_State = PM_DetermineSize;
        }
    }

    /// Determine Reservoir Size
    if(PM_State == PM_DetermineSize)
    {
        if(millis() - TestTimer <= _smallReservoirTime)
            R_Size = Small;
        else
            R_Size = Large;

        if(R_Size == Small)
        {
            Serial.println("Reservoir Size: Small");
            _onPressure = CONFIG_PumpOnPressure_Small;
            _offPressure = CONFIG_PumpOffPressure_Small;
        }
        else if(R_Size == Large)
        {
            Serial.println("Reservoir Size: Large");
            _onPressure = CONFIG_PumpOnPressure_Large;
            _offPressure = CONFIG_PumpOffPressure_Large;
        }
        
        TestTimer = millis();
        Serial.println("PM_Settle");
        PM_State = PM_Settle;
    }

    /// Settle
    if(PM_State == PM_Settle)
    {
        if(millis() - TestTimer > _settleTime)
        {
            _leakTestPressure = pressure * (100 - _leakPercentage) / 100; // leak test pressure is a percentage of the settled pressure
            Serial.printlnf("PM_LeakCheck: %d", _leakTestPressure);
            PM_State = PM_LeakCheck;
        }
    }

    /// Detect Leaks
    if(PM_State == PM_LeakCheck)
    {
        if(pressure < _leakTestPressure)
        {
            snprintf(ErrorCause, 32, "Leak Detected");
            Serial.println("PM_Error");
            PM_State = PM_Error;
        }
        if(millis() - TestTimer > _leakTestTime)
        {
            Serial.println("PM_Ready");
            PM_State = PM_Ready;
        }
    }

    /// Manage Pump Level
    if(PM_State == PM_Ready)
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

    /// Pressure Error
    if(PM_State == PM_Error)
    {
        static bool PrintOnce = true;
        if(PrintOnce)
        {
            Serial.printlnf("Pressure Error: %s", ErrorCause);
            PrintOnce = false;
        }     
    }

    return PM_State;
}

PressureManagerState pressureManager::getState()
{
    return PM_State;
}

// Set new empty, test pressures
int32_t pressureManager::setEmptyPressure(int32_t pressure)
{
    if(pressure < 0)
        pressure = 0;
    
    _emptyPressure = pressure;
    
    return _emptyPressure;
}

int32_t pressureManager::setTestPressure(int32_t pressure)
{
    if(pressure > CONFIG_MaxChargePressure)
        pressure = CONFIG_MaxChargePressure;

    _testPressure = pressure;

    return _testPressure;
}

// Set Leak Percentage
uint8_t pressureManager::setLeakPercentage(uint8_t percentage)
{
    if(percentage > 100)
        percentage = 100;

    _leakPercentage = percentage;

    return _leakPercentage;
}

// Set Max Charge Time, Small Reservoir Time, Settle Time and Leak Test Time
uint pressureManager::setMaxChargeTime(uint time)
{
    _maxChargeTime = time;

    return _maxChargeTime;
}

uint pressureManager::setSmallReservoirTime(uint time)
{
    _smallReservoirTime = time;

    return _smallReservoirTime;
}

uint pressureManager::setSettleTime(uint time)
{
    _settleTime = time;

    return _settleTime;
}

uint pressureManager::setLeakTestTime(uint time)
{
    _leakTestTime = time;

    return _leakTestTime;
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