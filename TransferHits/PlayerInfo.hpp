#ifndef _PLAYERINFO_HPP
#define _PLAYERINFO_HPP

#include "weapon.hpp"
#include "hit.hpp"

class PlayerInfo {
private:
    int PlayerID;
    weapon currentWeapon;
    hit hits[100];
    int health;
    int hitAmount;
public:
    PlayerInfo(){
        health = 100;
        hitAmount = 0;
    }

    weapon GetWeapon();
    void SetWeapon( weapon NewWeapon );
    int GetPlayerID();
    void SetPlayerID( int NewPlayerID );
    hit* GetHits();
    void AddHit( int EnemyID, int Damage, int Time );
    void SetHealth( int NewHealth );
    int GetHealth();
    int GethitAmount();
};

#endif