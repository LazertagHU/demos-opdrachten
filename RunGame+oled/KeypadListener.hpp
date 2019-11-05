#ifndef _KEYPADLISTENER_HPP
#define _KEYPADLISTENER_HPP



class KeypadListener{
private:

    
public:
    /// \brief
    /// Purely virtual and abstract class used for the listener patern.
    virtual void KeyPressed(char Key) = 0;

};

#endif


