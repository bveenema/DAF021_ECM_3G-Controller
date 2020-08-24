#ifndef PressureManager_H
#define PressureManager_H

#include "Particle.h"
#include "VDW_MCP23017.h"
#include "VDW_PressureSensor_HoneyWell_HSC_SSC.h"

#define OFF 0
#define ON 1
#define CLOSED 0
#define OPEN 1

enum PressureManagerState
{
    PM_Init,
    PM_Empty,
    PM_Charge,
    PM_DetermineSize,
    PM_Settle,
    PM_LeakCheck,
    PM_Ready,
    PM_Error,
};

// Pressure Manager get passed the pressure sensor objects. It manages keeping them updated and getting pressures when needed.
/// but initialization and zeroing of pressure sensors is left to the main loop.

class pressureManager
{
    public:
        // constructor
        pressureManager(){}

        // init
        /// Set initial on and off pressures, initialize the pump and valve. Call in setup
        /// Pass sensors as reference
        /// \param[VDW_MCP23017] *ioExp - the I/O expander the pump and valves are attached to
        /// \param[uint8_t] pumpPin - the pin the pump is attached to on I/O expander
        /// \param[uint8_t] valvePin - the pin the valve is attached to on I/O expander
        /// \param[VDW_PressureSensor_Honeywell_HSC_SSC] *Blue - pointer to the Blue Pressure Sensor
        /// \param[VDW_PressureSensor_Honeywell_HSC_SSC] *Blue - pointer to the Red Pressure Sensor
        void init(VDW_MCP23017 *ioExp, uint8_t pumpPin, uint8_t valvePin, VDW_PressureSensor_Honeywell_HSC_SSC *Blue, VDW_PressureSensor_Honeywell_HSC_SSC *Red);

        // update
        /// Check pressure, turn pump on/off, open/close air exhaust valve. Must be called every loop
        /// \return[PressureManagerState] - the current state of the pressure manager
        PressureManagerState update();

        // getState
        /// Returns the current state of the Pressure Manager
        /// \return[PressureManagerState] - the current state of the pressure manager
        PressureManagerState getState();

        // setOnPressure, setOffPressure, setEmptyPressure, setTestPressure, setLeakPercentage
        // setMaxChargeTime, setSmallReservoirTime, setSettleTime, setLeakTestTime
        /// Set new on and off pressures. Ensures pressure is in valid range.
        /// \param[int32_t] pressure - the new pressure
        /// \return[int32_t] - the actual pressure set (ranged)
        int32_t setEmptyPressure(int32_t pressure);
        int32_t setTestPressure(int32_t pressure);
        uint8_t setLeakPercentage(uint8_t percentage);
        uint setMaxChargeTime(uint time);
        uint setSmallReservoirTime(uint time);
        uint setSettleTime(uint time);
        uint setLeakTestTime(uint time);

        // Charged
        /// Returns true if the system is charged
        /// \return[bool] - true if system is charged
        bool charged();

    private:
        VDW_MCP23017 *_ioExp;
        uint8_t _pumpPin, _valvePin,  _leakPercentage;
        VDW_PressureSensor_Honeywell_HSC_SSC *_pBlue, *_pRed;
        int32_t _onPressure, _offPressure, _emptyPressure, _testPressure, _leakTestPressure;
        uint _maxChargeTime, _smallReservoirTime, _settleTime, _leakTestTime;
        bool _isCharged = false;

        PressureManagerState PM_State = PM_Init;


        // Turn Pump on(1) or off(0)
        void setPump(bool state);

        // Open(1)/Close(0) Valve
        void setValve(bool state);



};

extern pressureManager PressureManager;

#endif