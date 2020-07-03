#ifndef LIQUID_SENSOR_H
#define LIQUID_SENSOR_H

#include "Particle.h"
#include "config.h"
#include "globals.h"
#include "VDW_MCP23017.h"

class liquidSensor
{
  public:
    liquidSensor();

    // Init
    /// Initiates Pins used by the liquid sensor
    /// \param[VDW_MCP23017] *ioExp - The I/O Expander the user remote is attached to
    /// \param[uint8_t] pin - the IO Expander pin the liquid sensor is connected to
    void init(VDW_MCP23017 *ioExp, uint8_t pin);

    // Update 
    /// Checks Liquid Sensors for presesence if detection is enabled
    /// Locks into no liquid state once triggered.
    /// Should be called every loop
    void update();

    // Has Liquid
    /// Returns current state of liquid presence
    /// \return[bool] - true = liquid is present, false = liquid is not present
    bool hasLiquid();

    // Enable Detection
    /// Allows the Liquid Sensor to start looking for liquid presence
    /// Different from setting enableLiquidSensors == false
    /// By default, detection is off and must be turned on
    /// \param[bool] enable - turns detection on or off (default true)
    void enableDetection(bool enable = true);

  private:
    VDW_MCP23017 *_ioExp;
    uint8_t _pin = 0;

    bool _hasLiquid = true;
    bool _detectionEnabled = false;
    bool _liquidReDetected = false;
    uint _liquidReDetectedTime = 0;
};

extern liquidSensor LiquidSensor_Red;
extern liquidSensor LiquidSensor_Blue;

#endif
