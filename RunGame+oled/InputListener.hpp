#ifndef _INPUTLISTENER_HPP
#define _INPUTLISTENER_HPP


enum class buttonid
{
    zeroButton, oneButton, twoButton, threeButton,
    fourButton, fiveButton, sixButton, sevenButton,
    eightButton, nineButton, aButton, bButton,
    cButton, dButton, eButton, starButton,
    hastagButton
};

class InputListener{
private:

    
public:
    
    virtual void InputMessage(buttonid id) = 0;

};

#endif


