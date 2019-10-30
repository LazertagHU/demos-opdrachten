#ifndef RUN_GAME_TAAK_HPP
#define RUN_GAME_TAAK_HPP

#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"

template<typename T>
struct typeMessage{
    T       toWrite;
    char    type;

    typeMessage(T toWrite, char type):toWrite(toWrite), type(type){}
};


class Display{
public: 
   
   template< typename T>
    void show(typeMessage<T> msg){};
};

class Player{public: int getSome(); void setSome(int x);};
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
    Player&                     player;
    Transmitter&                transmitter;
    rtos::channel<buttonid, 10> inputChannel;
    rtos::flag                  messageFlag;
    rtos::pool<int>             messagepool;
    rtos::pool<Player>          playerpool;
    rtos::clock                 secondClock;
    rtos::timer                 delayTimer;

    void main()
    {
        
        state_t                 currentState        = state_t::IDLE;
        substates_runGame_t     currentSubState     = substates_runGame_t::ALIVE;
        buttonid                bnID;
        uint32_t                command;    
        uint32_t                startCommand;
        uint32_t                msg;
        bool                    playerWeaponEntered = false;
        bool                    playerIDEntered     = false;
        bool                    gameLeader          = false;
        bool                    gameTimeEntered     = false;
        int                     countdown           = 0;
        int                     remainingGameTime   = 0;
        int                     delay               = 0;  


        switch (currentState)
        {
        case state_t::IDLE:
            auto x = typeMessage{"iets", 'M'};
            display.show(typeMessage{"Game Setup", 'M'});
            auto evt = wait(inputChannel + messageFlag);
            if(evt == inputChannel)
            {
                bnID == inputChannel.read();
                if(bnID == buttonid::cButton && playerWeaponEntered == true && gameLeader == true)
                {
                    display.show(typeMessage{"Enter Game Time", 'M'});
                    command = 0;
                    currentState = state_t::ENTER_TIME_REMAINING;
                }
                else if(bnID == buttonid::bButton)
                {
                    display.show(typeMessage{"Choose weapon", 'M'});
                    currentState = state_t::WAIT_FOR_WEAPON_NUMBER;
                }else if(bnID == buttonid::aButton)
                {
                    display.show(typeMessage{"Enter a player ID", 'M'});
                    currentState = state_t::WAIT_FOR_PLAYER_NUMBER;
                }
                else{
                    // weet niet of deze geprogrammeerd moet worden???
                }
            }
            else
            {
                msg = messagepool.read();
                if(msg == 10) // gametime
                {
                    remainingGameTime   = computeGameTime(msg); //----------
                    gameTimeEntered     = true;
                }
                else if (msg == 10 /*startgame*/ && gameTimeEntered == true && playerIDEntered == true && playerWeaponEntered == true)
                {
                    display.show(typeMessage{"Starting game in", 'M'});
                    countdown       = 10 + computeCountdown(msg);
                    currentState    = state_t::AFTELLEN;

                }
            }
            break;

        case state_t::WAIT_FOR_PLAYER_NUMBER:
            int input = waitForInput();
            if(input > 0 && input <= 15)
            {
                auto temp = playerpool.read();
                temp.setSome(input);
                playerpool.write(temp);
                playerIDEntered == true;
            }
            else
            {
                display.show(typeMessage{"invalid player id", 'M'});
            }
            currentState = state_t::IDLE;
            break;

        case state_t::WAIT_FOR_WEAPON_NUMBER:
            int input = waitForInput();
            if(input > 0 && input <= 15)
            {
                auto temp = playerpool.read();
                temp.setSome(input);
                playerpool.write(temp);
                playerWeaponEntered == true;
            }
            else
            {
                display.show(typeMessage{"invalid weapon id", 'M'});
            }
            currentState = state_t::IDLE;
            break;

        case state_t::ENTER_TIME_REMAINING:
            command = 0;
            int input = waitForInput();
            if(input > 0 && input <= 15){
                command = calculateCheckSum(input);
                currentState = state_t::SEND_COMMAND_STATE;
            }
            break;

        case state_t::SEND_COMMAND_STATE:
            bnID = inputChannel.read();
            if(bnID == buttonid::hastagButton){
                transmitter.send(command);
            }else if( bnID == buttonid::starButton){
                countdown = 30;
                display.show(typeMessage{"press * to send start command", 'M'});
                startCommand = computeStartCommand(countdown);
                currentState = state_t::START_GAME_TRANSMISSION_STATE;
            }else{
                    // donno of deze moet
            }
            break;

        case state_t::START_GAME_TRANSMISSION_STATE:
            auto evt = wait(secondClock + inputChannel);
                if(evt == inputChannel){
                    bnID = inputChannel.read();
                    if(bnID == buttonid::starButton){
                        transmitter.send(startCommand);
                    }
                }else if(evt == secondClock){
                    if(countdown > 1){
                        countdown--;
                        startCommand = computeStartCommand(countdown);
                        display.show(typeMessage{countdown, 'T'});
                    }else{
                        display.show(typeMessage{"Starting game", 'M'});
                        countdown = 10;
                        display.show(typeMessage{countdown, 'T'});
                    }
                }
            
            break;

        case state_t::AFTELLEN:
            wait(secondClock);
            if(countdown > 1){
                display.show(typeMessage{--countdown, 'T'});
            }else{
                auto tmp = playerpool.read();
                tmp.setSome(100); // set lives
                playerpool.write(tmp);
                display.show(typeMessage{"Alive", 'M'});
                currentState = state_t::RUNGAME;
            }

        case state_t::RUNGAME:
            switch (currentSubState)
            {
            case substates_runGame_t::ALIVE:
                auto evt = wait(messageFlag + secondClock + inputChannel);
                if(evt == messageFlag){
                    msg = messagepool.read();
                    if(isHitMessage(msg)){
                        auto tmp = playerpool.read();
                        tmp.setSome(tmp.getSome() - computeHit(msg));   // set lives
                        playerpool.write(tmp);
                        delay = computeDelay(msg);
                        delayTimer.set(delay);   /// check return type of computedelay
                        display.show(typeMessage{"hit by", 'M'}); // nog dit uitvogelen
                        currentSubState = substates_runGame_t::HIT;
                    }
                    if(playerpool.read().getSome() < 0){
                        currentState = state_t::GAME_OVER;
                    }
                }else if(evt == secondClock){
                    if( remainingGameTime > 0 ){
                        remainingGameTime--;
                        display.show(typeMessage{remainingGameTime, 'T'});
                    }else{
                        currentState = state_t::GAME_OVER;
                    }
                }else{
                    bnID = inputChannel.read();
                }
                break;
            
            case substates_runGame_t::WEAPON_COOLDOWN:
                break;

            case substates_runGame_t::HIT:
                break;

            default:
                break;
            }

        default:
            break;
        }
    }


    int computeGameTime(int msg){};
    int computeCountdown(int msg){};
    int computeStartCommand(int msg){};
    int waitForInput(){};
    uint8_t calculateCheckSum(int & input){};
    bool isHitMessage(int message){};
    int computeHit(int message){};
    int computeDelay(int message){};




};



#endif