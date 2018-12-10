#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PINS 19

// pin definitions
#define P_ORANGE 14
#define P_RED 15
#define P_BROWN 17
#define P_BLACK 27
#define P_WHITE 22
#define P_GREY 10
#define P_PURPLE 9
#define P_BLUE 11
#define P_GREEN 13
#define P_YELLOW 19
#define P_ORANGE2 26
#define P_A 18
#define P_B 23
#define P_C 24
#define P_D 25
#define P_E 8
#define P_F 12
#define P_G 16
#define P_H 20

#define NUM_TOP_PINS 11
char top_pins[NUM_TOP_PINS] = {P_ORANGE, P_RED, P_BROWN, P_BLACK, P_WHITE, P_GREY, P_PURPLE, P_BLUE, P_GREEN, P_YELLOW, P_ORANGE2};

#define NUM_BOTTOM_PINS 8
char bottom_pins[NUM_BOTTOM_PINS] = {P_A, P_B, P_C, P_D, P_E, P_F, P_G, P_H};

void sleep_ms(int ms){
    usleep(1000.0 * ms);
}

int main(int c, char *v[])
{
    if(gpioInitialise()< 0) {
        printf("setup failed !\n");
        return -1;
    }
    for (int i = 0; i < NUM_BOTTOM_PINS; i++) {
        char bottom_pin = bottom_pins[i];
        gpioSetMode(bottom_pin, PI_OUTPUT);
        gpioWrite(bottom_pin, 0);
    }
    for (int i = 0; i < NUM_TOP_PINS; i++) {
        gpioSetMode(top_pins[i], PI_INPUT);
    }
    while(1){
        for (int j = 0; j < NUM_BOTTOM_PINS; j++) {
            char bottom_pin = bottom_pins[j];
            gpioWrite(bottom_pin, 1);
            sleep_ms(100);
            for (int k = 0; k < NUM_TOP_PINS; k++) {
                char top_pin = top_pins[k];
                if (gpioRead(top_pin)) {
                    printf("Detected hit at\t Top %d \tBottom %d\n", top_pin, bottom_pin);
                }
                sleep_ms(100);
            }
            gpioWrite(bottom_pin, 0);
            printf("=====\n");
            sleep_ms(100);
        }
    }
}

