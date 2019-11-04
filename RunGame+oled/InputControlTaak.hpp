#ifndef _INPUTCONTROLTAAK_HPP
#define _INPUTCONTROLTAAK_HPP
#include <array>
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "KeypadTaak.hpp"
#include "InputListener.hpp"
#include "ButtonTaak.hpp"


class InputControlTaak : public rtos::task<>, KeypadListener{
private:
    InputListener                                       *Inputlistener;
    rtos::channel<char, 18>                             InputChannel;
    char                                                Key  = '\0';
    KeypadTaak                                          Keypadtaak;
    ButtonTaak                                          Buttontaak;

public:
    /// \brief
    /// Default constructor for the InputControlTaak
    /// \details    
    /// Set task name, binds the listener, inits the channel and makes the button and keypad tasks
    InputControlTaak(InputListener *Inputlistener):
    task                (5, "InputControlTaak"),
    Inputlistener       (Inputlistener),
    InputChannel        (this, "InputChannel"),
    Keypadtaak          (this),
    Buttontaak          (this)
    {}
    /// \brief
    /// Interface for writing pressed buttons
    /// \details    
    /// Public function to write the pressed key to. This function internally uses a channel as waitable to save this incoming data.                                                                                
    void KeyPressed(char Key){InputChannel.write(Key);};

    /// \brief
    /// Main() for the Input Control task.
    /// \details
    /// this task waits on the channel for new presses, decodes them and sends them on to the listener
    void main();
};





#endif