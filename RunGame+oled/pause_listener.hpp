#ifndef PAUSE_LISTENER
#define PAUSE_LISTENER

class pause_listener {
public:
    /// \brief
    /// Purely virtual and abstract class used for the listener patern.
    virtual void pause_detected( int n ) = 0;
};

#endif //PAUSE_LISTENER