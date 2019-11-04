//////////////////////////////////////////////////////////////
//DISPLAYTAAK.HPP 
//Themaproject Devices, groep 2
//Code is bedoelt voor de display van een lasertag spel
//
//FUNCTIONALITEIT
//  De hiernavolgende klas is bedoelt om via het aanroepen van
//  een public functie, waardes op een aangesloten OLED scherm
//  te plaatsen. De class bevat public aanroepbare functies,
//  die data(een string, char of int), en een Datatype.
//
//AANROEPBARE FUNCTIES
//- showMessage(const char* Message, char oledRec)
//      Deze functie heeft 2 parameters
//      - const char* Message
//          vraagt een const char* of string variabel,
//          dit variabel zal daarna op de meegegeven positie
//          geplaatst worden
//      - char oledRec
//          Vraagt een char Variabel. Dit variabel word gebruikt
//          om de positie op het scherm te specificeren, waar 
//          de inhoud van variabel "Message" geplaatst zal worden
//          Je kan hierbij kiezen uit
//              M(middelste vak)  : Bedoelt voor General berichten 
//                                  naar de speler.
//              N(vak Linksboven) : Bedoelt voor de naam/ID van de speler
//              
//              A(Vak Rechtsboven): Bedoelt voor het huidige aantal Ammo
//
//              H(Vak Linksonder) : Bedoelt voor het aangeven van de HP van de speler
//              
//              T(Vak Rechtsonder): Bedoelt voor het aangeven van de Speeltijd
//                                  (dit moet elke seconde worden aangepast)
//              G(Vak Rechtsboven): Bedoelt voor het aangeven van het Wapentype
//
//////////////////////////////////////////////////////////////



#ifndef _OLEDTAAK_HPP
#define _OLEDTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"   
                                                                                    //////////////////////////////////////////////////////////////                                                               //TypeMessage:  een struct die meerdere types aan messages
struct TypeMessage{                                                                 //              
    int         IntToWrite;                                                         //- ToWrite:      Bevat de INT voor het OLEDscherm 
    const char* StringToWrite;                                                      //- StringToWrite Bevat de std::string voor het scherm
    char        Type;                                                               //- Type:         Beslist voor welk OLEDschermdeel de tekst bedoel is
};                                                                                  //
                                                                                    //////////////////////////////////////////////////////////////
class DisplayTaak : public rtos::task<>{                                            //- Rtos::task voor de OLED display   
private:                                                                            //- Deze task zal constant wachten op nieuwe data,
                                                                                    //  en het meegegeven data type, waarmee de task zelf    
    rtos::channel<TypeMessage, 10>         inputChannel;                             //  kan beslissen waar op het scherm de data 
                                                                                    //  verwerkt moet worden
public:                                                                             //////////////////////////////////////////////////////////////                
    /// \brief
    /// Default constructor for DisplayTaak
    /// \details    
    /// Constructs display task with a name and a channel for messages to be displayed
    DisplayTaak():                                                                  //- CLASS CONSTRUCTOR
    task                (2, "DisplayTaak"),                                            //- creeer de rtos::task
    inputChannel        (this, "inputChannel")                                      //- creeer de rtos::Channel
    {}                                                                              //
                                                                                    //////////////////////////////////////////////////////////////
    /// \brief
    /// Interface for writing messages to the display
    /// \details    
    /// Public function to write messages to. This function internally uses a channel as waitable to save this incoming data.                                                                                //
    void showMessage(const char* Message, char oledRec);                            //- Write de struct in de channel. Nu kan de task het verwerken
    
    /// \brief
    /// Interface for writing messages to the display
    /// \details    
    /// Public function to write messages to. This function internally uses a channel as waitable to save this incoming data.                                                                                //////////////////////////////////////////////////////////////
    void showMessage(int Message, char oledRec);                                       //////////////////////////////////////////////////////////////
private:
    /// \brief
    /// Main() for the display task.
    /// \details    
    /// The display task inits with alot of display terminals
    /// with the stream operator we can write in these terminals to 
    /// dedicaded sectors of the display. When the tasks has something 
    /// in its channel it writes to one of these terminals based on the given type.                                                                            //    
    void main() override;                                                           //De rtos::task heeft zijn eigen main die constant draait
                                                                                    ////////////////////////////////////////////////////////////////
    
    
};





#endif
