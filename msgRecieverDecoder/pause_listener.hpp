#ifndef PAUSE_LISTENER
#define PAUSE_LISTENER

class pause_listener {
public:
    virtual void pause_detected( int n ) = 0;
};

#endif //PAUSE_LISTENER