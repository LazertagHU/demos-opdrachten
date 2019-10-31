#include "SendTask.hpp"

void SendTask::SendMessage( int CommandID ){
    sendpool.write( CommandID );
    SendFlag.set();
}

void SendTask::main(){
    state = states::IDLE;
    int i;
    uint32_t message;
    bool bit;
    for(;;){
        switch(state){
            case states::IDLE:
                wait( SendFlag );
                i = 31;
                message = sendpool.read();
                bit = (message >> i) & 1;
                laser.write(1);
                if( bit == 1 ){
                    state = states::SENDHIGH1;
                }else{
                    state = states::SENDHIGH0;
                }
                break;
            
            case states::SENDHIGH1:
                led.write(1);
                hwlib::wait_us(1600);
                state = states::SENDLOW1;
                break;
            
            case states::SENDLOW1:
                led.write(0);
                hwlib::wait_us(800);
                i--;
                bit = (message >> i) & 1;
                if( i == 15 ){
                    hwlib::wait_us( 2200 );
                }else if( i < 0 ){
                    led.write(1);
                    hwlib::wait_ms( 1 );
                    led.write(0);
                    hwlib::wait_ms( weapondelay );
                    laser.write(0);
                    state = states::IDLE;
                    break;
                }

                if( bit == 1 ){
                    state = states::SENDHIGH1;
                }else{
                    state = states::SENDHIGH0;
                }
                break;
            
            case states::SENDHIGH0:
                led.write(1);
                hwlib::wait_us(800);
                state = states::SENDLOW0;
                break;
            
            case states::SENDLOW0:
                led.write(0);
                hwlib::wait_us(1600);
                i--;
                bit = (message >> i) & 1;
                if( i == 15 ){
                    hwlib::wait_us( 1400 );
                }else if( i < 0 ){
                    led.write(1);
                    hwlib::wait_ms( 1);
                    led.write(0);
                    hwlib::wait_ms( weapondelay );
                    laser.write(0);
                    state = states::IDLE;
                    break;
                }

                if( bit == 1 ){
                    state = states::SENDHIGH1;
                }else{
                    state = states::SENDHIGH0;
                }
                break;
        }
    }
}