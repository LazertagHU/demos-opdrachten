#include "PlayerInfo.hpp"
#include "hwlib.hpp"

weapon PlayerInfo::GetWeapon(){ return currentWeapon; }

void PlayerInfo::SetWeapon( weapon NewWeapon ){ currentWeapon = NewWeapon; }
    
int PlayerInfo::GetPlayerID(){ return PlayerID; }

void PlayerInfo::SetPlayerID( int NewPlayerID ){ PlayerID = NewPlayerID; }

hit* PlayerInfo::GetHits(){ return hits; }

void PlayerInfo::AddHit( int EnemyID, int Damage, int Time ){
    hit newHit{ EnemyID, Damage, Time };
    hits[hitAmount] = newHit;
    hitAmount++;
}

void PlayerInfo::SetHealth( int NewHealth ){ health = NewHealth; }

int PlayerInfo::GetHealth(){ return health; }

int PlayerInfo::GethitAmount(){ return hitAmount; }