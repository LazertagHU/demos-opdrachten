#include "PlayerInfo.hpp"


weapon PlayerInfo::GetWeapon(int n){ 
    return weapons[n-1]; 
}

unsigned int PlayerInfo::GetCurrentWeapon(){ 
    return currentWeapon;
}

void PlayerInfo::SetWeapon( int weaponID ){ 
    currentWeapon = weaponID; 
}
    
unsigned int PlayerInfo::GetPlayerID(){ 
    return PlayerID; 
}

void PlayerInfo::SetPlayerID( unsigned int NewPlayerID ){ 
    PlayerID = NewPlayerID; 
}

//hit* PlayerInfo::GetHits(){ 
//    return hits; 
//}

//void PlayerInfo::AddHit( int EnemyID, int Damage, int Time ){
//    hit newHit{ EnemyID, Damage, Time };
//    hits[hitAmount] = newHit;
//    hitAmount++;
//}

void PlayerInfo::SetHealth( int NewHealth ){ 
    health = NewHealth; 
}

int PlayerInfo::GetHealth(){ 
    return health; 
}

//int PlayerInfo::GethitAmount(){ 
//    return hitAmount; 
//}