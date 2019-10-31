#ifndef _PLAYERINFO_HPP
#define _PLAYERINFO_HPP

#include "weapon.hpp"
#include "hwlib.hpp"
#include "hit.hpp"

class PlayerInfo {
private:
    int PlayerID;
    std::array<weapon, 10> weapons{weapon{"pistol", 5, 2, 1}, weapon{"sniper", 20, 5, 3}, weapon{"rifle", 10, 4, 2}};
    int currentWeapon;
    hit hits[100];
    int health;
    int hitAmount;
public:
    PlayerInfo(){
        health = 100;
        hitAmount = 0;
    }

    weapon GetWeapon(int n);
    int GetCurrentWeapon();
    void SetWeapon(int n);
    int GetPlayerID();
    void SetPlayerID( int NewPlayerID );
    hit* GetHits();
    void AddHit( int EnemyID, int Damage, int Time );
    void SetHealth( int NewHealth );
    int GetHealth();
    int GethitAmount();
};

#endif