#ifndef _TEST_HPP
#define _TEST_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendTask.hpp"

class test : public rtos::task<> {
private:
    hwlib::target::pin_in button;
    SendTask & irsend;
    
public:
    test( const char * name, hwlib::target::pin_in button, SendTask & irsend ):
        task( name ),
        button( button ),
        irsend( irsend )
    {}

    void main() override{
        int msg = 0b1'00100'10101'10001'1'00100'10101'10001;
        for(;;){
            if( !button.read() ){
                irsend.SendMessage( msg );
                hwlib::wait_ms( 2000 );
            }
            button.refresh();
            hwlib::wait_ms( 50 );
        }
    }
};

#endif
