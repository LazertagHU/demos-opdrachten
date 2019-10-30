#ifndef RUN_GAME_TAAK_HPP
#define RUN_GAME_TAAK_HPP

#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"

class Display{
public: 
    void show(char* msg, char x){};
    void show(int msg, char x){};
};

struct weapon{
    const char* name;
    int damage;
    int deathdelay;
};


class Player{
private:
    std::array<weapon, 10> weapons{weapon{"pistol", 5, 2}, weapon{"sniper", 20, 5}, weapon{"rifle", 10, 4}};
public: 
    void setID(int id); 
    int getSome(); 
    void setSome(int x); 
    int getLives(); 
    void setLives(int x); 
    int getWeaponCooldown(); 
    int getPlayerID();
    weapon getWeapon(int n){return weapons[n];}

};
class Transmitter{public: void send(int command);};

enum class buttonid
{
    zeroButton, oneButton, twoButton, threeButton,
    fourButton, fiveButton, sixButton, sevenButton,
    eightButton, nineButton, aButton, bButton,
    cButton, dButton, eButton, starButton,
    hastagButton
};

class RunGameTaak : public rtos::task<> 
{
private:

    enum class state_t 
    {
        IDLE, WAIT_FOR_PLAYER_NUMBER, WAIT_FOR_WEAPON_NUMBER, 
        ENTER_TIME_REMAINING, SEND_COMMAND_STATE,
        START_GAME_TRANSMISSION_STATE, AFTELLEN, RUNGAME, GAME_OVER 
    };

    enum class substates_runGame_t
    {
        ALIVE, WEAPON_COOLDOWN, HIT
    };

    Display&                    display;
    Transmitter&                transmitter;
    rtos::channel<buttonid, 10> inputChannel;
    rtos::flag                  messageFlag;
    rtos::pool<int>             messagepool;
    rtos::pool<Player>          playerpool;
    rtos::clock                 secondClock;
    rtos::timer                 delayTimer;

    void main() override;


    int computeGameTime(int msg);

    int computeCountdown(int msg);

    int computeStartCommand(int countdown);

    int waitForInput();
   
    uint32_t calculateCheckSum(uint32_t input);

    bool isHitMessage(uint32_t message);
    
    int computeHit(uint32_t message);

    int computeDelay(uint32_t message);
    

public:
    RunGameTaak(
        Display & display, 
        Transmitter& transmitter
    ):
        task("runGameTaak"),
        display(display),
        transmitter(transmitter),
        inputChannel(this, "inputChannel"),
        messageFlag(this, "messageFlag"),
        messagepool("messagepool"),
        playerpool("playerpool"),
        secondClock(this, 1'000'000, "secondClock"),
        delayTimer(this, "delayTimer")
    {}

};



#endif