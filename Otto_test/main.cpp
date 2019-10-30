#include "rtos.hpp"
#include "hwlib.hpp"
#include <array>


class gen{
private:
    int counter;
    std::array<uint8_t, 32> messageArray;
    uint32_t message;
    uint32_t left;

public:
    gen()
    : counter(0), message(0b1'00100'10101'10001'1'00100'10101'10001), left(0)
    {
        for (int i = 31; i >= 0; i--)
        {
            messageArray[i] = (bool)((message>>i)%2) ? 16 : 8;
            hwlib::cout << messageArray[i] << " ";
        }
        hwlib::cout << "\n";
    }
    bool read(){
        if (counter == 32){
            counter = 0;
        }
        
        if (left <= 0)
        {
            left = messageArray[counter];
            counter++;
            return false;
        }
        left--;
        return true;
               
    };
};


class pause_listener {
public:
    virtual void pause_detected( int n ) = 0;
};

class pause_detector : public rtos::task<> {
private:
    gen generator;
    pause_listener & listener;
    rtos::clock interval_clock;

    void main(){
        int n = 0;
        for(;;){
            wait( interval_clock );
            if(generator.read()){ n+=100;  }
            else { 
                listener.pause_detected( n ); 
                while( !generator.read() ){
                    wait( interval_clock );
                }
                n = 0;
            } 
        }
    }

public:
    pause_detector(
        const char * name,
        gen & generator,
        pause_listener & listener
    ):
        task( name ),
        generator( generator ),
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
            reset_timer.set(4000000);
            auto p = wait(pauses + reset_timer);
            if (p == reset_timer)
            {
                n = 0; msg = 0;
                hwlib::cout << "timer\n"; 
                continue;
            }
            else{
                uint32_t read = pauses.read();
                hwlib::cout << " read: " << read << " n: " << n << "\n";
                n++;
                if (n >= 32)
                {
                    if ((uint16_t)(msg>>16)^((uint16_t)msg))
                    {
                        hwlib::cout << ((uint16_t)(msg>>16)^((uint16_t)msg)) << "bad message 0\n";
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

    auto logger             = test_logger("Logger");
    auto msg_decodation     = msg_decoder("msg_decoder", logger);
    auto generator          = gen();
    auto pause_detection    = pause_detector("pause_detector", generator, msg_decodation);
   

    rtos::run();
}