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
#include "TransferHitsControlTaak.hpp"
#include "InputControlTaak.hpp"

class RunGameTaak : public rtos::task<>, public msg_listener, public InputListener
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
    TransferHitsControlTaak&    transfer;
    InputControlTaak            inputControl;
    rtos::channel<buttonid, 10> inputChannel;
    rtos::flag                  messageFlag;
    rtos::pool<uint32_t>        messagepool;
    rtos::pool<PlayerInfo>&     playerpool;
    rtos::clock                 secondClock;
    rtos::timer                 delayTimer;
    buttonid                    bnID;

    /// \brief
    /// The main() of the RunGame task.
    /// \details
    /// This task controls the game setup and the game whilst it's running
    /// It's code is based on the runGameTaak std and can be found in the
    /// google drive map.

    void main() override;

    /// \brief
    /// Returns the decoded game time.
    /// \details
    /// The game leader can send a specific command which states the length
    /// of the upcoming game.
    /// This function decodes that command and returns the gametime. 
    uint32_t computeGameTime(uint32_t msg);

    /// \brief
    /// Returns the decoded countdown.
    /// \details
    /// The game leader can send a specific command which tells the player 
    /// in how many seconds the game is going to start. This way, every
    /// player starts at about the same time.
    /// 
    /// This function decodes that message and translates it in to the proper
    /// countdown.
    ///
    uint32_t computeCountdown(uint32_t msg);

    /// \brief
    /// Modifies startCommand to the correct start countdown
    /// \details
    /// The game leader can send a specific command which tells the player 
    /// in how many seconds the game is going to start. This way, every
    /// player starts at about the same time.
    /// 
    /// This function calculates the specific command that needs to be send 
    /// by the game leader at a specific countdown timer.
    void computeStartCommand(uint32_t countdown, uint32_t &startCommand);

    /// \brief
    /// Returns pressed chars on keypad.
    /// \details
    /// This function adds two inputs of the keypad together.
    /// The user has the option to return when no numbers are entered
    /// or when one number is entered. this way the user can return
    /// 1 - 9 and then return out of the function.
    ///
    int waitForInput(char place);
   
    /// \brief
    /// Returns XOR checksum
    /// \details
    /// This function calculates the checksum of a specific command.
    /// This is done by XOR-ing specific bits
    uint32_t calculateCheckSum(uint32_t input);

    /// \brief
    /// This function is used to decide wether te player is hit
    bool isHitMessage(uint32_t message);
    
    /// \brief
    /// Returns damage taken.
    /// \details
    /// This function is used to calculate the hit damage when a
    /// player is hit
    int computeHit(uint32_t message);

    /// \brief
    /// Returns after hit timeout.
    /// \details
    /// This function is used to calculate the delay that a player
    /// can't shoot or be shot after he has been shot by another player
    int computeDeathDelay(uint32_t message);

    /// \brief
    /// Returns after shot timeout.
    /// \details
    /// This function is used to calculate the delay that a player
    /// can't shoot after he has shot
    int computeShootDelay();

    /// \brief
    /// This function checks if the message received is a start message;
    bool isStartMessage(uint32_t message);

    /// \brief
    /// This function checks if the message received contains the game time;
    bool isGameTimeMessage(uint32_t message);

    /// \brief
    /// Extracts and returns the enemyID from message.
    uint32_t getEnemyID(uint32_t message);
    

public:
    /// \brief
    /// The default constructor of RunGameTaak
    /// \details
    /// Names its task, binds all given paramaters, inits own objects and starts its 1s clock.
    RunGameTaak(
        DisplayTaak & display, 
        SendTask& transmitter,
        TransferHitsControlTaak& transfer,
        rtos::pool<PlayerInfo> & playerpool
    ):
        task("runGameTaak"),
        display(display),
        transmitter(transmitter),
        transfer(transfer),
        inputControl(this),
        inputChannel(this, "inputChannel"),
        messageFlag(this, "messageFlag"),
        messagepool("messagepool"),
        playerpool(playerpool),
        secondClock(this, 1'000'000, "secondClock"),
        delayTimer(this, "delayTimer")
    {}

    /// \brief
    /// Interface for writing recieved messages
    /// \details    
    /// Public function to write the recieved message to. This function internally uses a pool and a flag to save this incoming data.  
    void sendMessage(uint32_t m ) override;

    /// \brief
    /// Interface for writing recieved commands
    /// \details    
    /// Public function to write commands to. This function internally uses a channel as waitable to save this incoming data.  
    void InputMessage(buttonid id)override;

};



#endif