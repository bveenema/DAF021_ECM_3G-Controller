#ifndef RF_Remote_H
#define RF_Remote_H

#include "Particle.h"
#include "VDW_MCP23017.h"

enum PressType
{
    None,
    ShortPress,
    LongPress
};

// Remote interfaces with the RF wrist controller/remote. It provides a readable flag that is reset up read whenever the button is pressed
class rfRemote
{
    public:
        // constructor
        rfRemote(){};

        // init
        /// Specify IO expander and pin for remote. Setup pin as INPUT triggered on RISING edge
        /// \param[VDW_MCP23017] *ioExp - The I/O Expander the user remote is attached to
        /// \param[uint8_t] pin - The pressure at which the pump will turn on
        void init(VDW_MCP23017 *ioExp, uint8_t pin);

        // update
        /// Check if remote has been short or long pressed. Set corresponding flags.
        void update();

        // getStatus
        /// returns remote button status. Resets FLAGs when read
        /// \return[PressType] - the status of the remote button
        PressType getStatus();

        int32_t clicks = 0;
        bool depressed = 0;
        uint32_t debounceTime = 20;
        uint32_t multiclickTime = 250;
        uint32_t longClickTime = 5000;

    private:
        VDW_MCP23017 *_ioExp;
        uint8_t _pin;

        bool _buttonState = false;
        bool _lastState = false;

        int32_t _clickCount = 0;
        uint32_t _lastBounceTime = 0;


        bool FLAG_ShortPress = false;
        bool FLAG_LongPress = false;

};

extern rfRemote Remote;

#endif