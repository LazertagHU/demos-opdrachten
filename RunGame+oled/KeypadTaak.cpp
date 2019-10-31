#include "KeypadTaak.hpp"


enum class KeypadState_t        {WAIT_FOR_INPUT};

void KeypadTaak::main(){

    KeypadState_t   KeypadState =   KeypadState_t::WAIT_FOR_INPUT;
    auto out0                   =   hwlib::target::pin_oc(hwlib::target::pins::a0);
    auto out1                   =   hwlib::target::pin_oc(hwlib::target::pins::a1);
    auto out2                   =   hwlib::target::pin_oc(hwlib::target::pins::a2);
    auto out3                   =   hwlib::target::pin_oc(hwlib::target::pins::a3);
    
    auto in0                    =   hwlib::target::pin_in(hwlib::target::pins::a4);
    auto in1                    =   hwlib::target::pin_in(hwlib::target::pins::a5);
    auto in2                    =   hwlib::target::pin_in(hwlib::target::pins::a6);
    auto in3                    =   hwlib::target::pin_in(hwlib::target::pins::a7);
    
    auto out_port               =   hwlib::port_oc_from(out0, out1, out2, out3);
    auto in_port                =   hwlib::port_in_from(in0, in1, in2, in3);
    
    auto matrix                 =   hwlib::matrix_of_switches(out_port, in_port);
    auto Keypad                 =   hwlib::keypad<16>(matrix, "147*2580369#ABCDE");
    
    for(;;){
        switch(KeypadState){
            case KeypadState_t::WAIT_FOR_INPUT:
                KeyTimer.set(20000);
                auto evt    = wait(KeyTimer);
                Temp = Keypad.pressed();
                if(Temp == '\0'){
                    break;
                }
                else if(Temp != '\0'){
                    Key = Keypad.getc();
                    Keypadlistener->KeyPressed(Key);
                    break;
                }
        }
        
    }
    
    
}