#ifndef _INPUTLISTENER_HPP
#define _INPUTLISTENER_HPP


enum class  E_Buttons          {zero_button, one_button, two_button, three_button, four_button, five_button, six_button, seven_button, eight_button, nine_button, A_button, B_button, C_button, D_button, E_button, star_button, hash_button};

class InputListener{
private:

    
public:
    
    virtual void InputMessage(E_Buttons Ebutton) = 0;

};

#endif


