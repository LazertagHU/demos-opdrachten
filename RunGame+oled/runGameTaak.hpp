#ifndef RUN_GAME_TAAK_HPP
#define RUN_GAME_TAAK_HPP

#include "DisplayTaak.hpp"
#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"
#include "weapon.hpp"
#include "PlayerInfo.hpp"
#include "SendTask.hpp"
#include "pause_detector.hpp"
#include "msg_decoder.hpp"

class ButtonListener {
public:
    virtual void buttonPressed() = 0;
}

class Transmitter{public: void send(int command);};

enum class buttonid
{
    zeroButton, oneButton, twoButton, threeButton,
    fourButton, fiveButton, sixButton, sevenButton,
    eightButton, nineButton, aButton, bButton,
    cButton, dButton, eButton, starButton,
    hastagButton
};

class RunGameTaak : public rtos::task<>, msg_listener 
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

    DisplayTaak&                display;
    SendTask&                   transmitter;
    rtos::channel<buttonid, 10> inputChannel;
    rtos::flag                  messageFlag;
    rtos::pool<uint32_t>        messagepool;
    rtos::pool<PlayerInfo>&     playerpool;
    rtos::clock                 secondClock;
    rtos::timer                 delayTimer;

    /*
    * Main function of this task
    * This task controls the game setup and the game whilst it's running
    * 
    * It's code is based on the runGameTaak std and can be found in the
    * google drive map.
    */
    void main() override;

    /*
    * The game leader can send a specific command which states the length
    * of the upcoming game.
    * This function decodes that command and returns the gametime. 
    */
    int computeGameTime(int msg);

    /*
    * The game leader can send a specific command which tells the player 
    * in how many seconds the game is going to start. This way, every
    * player starts at about the same time.
    * 
    * This function decodes that message and translates it in to the proper
    * countdown.
    */
    int computeCountdown(int msg);

    /*
    * The game leader can send a specific command which tells the player 
    * in how many seconds the game is going to start. This way, every
    * player starts at about the same time.
    * 
    * This function calculates the specific command that needs to be send 
    * by the game leader at a specific countdown timer.
    * 
    */
    int computeStartCommand(int countdown);

    /*
    * This function adds two inputs of the keypad together.
    * The user has the option to return when no numbers are entered
    * or when one number is entered. this way the user can return
    * 1 - 9 and then return out of the function.
    */
    int waitForInput();
   
    /*
    * This function calculates the checksum of a specific command.
    * This is done by XOR-ing specific bits
    */
    uint32_t calculateCheckSum(uint32_t input);

    /*
    * This function is used to decide wether te player is hit
    */
    bool isHitMessage(uint32_t message);
    
    /*
    * This function is used to calculate the hit damage when a
    * player is hit
    */
    int computeHit(uint32_t message);

    /*
    * This function is used to calculate the delay that a player
    * can't shoot or be shot after he has been shot by another player
    */
    int computeDeathDelay(uint32_t message);

    int computeShootDelay(uint32_t message);

    /*
    * This function checks if the message received is a start message;
    */
    bool isStartMessage(uint32_t message);

    /*
    * This function checks if the message received contains the game time;
    */
    bool isGameTimeMessage(uint16_t message);
    

public:
    /*
    * Constructor of RunGameTaak
    */
    RunGameTaak(
        DisplayTaak & display, 
        SendTask& transmitter,
        rtos::pool<PlayerInfo> & playerpool
    ):
        task("runGameTaak"),
        display(display),
        transmitter(transmitter),
        inputChannel(this, "inputChannel"),
        messageFlag(this, "messageFlag"),
        messagepool("messagepool"),
        playerpool(playerpool),
        secondClock(this, 1'000'000, "secondClock"),
        delayTimer(this, "delayTimer")
    {}


    void inputMessage(buttonid id);

    void sendMessage(uint32_t m ) override;



};



#endif