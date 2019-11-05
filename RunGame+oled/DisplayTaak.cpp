#include "DisplayTaak.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>


void DisplayTaak::main(){
    auto scl                = hwlib::target::pin_oc( hwlib::target::pins::scl);
    auto sda                = hwlib::target::pin_oc( hwlib::target::pins::sda);
    auto i2c_bus            = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda);
    auto oled               = hwlib::glcd_oled(i2c_bus, 0x3c);
    
    auto Wmessage           = hwlib::part(oled, hwlib::xy(3, 20), hwlib::xy(123,25));
    auto Wname              = hwlib::part(oled, hwlib::xy(3, 5), hwlib::xy(32,10));
    auto WweaponT           = hwlib::part(oled, hwlib::xy(38, 5), hwlib::xy(61,9));
    auto Wammo              = hwlib::part(oled, hwlib::xy(100, 5), hwlib::xy(26,9));
    auto Whealth            = hwlib::part(oled, hwlib::xy(3, 53), hwlib::xy(60, 10));
    auto Wtime              = hwlib::part(oled, hwlib::xy(67, 53), hwlib::xy(60,10));
    auto Wrectangle         = hwlib::part(oled, hwlib::xy(0, 0), hwlib::xy(128,64));
    
 
    auto Fmessage           = hwlib::font_default_8x8();
    auto Fname              = hwlib::font_default_8x8();
    auto Fammo              = hwlib::font_default_8x8();
    auto Fhealth            = hwlib::font_default_8x8();
    auto Ftime              = hwlib::font_default_8x8();
    
    auto MessageDisplay     = hwlib::terminal_from(Wmessage, Fmessage);
    auto NameDisplay        = hwlib::terminal_from(Wname, Fname);
    auto AmmoDisplay        = hwlib::terminal_from(Wammo, Fammo);
    auto HealthDisplay      = hwlib::terminal_from(Whealth, Fhealth);
    auto TimeDisplay        = hwlib::terminal_from(Wtime, Ftime);
    auto WeaponTDisplay     = hwlib::terminal_from(WweaponT, Fammo);
    
                                                                                    //////////////////////////////////////////////////////////////
    hwlib::rectangle MessageRectangle(hwlib::xy(0, 15), hwlib::xy(127,49));                                   //Maak alle rectangles aan
    hwlib::rectangle NameRectangle(hwlib::xy(0, 0), hwlib::xy(35, 15));                                      //
    hwlib::rectangle AmmoRectangle(hwlib::xy(35, 0), hwlib::xy(127, 15));                                     //
    hwlib::rectangle HealthRectangle(hwlib::xy(0, 49), hwlib::xy(64,63));                                    //
    hwlib::rectangle TimeRectangle(hwlib::xy(64, 49), hwlib::xy(127,63));                                      //

    
    oled.clear(); 
    //oled.flush();
    MessageRectangle.draw(Wrectangle);  
    NameRectangle.draw(Wrectangle);  
    AmmoRectangle.draw(Wrectangle);   
    HealthRectangle.draw(Wrectangle);  
    TimeRectangle.draw(Wrectangle); 
    oled.flush();
    enum class display_state_t        {IDLE};
    display_state_t         state   = display_state_t::IDLE;
 
   
    for(;;){
        switch(state){
            case display_state_t::IDLE:{
                auto Message = inputChannel.read();
            // break;
                if(Message.Type == 'M'){
                   
                    hwlib::wait_us(10); Wmessage.clear();
                    hwlib::wait_us(10); MessageDisplay  << "\t0000"; 
                    hwlib::wait_us(10);MessageDisplay <<  Message.StringToWrite;
                    hwlib::wait_us(10); MessageDisplay << hwlib::flush;
                }
                else if(Message.Type == 'N'){
                    hwlib::wait_us(10); Wname.clear();
                    hwlib::wait_us(10); NameDisplay << "\t0000";
                    hwlib::wait_us(10); NameDisplay << "#";
                    hwlib::wait_us(10); NameDisplay << Message.IntToWrite;
                    hwlib::wait_us(10); NameDisplay <<hwlib::flush;
                }
                else if(Message.Type == 'A'){
                    hwlib::wait_us(10); Wammo.clear();
                    hwlib::wait_us(10); AmmoDisplay << "\t0000";
                    hwlib::wait_us(10); AmmoDisplay << Message.IntToWrite;
                    hwlib::wait_us(10); AmmoDisplay << hwlib::flush;
                }
                else if(Message.Type == 'H'){
                    hwlib::wait_us(10); Whealth.clear();
                    hwlib::wait_us(10); HealthDisplay   << "\t0000";
                    hwlib::wait_us(10); HealthDisplay << "HP: ";
                    hwlib::wait_us(10); HealthDisplay << Message.IntToWrite;
                    hwlib::wait_us(10); HealthDisplay << hwlib::flush;
                }
                else if(Message.Type == 'T'){
                    hwlib::wait_us(10); Wtime.clear();
                    hwlib::wait_us(10); TimeDisplay     << "\t0000";
                    hwlib::wait_us(10); TimeDisplay << Message.IntToWrite;
                    hwlib::wait_us(10); TimeDisplay <<  hwlib::flush;
                }
                else if(Message.Type == 'G'){
                    hwlib::wait_us(10); WweaponT.clear();
                    hwlib::wait_us(10); WeaponTDisplay << "\t0000"; 
                    hwlib::wait_us(10); WeaponTDisplay << Message.StringToWrite; 
                    hwlib::wait_us(10); WeaponTDisplay << hwlib::flush;
                }
                break;
            }   
            default:
                break;
        }
    }
};


void DisplayTaak::showMessage(const char* Message, char oledRec){  
                            //- Public functie voor het verwerken van een dataType String
    TypeMessage sendMessage;                                                  //- creeer een TypeMessage struct object voor de Channel
    sendMessage.StringToWrite   = Message;                                      //- Vul de struct met de Message en het dataType
    sendMessage.Type            = oledRec;                                      //
    inputChannel.write(sendMessage); 
}          

void DisplayTaak::showMessage(int Message, char oledRec){                                    //- Public functie voor het verwerken van een dataType Int
    TypeMessage sendMessage;                                                    //- Creeer een TypeMessage struct object voor de Channel
    sendMessage.IntToWrite   = Message;                                         //- Vul de struct met de Message en het dataType    
    sendMessage.Type            = oledRec;                                      //    
    inputChannel.write(sendMessage);                                            //- Write de struct in de channel. Nu kan de task het verwerken                         
}           