#include "InputControlTaak.hpp"

enum class  InputState_t       {WAIT_FOR_INPUT};


void InputControlTaak::main(){
    InputState_t   InputState =   InputState_t::WAIT_FOR_INPUT;
    
    
    for(;;){
        switch(InputState){
            
            case InputState_t::WAIT_FOR_INPUT:
                auto evt    = wait(InputChannel);
                Key = InputChannel.read();
                switch(Key){
                    case '0':
                        Inputlistener->InputMessage(E_Buttons::zero_button);
                        break;
                    case '1':
                        Inputlistener->InputMessage(E_Buttons::one_button);
                        break;
                    case '2':
                        Inputlistener->InputMessage(E_Buttons::two_button);
                        break;
                    case '3':
                        Inputlistener->InputMessage(E_Buttons::three_button);
                        break;
                    case '4':
                        Inputlistener->InputMessage(E_Buttons::four_button);
                        break;
                    case '5':
                        Inputlistener->InputMessage(E_Buttons::five_button);
                        break;
                    case '6':
                        Inputlistener->InputMessage(E_Buttons::six_button);
                        break;
                    case '7':
                        Inputlistener->InputMessage(E_Buttons::seven_button);
                        break;
                    case '8':
                        Inputlistener->InputMessage(E_Buttons::eight_button);
                        break;
                    case '9':
                        Inputlistener->InputMessage(E_Buttons::nine_button);
                        break;
                    case 'A':
                        Inputlistener->InputMessage(E_Buttons::A_button);
                        break;
                    case 'B':
                        Inputlistener->InputMessage(E_Buttons::B_button);
                        break;
                    case 'C':
                        Inputlistener->InputMessage(E_Buttons::C_button);
                        break;
                    case 'D':
                        Inputlistener->InputMessage(E_Buttons::D_button);
                        break;
                    case 'E':
                        Inputlistener->InputMessage(E_Buttons::E_button);
                        break;
                    case '*':
                        Inputlistener->InputMessage(E_Buttons::star_button);
                        break;
                    case '#':
                        Inputlistener->InputMessage(E_Buttons::hash_button);
                        break;
                        
                    default: break;
                        
                }

        }
        
    }
    
    
}