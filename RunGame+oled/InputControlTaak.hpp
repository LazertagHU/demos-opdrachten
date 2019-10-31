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
    InputControlTaak(InputListener *Inputlistener):
    task                ("KeypadTaak"),
    Inputlistener       (Inputlistener),
    InputChannel        (this, "InputChannel"),
    Keypadtaak          (this),
    Buttontaak          (this)
    {}
    
    void KeyPressed(char Key){InputChannel.write(Key);};
    void main();
};





#endif