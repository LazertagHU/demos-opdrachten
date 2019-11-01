#ifndef _KEYPADTAAK_HPP
#define _KEYPADTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"


class KeypadTaak : public rtos::task<>{
private:

    rtos::timer                                         KeyTimer;
    int                                                 Temp = 0;
    char                                                Key  = '\0';
    KeypadListener                                      *Keypadlistener;
    

    
public:
    KeypadTaak(KeypadListener *Keypadlistener):
    task                ("KeypadTaak"),
    KeyTimer            (this, "KeyTimer"),
    Keypadlistener      (Keypadlistener)
    {
        hwlib::cout << "\nkeypad constructor\n";
    }
    
    void main();
};





#endif