#include "rf_remote.h"

rfRemote Remote;

// Function to make the IO read


void rfRemote::init(VDW_MCP23017 *ioExp, uint8_t pin)
{
    _ioExp = ioExp;
    _pin = pin;

    // Setup Pin as INPUT with PULLUP(true) triggered on FALLING edge
    _ioExp->setMode(_pin, INPUT, true);
    _ioExp->setInterrupt(_pin, CHANGE);
}

void rfRemote::update()
{
    // Debounce and detect whether button is short or long click (Re-worked from pkourney/clickButton)
    uint32_t now = millis();
    _buttonState = !_ioExp->readPin(_pin); // our button is active-low, invert signal

    // If the switch changed, due to noise or a button press, reset the debounce timer
    if (_buttonState != _lastState) _lastBounceTime = now;


    // debounce the button (Check if a stable, state change has occured)
    if (now - _lastBounceTime > debounceTime && _buttonState != depressed)
    {
      depressed = _buttonState;
      if(depressed) _clickCount++;
    }

    // If the button released state is stable, report number of clicks and start new cycle
    if (!depressed && (now - _lastBounceTime) > multiclickTime)
    {
      // positive count for released buttons
      clicks = _clickCount;
      _clickCount = 0;
    }

    // Check for "long click"
    if (depressed && (now - _lastBounceTime > longClickTime))
    {
      // negative count for long clicks
      clicks = 0 - _clickCount;
      _clickCount = 0;
    }

    _lastState = _buttonState;

    if(clicks > 0)
        FLAG_ShortPress = true;
    else if(clicks < 0)
        FLAG_LongPress = true;
}

PressType rfRemote::getStatus()
{
    if(FLAG_ShortPress)
    {
        FLAG_ShortPress = false;
        return ShortPress;
    }
    else if(FLAG_LongPress)
    {
        FLAG_LongPress = false;
        return LongPress;
    }
    else
        return None;
    
}