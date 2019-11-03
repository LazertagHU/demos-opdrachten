#include "TransferHitsControlTaak.hpp"
#include "hwlib.hpp"
#include "PlayerInfo.hpp"


void TransferHitsControlTaak::writing(){
    TransferEnableFlag.set();
}

void TransferHitsControlTaak::main(){
    state = states::IDLE;
    for(;;){
        switch( state ){
            case states::IDLE:
                wait( TransferEnableFlag );
            // break;
                PlayerInfo player = entity_pool.read();
                int playerID = player.GetPlayerID();
                
                hwlib::cout << "PlayerID: " << playerID << "\n";
                if( hitAmount != 0 ){
                    for( unsigned int i = hitAmount; i > 0; i-- ){
                        hwlib::cout << "Enemy: " << hits[i-1].EnemyID << ", Damage: " << hits[i-1].Damage << ", Time: " << hits[i-1].Time << "\n";
                    }
                }else{
                    hwlib::cout << "No hits registered\n";
                }    
                break;
        }
    }

}

void TransferHitsControlTaak::AddHit( int EnemyID, int Damage, int Time ){
    hit newHit{ EnemyID, Damage, Time };
    if( hitAmount < 100 ){
        hits[hitAmount] = newHit;
        hitAmount++;
    }
}
