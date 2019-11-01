#include "runGameTaak.hpp"


int main(){
    namespace target = hwlib::target;

    /* constructing pins */
    auto lasersight         = hwlib::target::pin_out( hwlib::target::pins::d1 );
    auto led                = hwlib::target::d2_36kHz();
    auto ir_sensor          = target::pin_in( target::pins::d8 );   
    auto tsop_gnd           = target::pin_out( target::pins::d9 );
    auto tsop_vdd           = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();
    
    /* constructing player pool */
    auto playerpool         = rtos::pool<PlayerInfo>("playerpool");
    
    /* constructing tasks */
    auto display            = DisplayTaak();
    auto transfer           = TransferHitsControlTaak(playerpool);
    auto transmitter        = SendTask( "InputTask", led, lasersight, 500 );
    auto runGame            = RunGameTaak(display, transmitter, transfer, playerpool);   
    // auto decoder            = msg_decoder("decoder", runGame);
    // auto pause_detection    = pause_detector("pause_detector", ir_sensor, decoder);
    // auto input              = InputControlTaak(runGame);

    /* run rtos */
    rtos::run();
}