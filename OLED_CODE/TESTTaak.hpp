#ifndef _TESTTAAK_HPP
#define _TESTTAAK_HPP
#include <array>
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"
#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "DisplayTaak.hpp"


enum class testState_t          {TESTING, COUNTDOWN};


class TestTaak : public rtos::task<>{
private:

             
    rtos::timer             TestTimer;
    rtos::clock             secondClock;
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
        secondClock(this, 1'000'000, "secondClock"),
        Display(Display)
    {}
private:
    void main(){
        testState_t         testState       = testState_t::TESTING;

        for(;;){
            switch(testState){
                case    testState_t::TESTING:
                    wait(secondClock);
                    Display.showMessage(TextMessages[TextMessageIndex], 'M');
                    TextMessageIndex++;
                    if(TextMessageIndex == 5){
                        TextMessageIndex = 0;
                    }

                    wait(secondClock);
                    Display.showMessage(PlayerNumber, 'N');
                    PlayerNumber--;
                   
                    wait(secondClock);
                    Display.showMessage(AmmoAmound, 'A');
                    AmmoAmound--;
                    
                    wait(secondClock);
                    Display.showMessage(LivesAmound, 'H');
                    LivesAmound--;
                   
                    wait(secondClock);
                    Display.showMessage(TimeLeft, 'T');
                    TimeLeft--;
                    testState = testState_t::COUNTDOWN;
                    break;

                case testState_t::COUNTDOWN:
                    wait(secondClock);
                    Display.showMessage(--TimeLeft, 'T');
                    break;
            }
        }
    }
};






#endif
