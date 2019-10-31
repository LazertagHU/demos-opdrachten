#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendTask.hpp"
#include "test.hpp"

int main(){
    auto button = hwlib::target::pin_in( hwlib::target::pins::d8 );
    auto lasersight = hwlib::target::pin_out( hwlib::target::pins::d9 );
    auto led = hwlib::target::d2_36kHz();
    
    auto irsend = SendTask( "InputTask", led, lasersight, 1000 );
    auto Test = test( "test", button, irsend );
    hwlib::cout << "start sender \n";
    hwlib::wait_ms( 500 );

    rtos::run();

}