#include "SendTask.hpp"

// Writes the CommandID to the sendpool and sets the SendFlag
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
            // Waits for the SendFlag sets i to 31 reads the sendpool for the message and shifts the message i times
            // to read the bit at the given position. After this it checks the bits value and sends it to the next state.
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
            
            // Writes 1 to the led and waits 1600us continues to the SENDLOW1 state
            case states::SENDHIGH1:
                led.write(1);
                hwlib::wait_us(1600);
                state = states::SENDLOW1;
                break;
            
            // Writes 0 to the led waits 800us substracts 1 from i shifts the message i times ands the value with 1 and stores it in bit
            // checks if i is the middle of the message ifso it waits for 3000us (2200 + 800) if it was the final bit it goes back to idle after
            // turning off the laser. Checks if bit is a one or zero continues to next state
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
            
            // Writes 1 to the led and waits 800us continues to the SENDLOW0 state
            case states::SENDHIGH0:
                led.write(1);
                hwlib::wait_us(800);
                state = states::SENDLOW0;
                break;
            
            // Writes 0 to the led waits 1600us substracts 1 from i shifts the message i times ands the value with 1 and stores it in bit
            // checks if i is the middle of the message ifso it waits for 3000us (1400 + 1600) if it was the final bit it goes back to idle after
            // turning off the laser. Checks if bit is a one or zero continues to next state
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

                // Writes 1 to the led and waits 800us continues to the SENDLOW0 state
                if( bit == 1 ){
                    state = states::SENDHIGH1;
                }else{
                    state = states::SENDHIGH0;
                }
                break;
        }
    }
}