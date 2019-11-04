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
    

    
public:
    KeypadTaak(KeypadListener *Keypadlistener):
    task                (4, "KeypadTaak"),
    KeyClock(this, 100'000, "KeyClock"),
    Keypadlistener      (Keypadlistener)
    {
        
    }
    
    void main();
};





#endif