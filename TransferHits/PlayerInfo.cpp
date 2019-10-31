#include "PlayerInfo.hpp"
#include "hwlib.hpp"

// Returns the current weapon assigned to the player
weapon PlayerInfo::GetWeapon(){ return currentWeapon; }

// Assigns a weapon to the player
void PlayerInfo::SetWeapon( weapon NewWeapon ){ currentWeapon = NewWeapon; }

// Returns the PlayerID
int PlayerInfo::GetPlayerID(){ return PlayerID; }

// Sets the PlayerID to a given int
void PlayerInfo::SetPlayerID( int NewPlayerID ){ PlayerID = NewPlayerID; }

// Returns the entire list of all the hits received
hit* PlayerInfo::GetHits(){ return hits; }

// Adds a hit to the list of hits
void PlayerInfo::AddHit( int EnemyID, int Damage, int Time ){
    hit newHit{ EnemyID, Damage, Time };
    hits[hitAmount] = newHit;
    hitAmount++;
}

// Sets the players health to a given int NewHealth
void PlayerInfo::SetHealth( int NewHealth ){ health = NewHealth; }

// Returns the current Player health
int PlayerInfo::GetHealth(){ return health; }

// Returns the amount of times the player has been hit
int PlayerInfo::GethitAmount(){ return hitAmount; }