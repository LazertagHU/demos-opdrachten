#ifndef _TRANSFERHITSCONTROLTAAK_HPP
#define _TRANSFERHITSCONTROLTAAK_HPP

#include "PlayerInfo.hpp"
#include "rtos.hpp"

class TransferHitsControlTaak : public rtos::task<> {
private:
    rtos::flag TransferEnableFlag;
    rtos::pool< PlayerInfo > & entity_pool;
    void main() override;
    enum class states { IDLE };
    states state;
public:
    TransferHitsControlTaak( rtos::pool< PlayerInfo > & entity_pool ):
        task( "TransferHitsControlTaak" ),
        TransferEnableFlag( this, "TransferEnableFlag" ),
	    entity_pool( entity_pool )
    {}

    void writing();
};

#endif
