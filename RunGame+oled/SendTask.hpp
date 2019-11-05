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

    /// \brief
    /// The main() of the SendTaak.
    /// \details
    /// Recieves messages by SendMessage(). Encodes these messages according to the protocol to send to the IR led.
    void main() override;
    
public:
    /// \brief
    /// Default constructor of the SendTask
    /// \details
    /// Names its task, binds its pin and inits the flag and the pool.
    SendTask( const char * name, hwlib::target::d2_36kHz led, hwlib::target::pin_out laser, int weapondelay ):
        task( 7, name ),
        led( led ),
        laser( laser ),
        weapondelay( weapondelay ),
        SendFlag( this, "SendFlag" ),
        sendpool( "sendpool" )
        {}

    /// \brief
    /// Interface for writing messages.
    /// \details    
    /// Public function to write messages to that need to be send by the IR led. This function internally uses a flag and a pool to save this incoming data. 
    void SendMessage( uint32_t CommandID );
};

#endif