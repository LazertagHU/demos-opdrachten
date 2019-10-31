#include "runGameTaak.hpp"


int main(){


    auto display = DisplayTaak();
    auto transmitter = Transmitter();

    auto playerpool = rtos::pool<Player>("playerpool");

    auto runGame = RunGameTaak(display, transmitter, playerpool);   

    rtos::run();
}