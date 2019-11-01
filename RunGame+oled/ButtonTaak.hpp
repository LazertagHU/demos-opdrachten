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
    ButtonTaak(KeypadListener *Keypadlistener):
    task                ("ButtonTaak"),
    ButtonClock(this, 100'000, "ButtonClock"),
    Keypadlistener      (Keypadlistener)
    {}
    
    void main() override{
        enum class ButtonState_t        {WAIT_FOR_INPUT, PRESSED};
        ButtonState_t ButtonState       = ButtonState_t::WAIT_FOR_INPUT;
        auto Button = hwlib::target::pin_in(hwlib::target::pins::d3);
        
        for(;;){
            
            switch(ButtonState){
                case ButtonState_t::WAIT_FOR_INPUT:
                    wait(ButtonClock);
                    if(!Button.read() && !buttonPressed ){
                        buttonPressed = true;
                        Keypadlistener->KeyPressed('E');
                        ButtonState = ButtonState_t::PRESSED;
                        break;
                    }
                    break;
                    
                case ButtonState_t::PRESSED:
                    wait(ButtonClock);
                    if(Button.read()){
                        buttonPressed = false;
                        ButtonState = ButtonState_t::WAIT_FOR_INPUT;
                        break;
                    }
                    break;
                
                default: break;

            }
        }
    }
};





#endif