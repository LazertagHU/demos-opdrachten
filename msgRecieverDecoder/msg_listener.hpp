#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

class msg_listener {
public:
    virtual void sendMessage ( uint32_t m ) = 0;
};

#endif //MSG_LISTENER_HPP