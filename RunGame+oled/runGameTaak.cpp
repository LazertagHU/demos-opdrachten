#include "runGameTaak.hpp"


void RunGameTaak::main()
{
    state_t                 currentState        = state_t::IDLE;
    substates_runGame_t     currentSubState     = substates_runGame_t::ALIVE;
    buttonid                bnID;
    uint32_t                command;    
    uint32_t                debugCommand        = 0b1'01111'00101'00000; // 0b1'01111'00101'01010
    uint32_t                startCommand        = 0b1'00000'10000'00000'1'00000'10000'00000;
    uint32_t                setTimeCommand      = 0b1'00000'00000'00000'1'00000'00000'00000;
    uint32_t                msg;
    uint32_t                shootCommand        = 15; // dit moet veranderd worden
    bool                    playerWeaponEntered = false;
    bool                    playerIDEntered     = false;
    bool                    gameLeader          = false;
    bool                    gameTimeEntered     = false;
    bool                    transferHitsAllowed = false;
    int                     countdown           = 0;
    int                     remainingGameTime   = 0;
    int                     delay               = 0;

    for(;;)
    {
        switch (currentState)
        {
        case state_t::IDLE:
            display.show("Game Setup", 'M') ;
            auto evt = wait(inputChannel + messageFlag);
            if(evt == inputChannel)
            {
                bnID == inputChannel.read();
                if(bnID == buttonid::cButton && playerWeaponEntered == true && gameLeader == true)
                {
                    display.show("Enter Game Time", 'M');
                    command = 0;
                    currentState = state_t::ENTER_TIME_REMAINING;
                }
                else if(bnID == buttonid::bButton)
                {
                    display.show("Choose weapon", 'M');
                    currentState = state_t::WAIT_FOR_WEAPON_NUMBER;
                }else if(bnID == buttonid::aButton)
                {
                    display.show("Enter a player ID", 'M');
                    currentState = state_t::WAIT_FOR_PLAYER_NUMBER;
                }
                else{
                    // weet niet of deze geprogrammeerd moet worden???
                }
            }
            else
            {
                msg = messagepool.read();
                if(isGameTimeMessage(msg)) // gametime
                {
                    remainingGameTime   = computeGameTime(msg); //----------
                    gameTimeEntered     = true;
                }
                else if (isStartMessage(msg) && gameTimeEntered == true && playerIDEntered == true && playerWeaponEntered == true)
                {
                    display.show("Starting game in", 'M');
                    countdown       = 10 + computeCountdown(msg);
                    currentState    = state_t::AFTELLEN;
                }
            }
            break;

        case state_t::WAIT_FOR_PLAYER_NUMBER:
            int input = waitForInput();
            if(input > 0 && input <= 15)
            {
                auto player = playerpool.read();
                player.SetPlayerID(input);
                playerpool.write(player);
                playerIDEntered == true;
            }
            else
            {
                display.show("invalid player id", 'M');
            }
            currentState = state_t::IDLE;
            break;

        case state_t::WAIT_FOR_WEAPON_NUMBER:
            int input = waitForInput();
            if(input > 0 && input <= 15)
            {
                auto player = playerpool.read();
                player.SetWeapon(input);
                playerpool.write(player);
                playerWeaponEntered == true;
            }
            else
            {
                display.show("invalid weapon id", 'M');
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
                display.show("press * to send start command", 'M');
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
                        display.show(countdown, 'T');
                    }else{
                        display.show("Starting game", 'M');
                        countdown = 10;
                        display.show(countdown, 'T');
                    }
                }
            
            break;

        case state_t::AFTELLEN:
            wait(secondClock);
            if(countdown > 1){
                display.show(--countdown, 'T');
            }else{
                auto player = playerpool.read();
                player.SetHealth(100); // set lives
                playerpool.write(player);
                display.show("Alive", 'M');
                currentState = state_t::RUNGAME;
            }

        case state_t::RUNGAME:
            switch (currentSubState)
            {
            case substates_runGame_t::ALIVE:
                auto evt = wait(messageFlag + secondClock + inputChannel);
                if(evt == messageFlag)
                {
                    msg = messagepool.read();
                    if(isHitMessage(msg))
                    {
                        auto player = playerpool.read();
                        player.SetHealth((player.GetHealth() - computeHit(msg));   // set lives
                        // tmp.addhit(msg) ---------
                        playerpool.write(player);
                        delay = computeDelay(msg);
                        delayTimer.set(delay);                                              /// check return type of computedelay
                        display.show("hit by", 'M');                           // nog dit uitvogelen
                        currentSubState = substates_runGame_t::HIT;

                        if(playerpool.read().GetHealth() < 0)
                        {
                            currentState = state_t::GAME_OVER;
                        }       
                    }
                    else
                    {
                    }
                    
                }
                else if(evt == secondClock)
                {
                    if( remainingGameTime > 0 )
                    {
                        remainingGameTime--;
                        display.show(remainingGameTime, 'T');
                    }
                    else
                    {
                        currentState = state_t::GAME_OVER;
                    }
                }
                else
                {
                    bnID = inputChannel.read();
                    if(bnID == buttonid::eButton)
                    {
                        transmitter.send(shootCommand);
                        delayTimer.set(computeShootDelay()); ///<<----
                        currentSubState = substates_runGame_t::WEAPON_COOLDOWN;
                    }else{
                        // weet niet of dit moet
                    }
                }
                break;
            
            case substates_runGame_t::WEAPON_COOLDOWN:
                auto evt = wait(delayTimer + messageFlag + secondClock);
                if(evt == messageFlag)
                {
                    msg = messagepool.read();
                    if(isHitMessage(msg))
                    {
                        auto player = playerpool.read();
                        player.SetHealth(player.GetHealth() - computeHit(msg));   // set lives
                        playerpool.write(tmp);
                        delay = computeDelay(msg);
                        delayTimer.set(delay);                                              /// check return type of computedelay
                        display.show("hit by", 'M');                           // nog dit uitvogelen
                        currentSubState = substates_runGame_t::HIT;

                        if(playerpool.read().GetHealth() < 0)
                        {
                            currentState = state_t::GAME_OVER;
                        }       
                    }
                    else
                    {
                    }
                    
                }
                else if(evt == secondClock)
                {
                    if( remainingGameTime > 0 )
                    {
                        remainingGameTime--;
                        display.show(remainingGameTime, 'T');
                    }
                    else
                    {
                        currentState = state_t::GAME_OVER;
                    }
                }
                else
                {
                    currentSubState = substates_runGame_t::ALIVE;
                }
                break;

            case substates_runGame_t::HIT:
                auto evt = wait(delayTimer + secondClock);
                if (evt == delayTimer)
                {
                    display.show("Alive", 'M');
                    currentSubState = substates_runGame_t::ALIVE;
                }
                else
                {
                    if( remainingGameTime > 0 )
                    {
                        remainingGameTime--;
                        display.show(remainingGameTime, 'T');
                    }
                    else
                    {
                        currentState = state_t::GAME_OVER;
                    }
                }
                
                break;

            default:
                break;
            }
        case state_t::GAME_OVER:
            display.show("Game over", 'M');
            bnID = inputChannel.read();
            if(bnID == buttonid::eButton){
                transfer.writing();
            }

            break;
        default:
            break;
        }
    }   
};

int RunGameTaak::computeGameTime(int msg)
{
    msg <<= 23;
    msg >>= 28;
    return msg;
};

int RunGameTaak::computeCountdown(int msg)
{
    msg <<= 23;
    msg >>= 28;
    return msg*2;
};

int RunGameTaak::computeStartCommand(int countdown)
{
        /* 
        This function calculates the startcommand that can be send to players. 
        the structure is:

        1-00000-1nnnn-xxxxx

        Where nnnn is the remaining countdown devided by 2. So that means a countdown of 30 will result in 1-00000-11111
        the control bits are calculated afterwards
        */
        auto msg = startCommand;
        countdown/=2;
        msg^= (countdown << 5 );
        msg^= (countdown << 21);
        msg = calculateCheckSum(msg);
        return msg;
};

int RunGameTaak::waitForInput()
{
    char tens;
    char ones;
    int returnval = -1;
    bool loop = true;

    enum class waitForInputStates{AWAIT_FIRST_CHARACTER, AWAIT_SECOND_CHARACTER, END};
    waitForInputStates state = AWAIT_FIRST_CHARACTER;
    
    while (loop)
    {
        switch(state)
        {
            case AWAIT_FIRST_CHARACTER:
                bnID = inputChannel.read();
                if(bnID >= buttonid::zeroButton && bnID <= buttonid::nineButton)
                {
                    tens = bnID;
                    display.show(tens, 'N');
                    state = waitForInputStates::AWAIT_SECOND_CHARACTER;
                }
                else if(bnID == buttonid::starButton)
                {
                    state = waitForInputStates::END;
                }
                break;
            
            case AWAIT_SECOND_CHARACTER:
                bnID = inputChannel.read();
                if(bnID >= buttonid::zeroButton && bnID <= buttonid::nineButton)
                {
                    ones = bnID;
                    display.show((tens-'0')*10 + (ones - '0'), 'N');
                    state = waitForInputStates::END;
                    returnval = (tens-'0')*10 + (ones - '0'));
                }
                else if(bnID == buttonid::starButton)
                {
    
                    returnval = tens - '0';
                    state = waitForInputStates::END;
                }
                break;
            case END:
                loop = false;
                return returnval;
                break;
            default:
                break;
        }
    }
};

uint32_t RunGameTaak::calculateCheckSum(uint32_t input)
{
    /*
    message layout:
    2 * 16 bits message.

    bit : 0           : 1 : 2 : 3 : 4 : 5 : 6 : 7 : 8 : 9 : 10 : 11: 12 : 13 : 14 : 15
    val : startBit    : player ID         : weapon ID          : XOR bits

    bit : 0           : 16 : 17 : 18 : 19 : 20 : 21 : 22 : 23 : 24 : 25 : 26: 27 : 28 : 29 : 30
    val : startBit    : player ID              : weapon ID              : XOR bits

    control bit 11 is the xor of bit 1 and 6;
    control bit 12 is the xor of bit 2 and 7;
    etc...
    */
    
    for(int i = 21; i < 26; i++)
    {
        bool x = input & (1 << i);
        bool y = input & (1 << (i+5));

        input = input | ((x ^ y) << (i-5));
        input = input | ((x ^ y) << (i-21));
    }

    return input;
};

bool RunGameTaak::isHitMessage(uint32_t message)
{
    /* 
    This funciton calculates wether the player has been shot by an enemy.
    If the player shoots himself, the function will return false;
    */
    auto id = playerpool.read().getPlayerID();
    message <<=17;
    message >>=27;
    if(((message) & id) != id ){        // if player id from message does not equal own plater id, it means that the player is shot.
        return true;
    }
    return false;
};

int RunGameTaak::computeHit(uint32_t message)
{
    /* 
    this function calculates the hit damage. 
    it calculates the weapon id the player has been hit by and
    returns the specific damage that is attached to that weapon.
    */
    message <<=22;
    message >> 27; // now contains weapon id;
    
    return player.getWeapon(message).damage;
};

int RunGameTaak::computeDeathDelay(int message)
{
    /* 
    this function calculates the deathdelay. 
    it calculates the weapon id the player has been hit by and
    returns the specific delay that is attached to that weapon.
    */
    message <<=22;
    message >> 27; // now contains weapon id;

    return player.getWeapon(message).deathdelay;
};

int RunGameTaak::computeShootDelay()
{
    /* 
    this function calculates the weaponcooldown. 
    it calculates the weapon id the player has shot with
    returns the specific delay that is attached to that weapon.
    */
    message <<=22;
    message >> 27; // now contains weapon id;

    return player.getWeapon(player.getCurrentWeapon()).weaponCooldown;
};


void RunGameTaak::inputMessage(buttonid id){
    inputChannel.write(id);
};

bool RunGameTaak::isStartMessage(uint32_t message){
    return ((msg >> 9) & 1) && ((msg >> 25) & 1);
};

bool RunGameTaak::isGameTimeMessage(uint32_t message){
    return (((message >> 9) & 1) == 0) && (((message >> 25) & 1) == 0 );
};

bool RunGameTaak::isGameTimeMessage(uint32_t message){
    return ((msg >> 9) & 1 == 0) && ((msg >> 25) & 1 == 0);
};


void RunGameTaak::sendMessage(uint32_t m){
    messagepool.write(m); messageFlag.set();
}

void RunGameTaak::InputMessage(buttonid id){
    inputChannel.write(id);
}