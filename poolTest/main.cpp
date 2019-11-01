#include "hwlib.hpp"
#include "rtos.hpp"

class playerInfo
{
private:
    bool succes;
    bool var;
public:
    playerInfo()
    : var(true)
    {}

    void setSucces(bool b){
        succes = b;
    }

    void setVar(bool b){
        var = b;
    }

    bool getSucces(){
        return succes;
    }

    bool getVar(){
        return var;
    }
};

class poolTest : public rtos::task<>{
private:
    rtos::pool<playerInfo> & playerInfoPool;

void main() override{

    hwlib::cout << "Startup" << "\n";
    for(;;){ 
        hwlib::cout << "Loop: " << "\n";
        hwlib::wait_ms(1000);
        hwlib::cout << "1 sec" << "\n";
        set(0);
        hwlib::cout << "1: " << getSucces() << "\n";
        hwlib::wait_ms(1000);
        set(1);
        hwlib::cout << "2: " << getSucces() << "\n";
        hwlib::wait_ms(1000);
        hwlib::cout << "3: " << getVar() << "\n";
    }
}

public:
    poolTest(rtos::pool<playerInfo> & playerInfoPool):
    playerInfoPool(playerInfoPool)
    {}

    void set(bool var){
        auto player = playerInfoPool.read();
        player.setSucces(var);
        playerInfoPool.write(player);
    }

    bool getVar(){
        return playerInfoPool.read().getVar();
    }

    bool getSucces(){
        return playerInfoPool.read().getSucces();
    }
};

int main(){
    namespace target = hwlib::target;

    /* constructing player pool */
    auto playerpool         = rtos::pool<playerInfo>("playerpool");
    auto pool = poolTest(playerpool);
    hwlib::wait_ms(500);
    hwlib::cout << "Hello \n";
    /* run rtos */
    rtos::run();
}