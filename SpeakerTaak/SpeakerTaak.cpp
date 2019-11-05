#include "SpeakerTaak.hpp"

enum class SpeakerState_t               {WAIT_FOR_INPUT, SHOOTING, HIT};

struct noot{
    int                Frequentie;
    int                duration;
};

void SpeakerTaak::main(){
    SpeakerState_t SpeakerState         = SpeakerState_t::WAIT_FOR_INPUT;
    auto            lsp                 = hwlib::target::pin_out( hwlib::target::pins::d7 );
    int             half_period;
    unsigned int    end;
    unsigned int    TempFreq;
    std::array<noot, 2> Shooting        { noot{200, 60000}, noot{1500, 225000} };
    std::array<noot, 2> Hit             { noot{200, 100000}, noot{130, 600000} };
    
    for(;;){
        switch(SpeakerState){
            
            case SpeakerState_t::WAIT_FOR_INPUT:{
                auto evt1           = wait(ShootFlag + HitFlag);
                if(evt1 == ShootFlag){
                    SpeakerState    = SpeakerState_t::SHOOTING;
                    break;
                    }
                else if(evt1 == HitFlag){
                    SpeakerState    = SpeakerState_t::HIT;
                    }
                break;
            }
            
            case SpeakerState_t::SHOOTING:{
                TempFreq                = Shooting[0].Frequentie;
                end                     = hwlib::now_us() + Shooting[0].duration;
                half_period             = ( 1'000'000 / (2 * TempFreq) );
                
                while (end > hwlib::now_us()){
                    lsp.write(1);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                    lsp.write(0);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                }
                
                TempFreq                = Shooting[1].Frequentie;
                end                     = hwlib::now_us() + Shooting[1].duration;
                half_period             = (1'000'000 / (2 * TempFreq));
                while (end > hwlib::now_us()){
                    lsp.write(1);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                    lsp.write(0);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                    if(TempFreq >= 400){
                        TempFreq       -= 4;
                        half_period     = (1'000'000 / (2 * TempFreq));
                    }
                    else if(TempFreq < 400 && TempFreq >=50){
                        TempFreq       -= 2;
                        half_period     = (1'0000'000 / (2 * TempFreq));
                    }
                        
                }
                SpeakerState            = SpeakerState_t::WAIT_FOR_INPUT;
                break;
            }
            
            
            case SpeakerState_t::HIT:{
                TempFreq                = Hit[0].Frequentie;
                end                     = hwlib::now_us() + Hit[0].duration;
                half_period             = (1'000'000 / (2 * TempFreq));
                
                while (end > hwlib::now_us()){
                    lsp.write(1);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                    lsp.write(0);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                }
                
                TempFreq                = Hit[1].Frequentie;
                end                     = hwlib::now_us() + Hit[1].duration;
                half_period             = (1'000'000 / (2 * TempFreq));
                while (end > hwlib::now_us()){
                    lsp.write(1);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                    lsp.write(0);
                    lsp.flush();
                    hwlib::wait_us(half_period);
                }
                SpeakerState            = SpeakerState_t::WAIT_FOR_INPUT;
                break;
                
                
            }
            
                
        }   
    }
};