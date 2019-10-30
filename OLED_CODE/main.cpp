//#include "hwlib.hpp"
#include "DisplayTaak.hpp"
#include "TESTTaak.hpp"
#include "rtos.hpp"

  
    
int main(void){
    auto D1     = DisplayTaak();
    auto T1     = TestTaak(D1);
  
    rtos::run();
    
}




