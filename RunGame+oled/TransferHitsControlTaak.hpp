#ifndef _TRANSFERHITSCONTROLTAAK_HPP
#define _TRANSFERHITSCONTROLTAAK_HPP

#include "PlayerInfo.hpp"
#include "rtos.hpp"
#include "hit.hpp"

class TransferHitsControlTaak : public rtos::task<> {
private:
    rtos::flag TransferEnableFlag;
    rtos::pool< PlayerInfo > & entity_pool;
    enum class states { IDLE };
    hit hits[100];
    states state;
    unsigned int hitAmount;
    void main() override;
      
public:
    TransferHitsControlTaak( rtos::pool< PlayerInfo > & entity_pool ):
        task( 8, "TransferHitsControlTaak" ),
        TransferEnableFlag( this, "TransferEnableFlag" ),
	    entity_pool( entity_pool )
    {
        hitAmount = 0;
    }

    void writing();
    void AddHit( int EnemyID, int Damage, int Time );
};

#endif
