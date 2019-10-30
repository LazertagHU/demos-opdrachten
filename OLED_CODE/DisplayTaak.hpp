#ifndef _OLEDTAAK_HPP
#define _OLEDTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"    
                                                                                    //////////////////////////////////////////////////////////////                                                               //TypeMessage:  een struct die meerdere types aan messages
struct TypeMessage{                                                                 //              aan kan nemen.
    int         IntToWrite;                                                         //ToWrite:      Bevat de INT voor het scherm 
    const char* StringToWrite;                                                      //StringToWrite Bevat de std::string voor het scherm
    char        Type;                                                               //Type:         Beslist voor welk schermdeel de tekst bedoel is
};                                                                                  //
                                                                                    //////////////////////////////////////////////////////////////


class DisplayTaak : public rtos::task<>{
private:

    rtos::channel<TypeMessage, 5>         inputChannel;                             //////////////////////////////////////////////////////////////
    
public:
                                                                                    //////////////////////////////////////////////////////////////
    DisplayTaak():                                                                  //CLASS CONSTRUCTOR
    task                ("DisplayTaak"),                                            //creeer de rtos::task
    inputChannel        (this, "inputChannel")                                      //creeer de Channel
    {}                                                                              //
                                                                                    //////////////////////////////////////////////////////////////
    
    void showMessage(const char* Message, char oledRec){
        TypeMessage sendMessage;
        sendMessage.StringToWrite   = Message;
        sendMessage.Type            = oledRec;
        inputChannel.write(sendMessage);
    }
    void showMessage(int Message, char oledRec){
        TypeMessage sendMessage;
        sendMessage.IntToWrite   = Message;
        sendMessage.Type            = oledRec;
        inputChannel.write(sendMessage);
    }

    void main();
    
    
    
};





#endif
