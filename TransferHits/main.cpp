#include "hwlib.hpp"
#include "PlayerInfo.hpp"
#include "hit.hpp"
#include "weapon.hpp"
#include "rtos.hpp"
#include "TransferHitsControlTaak.hpp"
#include "test.hpp"

int main(){
    hwlib::wait_ms( 500 );
    auto player = PlayerInfo();

    player.SetPlayerID( 10 );
    player.SetHealth( 30 );
    player.AddHit( 3, 20, 130 );
    player.AddHit( 6, 90, 40 );
    player.AddHit( 5, 10 , 12 );
    player.AddHit( 2, 4, 10 );
    player.AddHit( 1, 4, 10 );
    player.AddHit( 8, 4, 10 );
    player.AddHit( 12, 4, 10 );
    player.AddHit( 9, 4, 10 );
    weapon newWep = { "Sniper", 30 };
    player.SetWeapon(newWep);

    auto button = hwlib::target::pin_in( hwlib::target::pins::d8 );
    rtos::pool< PlayerInfo > entity_pool( "entity_pool" );
    auto transfer = TransferHitsControlTaak( entity_pool );
    auto testing = test( "test", button, transfer, player, entity_pool );

    rtos::run();
}