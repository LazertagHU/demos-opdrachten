#ifndef _TEST_HPP
#define _TEST_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "TransferHitsControlTaak.hpp"

class test : public rtos::task<> {
private:
    hwlib::target::pin_in button;
    TransferHitsControlTaak & transfer;
    PlayerInfo player;
    rtos::pool< PlayerInfo > & entity_pool;
public:
    test( const char * name, hwlib::target::pin_in button, TransferHitsControlTaak & transfer, PlayerInfo player, rtos::pool< PlayerInfo > & entity_pool ):
        task( name ),
        button( button ),
        transfer( transfer ),
        player( player ),
	    entity_pool( entity_pool )
    {}

    void main() override{
        for(;;){
            if( !button.read() ){
                entity_pool.write( player );
                hwlib::wait_ms( 1000 );
                transfer.writing();
                hwlib::wait_ms( 1000 );
            }
            button.refresh();
            hwlib::wait_ms( 50 );
        }
    }
};

#endif