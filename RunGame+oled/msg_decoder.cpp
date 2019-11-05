#include "msg_decoder.hpp"

void msg_decoder::main(){
    uint32_t n = 0; 
    uint32_t msg = 0;
    for(;;){
        reset_timer.set(5000);
        auto p = wait(pauses + reset_timer);
        if (p == reset_timer)
        {
            n = 0; msg = 0;
            // hwlib::cout << "reset_timer\n";
            continue;
        }
        else{
            int read = pauses.read();
        
            if (n >= 32)
            {
                if ((uint16_t)(msg>>16)^((uint16_t)msg))
                {
                    hwlib::cout << msg << "  " <<((uint16_t)(msg>>16)^((uint16_t)msg)) << "bad message 0\n";
                    n = 0; msg = 0;
                    continue;
                }
                if (((msg%32) ^ ((msg>>5)%32)) != ((msg >> 10)%32))
                {
                    hwlib::cout << msg << "\n";
                    hwlib::cout << ((msg >> 2)%32) << " " << ((msg >> 7)%32) << " " << ((msg >> 12)%32) << "bad message 1\n";
                    n = 0; msg = 0;
                    continue;
                }
                if (!(msg >> 31))
                {
                    hwlib::cout << "bad message 2\n";
                    n = 0; msg = 0;
                    continue;
                }
                hwlib::cout << " huh";
                listener.sendMessage(msg);
            }
            else
            {
                if(read > 1200){
                    msg = (msg << 1) + 1;
                }
                else{
                    msg <<= 1;
                }
                n++;
            }
        }
    }
};