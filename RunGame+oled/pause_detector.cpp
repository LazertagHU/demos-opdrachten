#include "pause_detector.hpp"

void pause_detector::main(){
    int n = 0;
    for(;;){
        wait( interval_clock );
        if(!irsensor.read()){ n+=100; debugPin.write(1);debugPin.flush(); }
        else { 
            listener.pause_detected( n ); 
            while( irsensor.read() ){
                debugPin.write(0);debugPin.flush();
                wait( interval_clock );
            }
            n = 0;
        } 
    }
}