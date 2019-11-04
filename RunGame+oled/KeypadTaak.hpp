#ifndef _KEYPADTAAK_HPP
#define _KEYPADTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"


class KeypadTaak : public rtos::task<>{
private:

    rtos::clock                                         KeyClock;
    int                                                 Temp = 0;
    char                                                Key  = '\0';
    KeypadListener                                      *Keypadlistener;
    
    /// \brief
    /// Main() for the Keypad task.
    /// \details
    /// uses the hwlib::keypad to read the keypad buttons on a 100ms interval.
    /// It uses its listener to send the pressed button.
    void main() override;
    
public:
    /// \brief
    /// Default constructor for the KeypadTaak
    /// \details
    /// Names the task, starts a 100ms clock and binds the listener
    KeypadTaak(KeypadListener *Keypadlistener):
    task("KeypadTaak"),
    KeyClock(this, 100'000, "KeyClock"),
    Keypadlistener(Keypadlistener)
    {}
};





#endif