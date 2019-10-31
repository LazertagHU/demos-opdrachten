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
                PlayerInfo player = entity_pool.read();
                int playerID = player.GetPlayerID();
                auto hits = player.GetHits();
                int hitNumber = player.GethitAmount();
                
                hwlib::cout << "PlayerID: " << playerID << "\n";
                if( hitNumber != 0 ){
                    for( unsigned int i = hitNumber; i > 0; i-- ){
                        hwlib::cout << "Enemy: " << hits[i-1].EnemyID << ", Damage: " << hits[i-1].Damage << ", Time: " << hits[i-1].Time << "\n";
                    }
                }else{
                    hwlib::cout << "No hits registered\n";
                }    
                break;
        }
    }

}
