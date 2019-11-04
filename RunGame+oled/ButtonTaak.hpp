#ifndef _BUTTONTAAK_HPP
#define _BUTTONTAAK_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"

class ButtonTaak : public rtos::task<>{
private:

    rtos::clock                                         ButtonClock;
    KeypadListener                                      *Keypadlistener;
    bool                                                buttonPressed = false;

    
public:

    /// \brief
    /// Default constructor for ButtonTaak
    /// \details
    /// This constructor names its task, starts its clock and binds the listener.

    ButtonTaak(KeypadListener *Keypadlistener):
    task                ("ButtonTaak"),
    ButtonClock(this, 100'000, "ButtonClock"),
    Keypadlistener      (Keypadlistener)
    {}
    
    /// \brief
    /// Main() for the ButtonTask
    /// \details
    /// This tasks job is to notify listening task of changes to the button. 
    /// When it reads a input, the KeyPressed(char) function of the listening class is called

    void main() override;
};





#endif