#ifndef PAUSE_DETECTOR
#define PAUSE_DETECTOR

#include "pause_listener.hpp"
#include "rtos.hpp"
#include "hwlib.hpp"

class pause_detector : public rtos::task<> {
private:
    hwlib::pin_in & irsensor;
    pause_listener & listener;
    rtos::clock interval_clock;
    hwlib::target::pin_out debugPin = hwlib::target::pin_out( hwlib::target::pins::d7 );

    void main(){
        int n = 0;
        for(;;){
            wait( interval_clock );
            if(!irsensor.read()){ n+=100;  }
            else { 
                if (n > 1200)
                {
                    debugPin.write(1);
                    debugPin.flush();
                }
                else
                {
                    debugPin.write(0);
                    debugPin.flush();
                }
                
                listener.pause_detected( n ); 
                while( irsensor.read() ){
                    wait( interval_clock );
                }
                n = 0;
            } 
        }
    }

public:
    pause_detector(
        const char * name,
        hwlib::pin_in & irsensor,
        pause_listener & listener
    ):
        task( name ),
        irsensor( irsensor ),
        listener(listener),
        interval_clock( this, 100, "interval_clock")
    {}
};

#endif //PAUSE_DETECTOR