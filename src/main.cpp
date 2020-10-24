#include "mbed.h"

InterruptIn button(p5);
DigitalOut red_led(p6);
DigitalOut yellow_led(p7);
DigitalOut blue_led(p8);

Timeout timeout;
Ticker ticker;

enum STATE
{
  OFF, ALERT, RED, YELLOW , BLUE, ON, START
};

int current_state;
int previe_state;
int manual_count;
int stateButton;

void transitionState();
void defaultState();

void changeRed()
{
    printf("red\n");
    red_led = 1;
    previe_state = current_state;
    current_state = RED;
    timeout.attach(&transitionState, 10);
}

void changeYellow()
{
    printf("yellow\n");
    yellow_led = 1;
    previe_state = current_state;
    current_state = YELLOW;
    timeout.attach(&transitionState, 4);
}

void changeBlue()
{
    printf("blue\n");
    blue_led = 1;
    previe_state = current_state;
    current_state = BLUE;
    timeout.attach(&transitionState, 20);
}

void changeYellow1Hz(){
    yellow_led = !yellow_led;
    ticker.attach(changeYellow1Hz, 0.5);
}

void changeAlert(){
    printf("alert\n");
    previe_state = current_state;
    current_state = ALERT;
    changeYellow1Hz();
}

void changeOff(){
    printf("off\n");
    previe_state = current_state;
    current_state = OFF;
    defaultState();
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
    current_state = START;
    previe_state = START;
    transitionState();
    
    button.rise(&startCount);
    button.fall(&endCount);
    
    while(1) {
        wait(1);
        printf("Button Count %d\n",manual_count);
    }
}

void transitionState(){
    defaultState();
    if(current_state == START && previe_state == START){
        printf("red init\n");
        red_led = 1;
        previe_state = RED;
        current_state = RED;
        timeout.attach(&transitionState, 10);
    }
    else if(stateButton == OFF){
        if(current_state == RED){
            changeBlue();
        }
        else if(current_state == BLUE){
            changeYellow();
        }
        else if(current_state == YELLOW){
            changeRed();
        }
    }
    else if(stateButton == ON){
        stateButton = OFF;
        if(current_state == RED){
            if(manual_count>=3 && manual_count<=10){
                changeAlert();
            }
            else if(manual_count > 10){
                changeOff();
            }
            else{
                changeRed();
            }
        }
        else if(current_state == YELLOW){
            if(manual_count>=3 && manual_count<=10){
                changeAlert();
            }
            else if(manual_count > 10){
                changeOff();
            }
            else{
                changeYellow();
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
            else{
                changeBlue();
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
            else{
                current_state = previe_state;
                changeAlert();
            }
        }
        else if(current_state == OFF){
            if(manual_count > 10){
                changeRed();
            }
            else{
                changeOff();
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
