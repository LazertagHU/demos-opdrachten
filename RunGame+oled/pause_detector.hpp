#ifndef PAUSE_DETECTOR
#define PAUSE_DETECTOR

#include "pause_listener.hpp"
#include "rtos.hpp"
#include "hwlib.hpp"

class pause_detector : public rtos::task<> {
protected:
    hwlib::pin_in&     irsensor;
    pause_listener&    listener;
    rtos::clock        interval_clock;
    hwlib::target::pin_out debugPin = hwlib::target::pin_out( hwlib::target::pins::d7 );

    /// \brief
    /// Main() of the pause_detector task.
    /// \details    
    /// This task samples the detector every 100us and registers the length of a continues pulse. 
    /// The length of these pulses is send to the listener.
    void main() override;

public:
    /// \brief
    /// The default constructor of the pause_detector task.
    /// \details    
    /// Names its task, binds its pin_in, binds its listener and inits its 100us clock.
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

#endif //PAUSE_DETECTOR