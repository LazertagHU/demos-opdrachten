#include "InputControlTaak.hpp"

enum class  InputState_t       {WAIT_FOR_INPUT};


void InputControlTaak::main(){
    InputState_t   InputState =   InputState_t::WAIT_FOR_INPUT;
    for(;;){
        switch(InputState){
            
            case InputState_t::WAIT_FOR_INPUT:
                Key = InputChannel.read();
                switch(Key){
                    case '0':
                        Inputlistener->InputMessage(buttonid::zeroButton);
                        break;
                    case '1':
                        Inputlistener->InputMessage(buttonid::oneButton);
                        break;
                    case '2':
                        Inputlistener->InputMessage(buttonid::twoButton);
                        break;
                    case '3':
                        Inputlistener->InputMessage(buttonid::threeButton);
                        break;
                    case '4':
                        Inputlistener->InputMessage(buttonid::fourButton);
                        break;
                    case '5':
                        Inputlistener->InputMessage(buttonid::fiveButton);
                        break;
                    case '6':
                        Inputlistener->InputMessage(buttonid::sixButton);
                        break;
                    case '7':
                        Inputlistener->InputMessage(buttonid::sevenButton);
                        break;
                    case '8':
                        Inputlistener->InputMessage(buttonid::eightButton);
                        break;
                    case '9':
                        Inputlistener->InputMessage(buttonid::nineButton);
                        break;
                    case 'A':
                        Inputlistener->InputMessage(buttonid::aButton);
                        break;
                    case 'B':
                        Inputlistener->InputMessage(buttonid::aButton);
                        break;
                    case 'C':
                        Inputlistener->InputMessage(buttonid::cButton);
                        break;
                    case 'D':
                        Inputlistener->InputMessage(buttonid::dButton);
                        break;
                    case 'E':
                        Inputlistener->InputMessage(buttonid::eButton);
                        break;
                    case '*':
                        Inputlistener->InputMessage(buttonid::starButton);
                        break;
                    case '#':
                        Inputlistener->InputMessage(buttonid::hastagButton);
                        break;
                        
                    default: 
                        break;
                        
                }

        }
        
    }
    
    
}