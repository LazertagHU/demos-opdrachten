#ifndef TEST_LOGGER_HPP
#define TEST_LOGGER_HPP

#include "msg_listener.hpp"
#include "rtos.hpp"
#include "hwlib.hpp"

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

#endif // TEST_LOGGER_HPP