#ifndef _TESTTAAK_HPP
#define _TESTTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayTaak.hpp"


enum class testState_t          {TESTING};
testState_t         testState       = testState_t::TESTING;

class TestTaak : public rtos::task<>{
private:

             
    rtos::timer             TestTimer;
    std::array<const char*, 5>  TextMessages = {"welkom bij lasertag", "123456789123456\n78983838383838\n38dkjgbdreg\nbrerr3", "Pepe forever!", "excuseerd u mij,", "Maar wat de neuk"};
    int TextMessageIndex                = 0;
    int PlayerNumber                    = 30;
    int AmmoAmound                      = 30;
    int LivesAmound                     = 100;
    int TimeLeft                        = 180;
    DisplayTaak & Display;

public:
    
    TestTaak(DisplayTaak & Display):
        task("TestTaak"),
        TestTimer(this, "TestTimer"),
        Display(Display)
    {}
    
    void main(){
        for(;;){
            
            switch(testState){
                case    testState_t::TESTING:
                    TestTimer.set(1000000);
                    auto evt1 = wait(TestTimer);
                    Display.showMessage(TextMessages[TextMessageIndex], 'M');
                    TextMessageIndex++;
                    if(TextMessageIndex == 5){
                        TextMessageIndex = 0;
                    }
                    TestTimer.set(1000000);
                    auto evt2 = wait(TestTimer);
                    Display.showMessage(PlayerNumber, 'N');
                    PlayerNumber--;
                    TestTimer.set(1000000);
                    auto evt3 = wait(TestTimer);
                    Display.showMessage(AmmoAmound, 'A');
                    AmmoAmound--;
                    TestTimer.set(1000000);
                    auto evt4 = wait(TestTimer);
                    Display.showMessage(LivesAmound, 'H');
                    LivesAmound--;
                    TestTimer.set(1000000);
                    auto evt5 = wait(TestTimer);
                    Display.showMessage(TimeLeft, 'T');
                    TimeLeft--;
                    
                    break;
            }
        }
    }
};






#endif
