#ifndef _TESTTAAKKEYPAD_HPP
#define _TESTTAAKKEYPAD_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"
#include "InputListener.hpp"
#include "InputControlTaak.hpp"


class TestTaakKeypad : public rtos::task<>, public InputListener{
private:

    InputControlTaak                                            InputControl;
    rtos::channel<E_Buttons, 10>                                TestChannel;                                               
public:

    void InputMessage(E_Buttons Ebutton)override{TestChannel.write(Ebutton);};

    TestTaakKeypad():
        task                ("TestTaakKeypad"),
        InputControl        (this),
        TestChannel         (this, "TestChannel")
    {}
    void main(){
        for(;;){
            auto evt = TestChannel.read();
            switch(evt){
                    case E_Buttons::zero_button:
                        hwlib::cout << "0\n";
                        break;
                    case E_Buttons::one_button:
                        hwlib::cout << "1\n";
                        break;
                    case E_Buttons::two_button:
                        hwlib::cout << "2\n";
                        break;
                    case E_Buttons::three_button:
                        hwlib::cout << "3\n";
                        break;
                    case E_Buttons::four_button:
                        hwlib::cout << "4\n";
                        break;
                    case E_Buttons::five_button:
                        hwlib::cout << "5\n";
                        break;
                    case E_Buttons::six_button:
                        hwlib::cout << "6\n";
                        break;
                    case E_Buttons::seven_button:
                        hwlib::cout << "7\n";
                        break;
                    case E_Buttons::eight_button:
                        hwlib::cout << "8\n";
                        break;
                    case E_Buttons::nine_button:
                        hwlib::cout << "9\n";
                        break;
                    case E_Buttons::A_button:
                        hwlib::cout << "A\n";
                        break;
                    case E_Buttons::B_button:
                        hwlib::cout << "B\n";
                        break;
                    case E_Buttons::C_button:
                        hwlib::cout << "C\n";
                        break;
                    case E_Buttons::D_button:
                        hwlib::cout << "D\n";
                        break;
                    case E_Buttons::E_button:
                        hwlib::cout << "E\n";
                        break;
                    case E_Buttons::star_button:
                        hwlib::cout << "*\n";
                        break;
                    case E_Buttons::hash_button:
                        hwlib::cout << "#\n";
                        break;
                        
                    default: break;
                        
                }
            
            
        }
        
        
    }
};





#endif