#ifndef _PLAYERINFO_HPP
#define _PLAYERINFO_HPP

#include "weapon.hpp"
#include "hwlib.hpp"
//#include "hit.hpp"

class PlayerInfo {
private:
    unsigned int PlayerID;
    std::array<weapon, 10> weapons{weapon{"pistol", 25, 4'000'000, 1'000'000}, weapon{"sniper", 10, 10'000'000, 50'000}};
    unsigned int currentWeapon;
    //hit hits[10]; // 100 breekt alles, 50 breek het op een andere manier. 10 werkt volledig
    int health;
    int hitAmount;
public:
    PlayerInfo(){
        health = 100;
        hitAmount = 0;
    }

    weapon GetWeapon(int n);
    unsigned int GetCurrentWeapon();
    void SetWeapon(int n);
    unsigned int GetPlayerID();
    void SetPlayerID( unsigned int NewPlayerID );
    //hit* GetHits();
    //void AddHit( int EnemyID, int Damage, int Time );
    void SetHealth( int NewHealth );
    int GetHealth();
    //int GethitAmount();
};

#endif