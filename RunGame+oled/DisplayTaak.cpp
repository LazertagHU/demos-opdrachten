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
    auto WweaponT           = hwlib::part(oled, hwlib::xy(38, 5), hwlib::xy(100,14));
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

    
    oled.clear();  hwlib::wait_ms(1);
    oled.flush();  hwlib::wait_ms(1);
    MessageRectangle.draw(Wrectangle);  hwlib::wait_ms(1);
    NameRectangle.draw(Wrectangle);  hwlib::wait_ms(1);
    AmmoRectangle.draw(Wrectangle);  hwlib::wait_ms(1);  
    HealthRectangle.draw(Wrectangle);   hwlib::wait_ms(1);
    TimeRectangle.draw(Wrectangle); hwlib::wait_ms(1);
    oled.flush();  hwlib::wait_ms(1);
    enum class display_state_t        {IDLE};
    display_state_t         state   = display_state_t::IDLE;
 
   
    for(;;){
        switch(state){
            case display_state_t::IDLE:{
                auto Message = inputChannel.read();
                if(Message.Type == 'M'){
                    Wmessage.clear();
                    //hwlib::wait_ms(1);
                    MessageDisplay  << "\t0000"; 
                    //hwlib::wait_ms(1);
                    MessageDisplay <<  Message.StringToWrite;
                    //hwlib::wait_ms(1);
                    MessageDisplay << hwlib::flush;
                }
                else if(Message.Type == 'N'){
                    Wname.clear();
                    NameDisplay << "\t0000" << "#" << Message.IntToWrite <<hwlib::flush;
                }
                else if(Message.Type == 'A'){
                    Wammo.clear();
                    AmmoDisplay << "\t0000"<<  Message.IntToWrite << hwlib::flush;
                }
                else if(Message.Type == 'H'){
                    Whealth.clear();
                    HealthDisplay   << "\t0000"<< "HP: " << Message.IntToWrite << hwlib::flush;
                }
                else if(Message.Type == 'T'){
                    Wtime.clear();
                    TimeDisplay     << "\t0000"<< Message.IntToWrite <<  hwlib::flush;
                }
                else if(Message.Type == 'G'){
                    WweaponT.clear();
                    WeaponTDisplay  << "\t0000"<< Message.StringToWrite << hwlib::flush;
                }
                break;
            }   
            default:
                break;
        }
    }
};
