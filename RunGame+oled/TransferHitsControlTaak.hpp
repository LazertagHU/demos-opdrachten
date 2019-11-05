#ifndef _TRANSFERHITSCONTROLTAAK_HPP
#define _TRANSFERHITSCONTROLTAAK_HPP

#include "PlayerInfo.hpp"
#include "rtos.hpp"
#include "hit.hpp"

class TransferHitsControlTaak : public rtos::task<> {
private:
    rtos::flag TransferEnableFlag;
    rtos::pool< PlayerInfo > & entity_pool;
    enum class states { WAIT_FOR_WRITE };
    hit hits[100];
    states state;
    unsigned int hitAmount;

    /// \brief
    /// The main for the TransferHitsControlTaak.
    /// \details
    /// Waits untill the game is finished and writes all the registered hits to the serial port.
    void main() override;
      
public:
    /// \brief
    /// The default constructor of TransferHitsControll
    /// \details
    /// Names its task and inits its flag and pool
    TransferHitsControlTaak( rtos::pool< PlayerInfo > & entity_pool ):
        task( 8, "TransferHitsControlTaak" ),
        TransferEnableFlag( this, "TransferEnableFlag" ),
	    entity_pool( entity_pool ),
        hitAmount(0)
    {}

    /// \brief
    /// Interface for enabling writing to the serialport
    void writing();

    /// \brief
    /// Interface for adding hits.
    /// \details    
    /// Public function to add a hit to the memory.
    void AddHit( int EnemyID, int Damage, int Time );
};

#endif
