#include "ButtonTaak.hpp"

void ButtonTaak::main(){
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
};