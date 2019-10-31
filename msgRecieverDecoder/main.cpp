#include "rtos.hpp"
#include "hwlib.hpp"
#include "test_logger.hpp"
#include "msg_decoder.hpp"
#include "pause_detector.hpp"

int main(){
    // wait for the PC console to start
    hwlib::wait_ms( 500 );

    hwlib::cout << "ir receive demo\n";

    namespace target = hwlib::target;
     
    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
    
    auto font    = hwlib::font_default_8x8();
    auto display = hwlib::terminal_from( oled, font );
    
    auto ir_sensor   = target::pin_in( target::pins::d8 );   
    auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();

    auto logger             = test_logger("Logger");
    auto msg_decodation     = msg_decoder( "msg_decoder", logger);
    auto pause_detection    = pause_detector("pause_detector", ir_sensor, msg_decodation);
   

    rtos::run();
}