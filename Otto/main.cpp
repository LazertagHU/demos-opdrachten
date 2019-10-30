#include "rtos.hpp"
#include "hwlib.hpp"

class pause_listener {
public:
    virtual void pause_detected( int n ) = 0;
};

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

class msg_listener {
public:
    virtual void sendMessage ( uint32_t m ) = 0;
};

class msg_decoder : public pause_listener, rtos::task<> {
private:
    msg_listener & listener;
    rtos::channel < int, 128 > pauses; 
    rtos::timer reset_timer;

    void main(){
        uint32_t n = 0; 
        uint32_t msg = 0;
        for(;;){
            reset_timer.set(6000);
            auto p = wait(pauses + reset_timer);
            if (p == reset_timer)
            {
                n = 0; msg = 0;
                // hwlib::cout << "reset_timer\n";
                continue;
            }
            else{
                int read = pauses.read();
            
               if (n >= 32)
                {
                    if ((uint16_t)(msg>>16)^((uint16_t)msg))
                    {
                        hwlib::cout << msg << "  " <<((uint16_t)(msg>>16)^((uint16_t)msg)) << "bad message 0\n";
                        n = 0; msg = 0;
                        continue;
                    }
                    if (((msg%32) ^ ((msg>>5)%32)) != ((msg >> 10)%32))
                    {
                        hwlib::cout << msg << "\n";
                        hwlib::cout << ((msg >> 2)%32) << " " << ((msg >> 7)%32) << " " << ((msg >> 12)%32) << "bad message 1\n";
                        n = 0; msg = 0;
                        continue;
                    }
                    if (!(msg >> 31))
                    {
                        hwlib::cout << "bad message 2\n";
                        n = 0; msg = 0;
                        continue;
                    }
                    hwlib::cout << " huh";
                    listener.sendMessage(msg);
                }
                else
                {
                    if(read > 1200){
                        msg = (msg << 1) + 1;
                    }
                    else{
                        msg <<= 1;
                    }
                    n++;
                }
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
        pauses(this, "pauses"),
        reset_timer(this, "reset_timer")
    {}

    void pause_detected( int n ) override { pauses.write( n );}


};

class test_logger : public msg_listener, rtos::task<> {
private:

    rtos::flag msg_recieved;
    rtos::pool <uint32_t> msg_pool;

    void main(){
        for(;;){
            wait(msg_recieved);
            uint32_t msg = msg_pool.read();
            hwlib::cout << "Message recieved: ";
            for (int i = 0; i < 32; i++)
            {
                hwlib::cout << ((msg >> i)%2);
            }
        }
    }

public:
    test_logger( 
        const char * name
    ):
        task( name ),
        msg_recieved(this, "msg_recieved")
    {}

    void sendMessage( uint32_t n ) override { msg_pool.write(n); msg_recieved.set();}
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