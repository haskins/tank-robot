/*
 * File:  Main.c
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: April 13, 2015
 * This is the main program for Lab 8, the final lab project for CPSC377
 * Robotics. The purpose of this lab is to have the robot navigate through an
 * obstacle course of pop cans. Using only the sonars, compass, and motors.
 *
 * NOTE: The code for this project is tweaked slightely to be optimized
 * for the B2B3 robot. Using this code on another robot may result in
 * unexpected results.
 *
 */
#include "led.h" //functionality for LEDs
#include "clock.h" //functionality for clock
#include "delay.h" //functionality for custom delays
#include "lcbh100.h" //functionality for the motors
#include "srf04.h" //functionality for sonar
#include "18F4320_config.h" // configures the compiler for the PIC18LF4320
#include "cmps03.h" //functionality for compass
#include <xc.h> //general purpose header file, inclde in all projects
#include <stdio.h> //standard IO libary functions
#include <stdlib.h> //standard libary functions
#include <delays.h> //allows the robot to delay the cpu based on clock ticks.

/**
 * A low priority interrupt for the clock. There can be multiple
 * interrupts inside this method.
 *
 * This method was written by Allen Kranz, minor changes have been made
 * such as RB5 and RB4 changing places.
 */
void interrupt low_priority lowISR(void) {
    // Interrupt raised by change in one of PORTB<7:4>
    if (INTCONbits.RBIF == 1) {
        // right sonar echo just went high so start clock
        if (PORTBbits.RB5 == 1 && PORTBbits.RB4 == 0) {
            WriteTimer0(0);
            ticksRL = 2;
            // Set to 1 for left and 2 for right return pulse
        }// left sonar echo just went high so start clock
        else if (PORTBbits.RB5 == 0 && PORTBbits.RB4 == 1) {
            WriteTimer0(0);
            ticksRL = 1;
            // Set to 1 for left and 2 for right return pulse
        }
        // left and right have finished
        if (PORTBbits.RB5 == 0 && PORTBbits.RB4 == 0) {
            // Set to 1 for left and 2 for right return pulse
            if (ticksRL == 2)
                ticksLeft = ReadTimer0();

            // Set to 1 for left and 2 for right return pulse
            if (ticksRL == 1)
                ticksRight = ReadTimer0();
        }
        portB = PORTB;
        // Clear the mismatch on PORTB
        INTCONbits.RBIF = 0;
        // reset the interrupt flag
    }
    // Now check for any other low level interrupts
}

/**
 * A high priority inturrupt for the clock. When the inturrupt is called
 * the TMR3IF, or Timer3 Inturrupt flag, is activated, execution will
 * move here. The tick_units are then incremented and the inturrupt flag
 * is reset.
 */
void interrupt high_priority highISR(void) {
    if (1 == PIR2bits.TMR3IF) 
        tick_units++;

    PIR2bits.TMR3IF = 0;
    //resets the inturrupt flag.
}

/**
 * States for state machine.
 */
enum STATE {
    scan, straight, dodge_left, dodge_right, fix_bearing, error, all_clear
};


/*
 * A custom delay fucntion that takes an int, and delays the cpu on the 
 * robot by (2550 clock ticks) * (number of iterations of loop)
 * This allows us for more precise control over the length of a delay.
 */
void custom_delay(int iterations) {
    for (int i = 0;
            i < iterations;
            i++)
        Delay10TCYx(255);
}

/*
 * This function is the main program for the robot.
 */
int main(int argc, char** argv) {
    //sets up hardware for use
    openLED();    //led
    Openlchb100();    //motor
    OpenCMPS03();    //compass
    OpenSRF04();    //sonar

    //sets the intended bearing using the compass
    custom_delay(100);
    unsigned char intended_bearing = ReadCMPS03();

    //tuning/tweaking parameters. Changes the way the robot performs.
    char right_turn_distance = 0;
    char left_turn_distance = 0;
    char allowed_error_normal = 2; // The allowed margain of error allowed
                                   //for the compass when the robot is
                                   //travelling in a straight line.
    unsigned char allowed_error_turn = 5; // The allowed margain of error
                                          //allowed when the robot is
                                          //adjusting its course.
    unsigned char current_bearing;
    unsigned char detection_distance = 12; //of the sonars.
    int travel_time_dodge = 800;
    int travel_time_all_clear = 200;
    unsigned char motor_speed_left = 217; //The left motor on the robot is
                                          //roughly 3.5% slower than the 
                                          //right motor. Thus the speed is
                                          //slightly faster to accommodate
                                          //this differnece.
    unsigned char motor_speed_right = 210;
    unsigned char motor_tick_rate = 30; //The length of time that the motors
                                        //on enabled for while the robot is
                                        //correcting bearing.
                                        //Lower tick values provide better
                                        //accuracy at the cost of speed.
    unsigned char bearing_adjust = 0;   //gets incemented by
                                        //bearing_adjust_amount every time 
                                        //the robot dodges a pop can to
                                        //account for the compass losing
                                        //acuracy as the robot travels.
    unsigned char bearing_adjust_amount = 3;
    enum STATE state = scan;
    int lane = 0; //Tracks the left/right position of the robot relative to
                  //the centre of the table. +1 is considered the right
                  //side, and -1 is the left side of the table. 0 is the
                  //centre if the table.

    //how long the robot turns when dodging obstacles.
    int degree_turn = 500; 

    while (1) {

        unsigned char adjusted_intended = intended_bearing + bearing_adjust;

        switch (state) {
            //The main state. The robot will ping the sonars to determine if
            //any objects are in front of the robot. Based on the results 
            //from the sonars the robot will change states and react
            //accordingly.
            case scan:
            {
                setLED(0b00000001);
                //reads the current bearing
                custom_delay(10);
                current_bearing = ReadCMPS03();
                //if the robot enters a nonexistant lane, enter the error
                //state
                if (lane > 1 || lane < -1) 
                    state = error;

                //If the robot is no longer on its correct bearing, fix it.
                if (current_bearing >= adjusted_intended + 
                        allowed_error_normal || current_bearing <=
                        adjusted_intended - allowed_error_normal)
                    state = fix_bearing;

                else {
                    //checks left sonar 
                    TriggerLeftSRF04();
                    delay_ms(100);
                    //scales sonar to approx. cm's
                    unsigned int leftCM = ticksLeft / 75;
                    //checks right sonar
                    TriggerRightSRF04();
                    delay_ms(100);
                    //scales sonar to approx. cm's
                    unsigned int rightCM = ticksRight / 75;
                    
                    //check both right and left distances, and act
                    //accordingly
                    if (leftCM < detection_distance && rightCM <
                            detection_distance && lane != 1) 
                        state = dodge_right;
                    else if (leftCM < detection_distance && rightCM <
                            detection_distance && lane != -1) 
                        state = dodge_left;
                    else if (leftCM < detection_distance && lane != -1) 
                        state = dodge_right;
                    else if (leftCM < detection_distance) 
                        state = dodge_right;
                    else if (rightCM < detection_distance && lane != 1) 
                        state = dodge_left;
                    else if (rightCM < detection_distance) 
                        state = dodge_left;
                    else 
                        state = all_clear;
                }
                break;
            }

            //Activates the motors on the robot so the robot drives forward.
            //Displays the current lane the robot thinks it is currently in.
            case all_clear:
            {
                switch (lane) {
                    case -1:
                    {
                        setLED(0b10000000);
                        break;
                    }
                    case 0:
                    {
                        setLED(0b11000000);
                        break;
                    }
                    case 1:
                    {
                        setLED(0b11100000);
                        break;
                    }
                    default:
                    {
                        setLED(0b11111111);
                        break;
                    }
                }
                //turns the motors on
                Runlchb100(motor_speed_left, motor_speed_right);
                //runs forward for some period of time before changing
                //state.
                custom_delay(travel_time_all_clear);
                state = scan;
                break;
            }

            //The robot enters this state when the robot corrects its
            //direction.
            case straight:
            {
                setLED(0b00011111);
                Runlchb100(motor_speed_left, motor_speed_right);
                custom_delay(travel_time_dodge);
                Closelchb100();
                state = scan;
                break;
            }

            //The robot enters this state when the robot detects a pop can 
            //on its right side.
            case dodge_left:
            {
                setLED(0b00000111);

                //changes the current lane
                lane = lane - 1;

                //turn left
                Runlchb100(-motor_speed_left, motor_speed_right);
                custom_delay(degree_turn);

                //go straight
                Runlchb100(motor_speed_left, motor_speed_right);
                custom_delay(travel_time_dodge);

                //turn right
                Runlchb100(motor_speed_left, -motor_speed_right);
                custom_delay(degree_turn*0.95);

                bearing_adjust += bearing_adjust_amount;
                state = straight;
                break;
            }

            //The robot enters this state when the robot detects a popcan 
            //on its left side.
            case dodge_right:
            {
                setLED(0b00001111);

                //changes the current lane
                lane = lane + 1;

                //turn right
                Runlchb100(motor_speed_left, -motor_speed_right);
                custom_delay(degree_turn);

                //go straight
                Runlchb100(motor_speed_left, motor_speed_right);
                custom_delay(travel_time_dodge);

                //turn left
                Runlchb100(-motor_speed_left, motor_speed_right);
                custom_delay(degree_turn);

                bearing_adjust += bearing_adjust_amount;
                state = straight;
                break;
            }

            //the robot will enter this state when it enters a lane that
            //doesn't exist.
            case error:
            {
                while (1) {
                    setLED(0b10101010);
                    delay_s(1);
                    setLED(0b01010101);
                    delay_s(1);
                }
            }

            //When the robot is not going in the correct direction, it will
            //enter this state to correct its direction.
            case fix_bearing:
            {
                setLED(0b00011111);

                //calculate turn distances. right and left turn have to sum
                //to 255 binary radians.
                right_turn_distance = adjusted_intended - current_bearing;
                left_turn_distance = 255 - right_turn_distance;
                //turn right until bearing reached
                if (right_turn_distance < left_turn_distance) {

                    do {
                        Runlchb100(motor_speed_left, -motor_speed_right);
                        custom_delay(motor_tick_rate);
                        Closelchb100();
                        current_bearing = ReadCMPS03();
                        custom_delay(motor_tick_rate);
                        custom_delay(motor_tick_rate);
                        if (current_bearing >= adjusted_intended - 
                                allowed_error_turn && current_bearing <=
                                adjusted_intended + allowed_error_turn) {
                            break;
                        }
                    } while (1);
                    
                }//turn left until bearing reached
                else {
                    do {
                        Runlchb100(-motor_speed_left, motor_speed_right);
                        custom_delay(motor_tick_rate);
                        Closelchb100();
                        current_bearing = ReadCMPS03();
                        custom_delay(motor_tick_rate);
                        custom_delay(motor_tick_rate);

                        if (current_bearing >= adjusted_intended - 
                                allowed_error_turn && current_bearing
                                <= adjusted_intended + allowed_error_turn) {
                            break;
                        }
                    } while (1);
                }
                state = scan;
                break;
            }
        }
    }
    return (EXIT_SUCCESS);
}