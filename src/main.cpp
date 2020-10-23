############################
#EM FASE DE DESENVOLVIMENTO#
############################

#include "mbed.h"

InterruptIn button(p5);
DigitalOut red_led(p6);
DigitalOut yellow_led(p7);
DigitalOut blue_led(p8);

Timeout timeout;
// Timeout buttonTimeout;
Ticker ticker;

enum STATE
{
  OFF, ALERT, RED, YELLOW , BLUE, ON
};

int current_state;
int previe_state;
int manual_count;
int stateButton;

void transitionState();
void defaultState();

void changeRed()
{
    printf("red");
    red_led = 1;
    previe_state = current_state;
    current_state = RED;
    timeout.attach(&transitionState, 10);
}

void changeYellow()
{
    printf("yellow");
    yellow_led = 1;
    previe_state = current_state;
    current_state = YELLOW;
    timeout.attach(&transitionState, 20);
}

void changeBlue()
{
    printf("blue");
    blue_led = 1;
    previe_state = current_state;
    current_state = BLUE;
    timeout.attach(&transitionState, 4);
}

void changeAlert(){
    printf("alert");
    yellow_led = !yellow_led;
    ticker.attach(changeAlert, 0.5);
}

void changeOff(){
    printf("off");
    defaultState();
}

void initialState(){
    red_led = 1;
    previe_state = RED;
    current_state = RED;
    timeout.attach(&transitionState, 10);
}

void manual_count_fun(){
    manual_count++;
}

void startCount()
{
    printf("Start count\n");
    timeout.detach();
    manual_count = 0;
    ticker.attach(manual_count_fun,1);
}

void endCount()
{
    printf("End count\n");
    ticker.detach();
    stateButton = ON;
    transitionState();
}

int main() {
    initialState();
    transitionState();
    
    button.rise(&startCount);
    button.fall(&endCount);
    
    while(1) {
        wait(1);
        printf("A %d\n",manual_count);
    }
}

void transitionState(){
    defaultState();
    if(stateButton == OFF){
        if(current_state == RED){
            changeYellow();
        }
        else if(current_state == YELLOW){
            changeBlue();
        }
        else{//BLUE
            changeRed();
        }
    }
    else{
        stateButton = OFF;
        if(current_state == RED){
            if(manual_count>=3 && manual_count<=10){
                changeAlert();
            }
            else if(manual_count > 10){
                changeOff();
            }
        }
        else if(current_state == YELLOW){
            if(manual_count>=3 && manual_count<=10){
                changeAlert();
            }
            else if(manual_count > 10){
                changeOff();
            }
        }
        else if(current_state == BLUE){
            if(manual_count>=3 && manual_count<=10){
                changeAlert();
            }
            else if(manual_count==20 || manual_count<=3){
                changeYellow();
            }
            else if(manual_count>10){
                changeOff();
            }
        }
        else if(current_state == ALERT){
            if(manual_count>=3 && manual_count<=10){
                if(previe_state == RED){
                    changeRed();
                }
                else if(previe_state == YELLOW){
                    changeYellow();
                }
                else{
                    changeBlue();
                }
            }
            else if(manual_count > 10){
                changeOff();
            }
        }
        else{//OFF
            if(manual_count > 10){
                changeRed();
            }
        }
    }
}

void defaultState()
{
  red_led = 0;
  yellow_led = 0;
  blue_led = 0;
  
  timeout.detach();
  ticker.detach();
}
