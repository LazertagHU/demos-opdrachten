#include "rtos.hpp"
#include "../rtos/rtos.hpp"
#include "hwlib.hpp"
#include "../hwlib/library/hwlib.hpp"

class pause_listener {
public:
    virtual void pause_detected( int n ) = 0;
};

class pause_detector : public rtos::task<> {
private:
    
    hwlib::pin_in & irsensor;
    long long int interval;
    pause_listener & listener;
    rtos::clock interval_clock;

    void main(){
        int n = 0;
        for(;;){
            wait( interval_clock );
            if(irsensor.read()){ n+=200;  }
            else { 
                listener.pause_detected( n ); 
                while( !irsensor.read() ){
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
        long long int interval,
        pause_listener & listener
    ):
        task( name ),
        irsensor( irsensor ),
        interval( interval ),
        listener(listener),
        interval_clock( this, 200, "interval_timer")
    {}
};

class msg_listener {
public:
    virtual void msg_received ( int m ) = 0;
};

class msg_decoder : public pause_listener, rtos::task<> {
private:

    msg_listener & listener;
    rtos::channel < int, 1024 > pauses;

    void main(){
        uint32_t n; 
        uint32_t m; 
        for(;;){
            auto p = pauses.read();
            //hwlib::cout << p << hwlib::endl;
            if( p > 4000 && p < 5000){
                n = 0;
                m = 0;
        
                while(n != 32){
                    wait(pauses);
                    auto p = pauses.read();
                    if( p >= 200 && p < 2000){
                        n++;
                        m = m << 1;
                        m |= (( p > 1000) ? 1 : 0);
                    }else{
                        // hwlib::cout << p << '\n';
                        // pauses.clear();
                        hwlib::cout << "error";
                        break;
                    }
                }
                if(n==32){listener.msg_received( m );}
            }
        }
    }
public:
    msg_decoder( 
        const char * name,
        msg_listener & listener
    ):
        task( name ),
        listener(listener),
        pauses(this, "pauses")
    {}

    void pause_detected( int n ) override { pauses.write( n );}
};



class msg_logger : public msg_listener, rtos::task<> {
private:
    // eigenlijk een struct
    long long int msg_duration;
    hwlib::terminal_from & display;
    rtos::timer timer;
    rtos::channel < int, 1028> messages;

    void main(){
        for(;;){
            display << '\f' << hwlib::flush;
            while(true){
                auto m = messages.read();
                show(m);
                timer.set(msg_duration);
                auto waitable = wait( timer + messages );
                if(waitable == timer)break;    
            }
        }
    }

    void show( int m ){
        display << '\f';
        for(int i = 32; i > 0; i--){
            if((m >> (i-1)) & 1){ display << '1';}
            else{ display << '0';}
            if(i == 17+8){ display << '\n';}
            else if(i == 17){display << '\n';}
            else if(i == 9){ display << '\n';}
        }
        display << hwlib::flush;
    }

public:

    msg_logger(
        const char* name,
        long long int msg_duration,
        hwlib::terminal_from & display
    ):
        task( name ),
        msg_duration(msg_duration),
        display(display),
        timer( this, "timer"),
        messages( this, "messages")
    {}

    void msg_received ( int m ) override { messages.write( m );} 
};

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
    
    auto ir_sensor = target::pin_in( target::pins::d8 );   
    auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();


    auto msg_log            = msg_logger("msg_logger", 5'000'000, display);
    auto msg_decodation     = msg_decoder( "msg_decoder", msg_log);
    auto pause_detection    = pause_detector("pause_detector", ir_sensor, 100LL, msg_decodation);
   


    rtos::run();
}