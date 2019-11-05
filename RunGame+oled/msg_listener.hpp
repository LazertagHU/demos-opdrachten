#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

#include "hwlib.hpp"

class msg_listener {
public:
    /// \brief
    /// Purely virtual and abstract class used for the listener patern.
    virtual void sendMessage ( uint32_t m ) = 0;
};

#endif //MSG_LISTENER_HPP