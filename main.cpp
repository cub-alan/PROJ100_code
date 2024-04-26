#include "mbed.h"
#include "motor.h"
#include "tunes.h"
#include "pwm_tone.h"
#include "PROJ100_Encoder.h"
#include "PROJ100_Encoder_Tests.h"
#include <cstdio>

#define TIME_PERIOD 10             //Constant compiler Values here 10 equates to 10ms or 100Hz base Frequency
#define ENCODER_PIN_LEFT            D8
#define ENCODER_PIN_RIGHT           D6 
#define PULSES_PER_ROTATION         20
#define DEBOUNCE_US                 30000

DigitalIn microswitch1(D4);         //Instance of the DigitalIn class called 'microswitch1'
DigitalIn microswitch2(D3);         //Instance of the DigitalIn class called 'microswitch2'
DigitalIn myButton(USER_BUTTON);    //Instance of the DigitalIn class called 'myButton'   
DigitalOut greenLED(LED1);          //Instance of the DigitalOut class called 'greenLED'

Motor Wheel(D13,D11,D9,D10);      //Instance of the Motor Class called 'Wheel' see motor.h and motor.cpp

PROJ100_Encoder right_encoder (ENCODER_PIN_RIGHT,PULSES_PER_ROTATION);  //Instance of the PROJ100Encoder class called 'right_encoder'
PROJ100_Encoder left_encoder(ENCODER_PIN_LEFT,PULSES_PER_ROTATION);     //Instance of the PROJ100Encoder class called 'left_encoder'

UnbufferedSerial ser(USBTX,USBRX,115200);   // Serial object for printing info

int main ()
{
    // Clear the terminal and print an intro
    printf("\033[2J\033[1;1H\r\n");
    printf("Plymouth University PROJ100 2023/24 Optical Encoder Demonstration\n\r");

    // Set the PWM frequency
    Wheel.Period_in_ms(TIME_PERIOD);                

    // Ensure our motors and encoders are stopped at the beginning
    right_encoder.stop();
    left_encoder.stop();
    Wheel.Stop();

    // The encoder pulses can be noisy and so are debounced within the class
    // You can experiment with changing the debouce time if you wish
    left_encoder.setDebounceTimeUs(DEBOUNCE_US);
    right_encoder.setDebounceTimeUs(DEBOUNCE_US);

    // Wait for the blue button to be pressed
    printf("Press Blue Button To Begin\n\r");
    while (myButton == 0){greenLED = !greenLED; ThisThread::sleep_for(100ms);}

    // Start the encoders
    left_encoder.start();
    right_encoder.start();

    /*********************ENCODER TESTS***************/
    // These contain while(1) loops so ensure that they are removed or commented out when running your own code
    // If these lines are left in the lines below will never run
    /*************************************************/

    //simple_test();
    //speed_test();
    int taskselection = 0;
    while (myButton == 1){
        wait_us(1000*1000); // wait 1 second
        taskselection = taskselection + 1; // add onje to the interger
    }
    
    if(taskselection < 2) { // if the button is only pressed and not held begin to run the beed pushing task

        while(1){ // infinate loop for beed pushing task
            
            Wheel.Speed(1.0,1.0); // move forward at full speed
            wait_us(10000*1000); // change to time taken to get to the edge

            Wheel.Speed(0,0);
            wait_us(500*1000);

            while (microswitch1 == 0){ // until the buggy hits the back
                Wheel.Speed(-1.0,-1.0); // reverse at full speed
            }
            
            Wheel.Speed(0,0);
            wait_us(500*1000);

            Wheel.Speed(-1.0,1.0); // turn on the spot
            wait_us(1500*1000); // wait till its 45 degrres

            Wheel.Speed(0,0);
            wait_us(500*1000);
            
            Wheel.Speed(1.0,1.0); // move forard away from back wall
            wait_us(1000*1000); // until it has moved away from the wall

            Wheel.Speed(0,0);
            wait_us(500*1000);

            Wheel.Speed(1.0,-1.0); // turn on the spot
            wait_us(1500*1000); // wait till its 45 degrres back to parrellel to the wall

            Wheel.Speed(0,0);
            wait_us(500*1000);

            while (microswitch1 == 0){ // until the buggy hits the back
                Wheel.Speed(-1.0,-1.0); // reverse at full speed
            }

            Wheel.Speed(0,0);
            wait_us(500*1000);

        }
    }
    else if (taskselection <4 ){ // if the button is still on after 3 seconds do the parrellel line task
        
        Wheel.Speed(1.0,1.0); // go forward both wheals at full speed till the end
        wait_us(10000*1000); // currently at 1 second but will change to time to reach end

        Wheel.Speed(0,0); // make sure it has stopped
        wait_us(1000*1000); // wait a second before going 

        Wheel.Speed(1.0,-1.0); // spin on the spot both full speed 1 negative one positive
        wait_us(5000*1000); // also 1 second for now but change to 180 timing

        Wheel.Speed(0,0); // make sure it has stopped
        wait_us(1000*1000); // wait a second before going 

        Wheel.Speed(1.0,1.0); // drive forward back to the start both wheels full speed
        wait_us(10000*1000); // change to time taken to drive back

        Wheel.Speed(0,0); // make sure it has stopped
        wait_us(1000*1000); // wait a second before going 

        Wheel.Speed(-1.0,1.0); // spin 180 the other way as before
        wait_us(5000*1000); // change to time to turn

        Wheel.Speed(0,0); // stop the buggy
        BATMAN(2); // victory tune
    }
    else if (taskselection < 6) {

        Wheel.Speed(1.0, 1.0);
        wait_us(10000*1000);

        Wheel.Speed(0,0);

        BATMAN(2);
    
    }
}