/*********************************************************************************************************************************************
*
* Program Name: MicroBit.cpp
*
* First written on 30/01/2018 by Kieran Foley.
* Last modified on 07/02/2018 by Kieran Foley.
*
* Author: Kieran Foley
*
* Program Description: This program wil execute a game in which the aim is to avoid all enemies (LED's) coming down from the top of the board.
*   The player starts of positioned in the middle of the bottom row and is controlled using buttons A and B to move left and right respectively.
*   Once the player collides with an ememy, the game will end with the LED's displaying a "Game Over" message.
*********************************************************************************************************************************************/

#include "MicroBit.h"

MicroBit uBit;
// Sets the speed of the game (higher number = longer sleep to display enemies).
int speed = 250;
// Set the players position to start in the middle of the bottom row.
int player = 2;

MicroBitSerial serial(USBTX,USBRX);

/*****************************************************************************************
*
* Function name : onButtonA()
*    returns : nil
*    arg1 : microBitEvent e
*    arg2 : nil
* Created by : Kieran Foley
* Date created : 06/01/2018
* Description : This function moves the player LED to the left unless the LED is already 
*   as far left as possible (unable to fall off the board).
*
*****************************************************************************************/
void onButtonA(MicroBitEvent e) {
      uBit.display.image.setPixelValue(player,4,0);
      if (player-1 == -1) {
      } else {
      player = player-1;
    }
}

/*****************************************************************************************
*
* Function name : onButtonB()
*    returns : nil
*    arg1 : microBitEvent e
*    arg2 : nil
* Created by : Kieran Foley
* Date created : 06/01/2018
* Description : This function moves the player LED to the right unless the LED is already 
*   as far right as possible (unable to fall off the board).
*
*****************************************************************************************/
void onButtonB(MicroBitEvent e) {
      uBit.display.image.setPixelValue(player,4,0);
      if (player + 1 == 5) {
      } else {
      player = player + 1;
    }
}

/*****************************************************************************************
*
* Function name : moveEnemy()
*    returns : nil
*    arg1 : nil
*    arg2 : nil
* Created by : Kieran Foley
* Date created : 06/01/2018
* Description : This function displays the player and the enemies on the LED board.
*   It generates a random number to decide which column the enemy till go down through whilst
*   checking if it has collided with the player (once it's hit the 4th row). If this returns
*   true, the game will end displaying "Game Over" to the user and terminating the program.
*
*****************************************************************************************/
void moveEnemy() {
    // Creates a random number between 0 - 4 to decide which column the LED's head south on.
    int randomNo = uBit.random(5);
    // Loops 5 times to move the LED's down the board through the 5 rows.
    // Sets the enemies, player LED's and speed of the game.
    for (int i = 0; i <5; i++) {
        uBit.display.image.setPixelValue(randomNo, i, 255);
        uBit.display.image.setPixelValue(player,4,255);
        uBit.sleep(speed);
        uBit.display.image.setPixelValue(randomNo, i, 0);

        // Speed up the game the longer it's running (but don't make it impossible) by reducing the sleep time.
        if (speed >100) {
          speed = speed - 3;
        }

    // If the player and enemy have collided -> Game Over.
    if (i == 4 && player == randomNo) {
        // Display "Game Over" to the user in a scrolling animation.
        uBit.display.scroll("Game Over");
        // Processor enters a power efficient sleep. Program ceases all execution.
        release_fiber();
    }
  }
}

/*****************************************************************************************
*
* Function name : main()
*    returns : nil
*    arg1 : nil
*    arg2 : nil
* Created by : Kieran Foley
* Date created : 30/01/2018
* Description : This function is the main function of the program. Its purpose is to initialise
*   the microBit, setup terminal printing and calling the other functions to create the game.
*   This function creates listeners to execute callbacks when buttons A and B are pushed and
*   loops through the function 'moveEnemy' to keep enemies moving down towards the player, 
*   This function is terminated once the player and enemy collide.
*
*****************************************************************************************/
int main() {
    // Initialises the microbit scheduler, memory allocator and bluetooth stack.
    uBit.init();

    // Setup a device driver for the terminal to recognise the seriel interface of the microbit.
    serial.baud(115200);

    // Calls the event listeners for buttons A and B.
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,MICROBIT_BUTTON_EVT_CLICK,onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B,MICROBIT_BUTTON_EVT_CLICK,onButtonB);

    // Keeps enemies coming until the game ends and allows the program to yield.
    while(1) {
        moveEnemy();
    }
}
