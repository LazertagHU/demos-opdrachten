#include "runGameTaak.hpp"

int main(){
    // wait for the PC console to start
    hwlib::wait_ms( 500 );

    hwlib::cout << "demo" << hwlib::endl;

    namespace target = hwlib::target;
    
    Display display;
    Transmitter tramsmitter;
    Player player;


    auto runGameTaak = RunGameTaak(display, tramsmitter);

    rtos::run();
}