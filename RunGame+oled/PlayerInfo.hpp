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
    /// \brief
    /// The default constructor for the PlayerInfo entity.
    /// \details
    /// Sets starting amounts of health and hitAmount. During the game this entity saves all player info
    PlayerInfo(){
        health = 100;
        hitAmount = 0;
    }
    
    /// \brief
    /// Returns the n weapon 
    weapon GetWeapon(int n);
    

    /// \brief
    /// Returns the current weapon
    unsigned int GetCurrentWeapon();

    /// \brief
    /// Sets the current weapon to n
    void SetWeapon(int n);

    /// \brief
    /// Returns the current PlayerID.
    unsigned int GetPlayerID();

    /// \brief
    /// Sets the player ID to NewPlayerID
    void SetPlayerID( unsigned int NewPlayerID );

    //hit* GetHits();
    //void AddHit( int EnemyID, int Damage, int Time );

    /// \brief
    /// Set the current health to NewHealt
    void SetHealth( int NewHealth );

    /// \brief
    /// Returns the current healt
    int GetHealth();
    //int GethitAmount();
};

#endif