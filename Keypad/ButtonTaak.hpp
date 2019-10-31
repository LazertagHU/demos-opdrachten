#ifndef _BUTTONTAAK_HPP
#define _BUTTONTAAK_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"

enum class ButtonState_t        {WAIT_FOR_INPUT, PRESSED};


class ButtonTaak : public rtos::task<>{
private:

    rtos::timer                                         ButtonTimer;
    KeypadListener                                      *Keypadlistener;
    bool                                                buttonPressed = false;
    
public:
    ButtonTaak(KeypadListener *Keypadlistener):
    task                ("ButtonTaak"),
    ButtonTimer            (this, "ButtonTimer"),
    Keypadlistener      (Keypadlistener)
    {}
    
    void main() override{
        
        ButtonState_t ButtonState       = ButtonState_t::WAIT_FOR_INPUT;
        auto Button = hwlib::target::pin_in(hwlib::target::pins::d3);
        
        for(;;){
            
            switch(ButtonState){
                
                
                case ButtonState_t::WAIT_FOR_INPUT:
                    ButtonTimer.set(100000);
                    wait(ButtonTimer);
                    if(!Button.read() && !buttonPressed ){
                        buttonPressed = true;
                        Keypadlistener->KeyPressed('E');
                        ButtonState = ButtonState_t::PRESSED;
                        break;
                    }
                    break;
                    
                case ButtonState_t::PRESSED:
                    ButtonTimer.set(100000);
                    wait(ButtonTimer);
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