#include "runGameTaak.hpp"


int main(){
    /* constructing pins */
    auto lasersight     = hwlib::target::pin_out( hwlib::target::pins::d9 );
    auto led            = hwlib::target::d2_36kHz();
    
    /* constructing player pool */
    auto playerpool     = rtos::pool<Player>("playerpool");
    
    /* constructing tasks */
    auto display        = DisplayTaak();
    auto transmitter    = SendTask( "InputTask", led, lasersight, 500 );
    auto runGame        = RunGameTaak(display, transmitter, playerpool);   

    auto decoder        = msg_decoder("decoder", runGame);

    /* run rtos */
    rtos::run();
}