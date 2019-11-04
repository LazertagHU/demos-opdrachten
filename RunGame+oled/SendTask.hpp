#ifndef _SENDTASK_HPP
#define _SENDTASK_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"
#include "../../hwlib/library/hwlib.hpp"

class SendTask : public rtos::task<> {
private:
    enum class states { IDLE, SENDHIGH1, SENDHIGH0, SENDLOW1, SENDLOW0 };
    states state;
    hwlib::target::d2_36kHz led;
    hwlib::target::pin_out laser;
    int weapondelay;
    rtos::flag SendFlag;
    rtos::pool<uint32_t> sendpool;
    rtos::mutex send_mutex;
    void main() override;
    
public:
    SendTask( const char * name, hwlib::target::d2_36kHz led, hwlib::target::pin_out laser, int weapondelay ):
        task( name ),
        led( led ),
        laser( laser ),
        weapondelay( weapondelay ),
        SendFlag( this, "SendFlag" ),
        sendpool( "sendpool" )
        {}

    void SendMessage( uint32_t CommandID );
};

#endif