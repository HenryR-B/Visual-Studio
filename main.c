/*
 * Author: Henry Bryant
 * Date: Feb. 18, 2018
 * Purpose: Prompts user if they want to play a game, and based on their input, plays that game (using a function call)
 *			prompts user if they want to play again, and if yes, prompts them which game they would like to play.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Craps.h"
#include "yahtzee.h"

int main(void) {
	int walletCash;
	char input;
	int start;
	char choice;
	char play;
	
	/* check which game the user would like to play */
	do {
		printf("Which game would you like to play? Enter the number associated with the game (Or enter 0 to quit).\n");
		printf("0. Quit\n");
		printf("1. Craps\n");
		printf("2. Yahtzee\n");
		printf("Your response: ");
		/* enter a number and checks if it is not a character */
		start = checkChar();
		
		/* checks that the number entered was in the range of 0 to 2 inclusive */
		while (start < 0 || start > 2) {
			printf("Which game would you like to play? Enter the number associated with the game (Or enter 0 to quit).\n");
			printf("0. Quit\n");
			printf("1. Craps\n");
			printf("2. Yahtzee\n");
			printf("Your response: ");
			start = checkChar();
		}
		
		/* based on the value of 'start', do the specific case related to that */
		switch (start) {
			case 0:
				return;
			case 1:
				/* play craps */
				printf("\nAre you ready to start your game of craps? (Y/N) ");
				input = checkValidInput(input);


				if (input == 'Y' || input == 'y') {
					walletCash = getWallet();
					playCraps(walletCash);
				}
				else {
					printf("\nWould you like to return to the main menu? (Y/N) ");
					play = checkValidInput(play);


					if (play == 'Y' || play == 'y')
						break;
					else {
						/* get the users amount of money to play with */
						walletCash = getWallet();
						playCraps(walletCash);
					}
				}
				break;
			case 2:
				/* play yahtzee */
				printf("\nAre you ready to play your game of yahtzee? (Y/N) ");
				input = checkValidInput();

				if (input == 'Y' || input == 'y') {
					playYahtzee();
					play = 'N';
				}
				else {
					printf("\nWould you like to return to the main menu? (Y/N) ");
					play = checkValidInput();

					if (play == 'Y' || play == 'y')
						break;
					else {
						playYahtzee();
						play = 'N';
					}
				}
				break;
		}
		
		if (play == 'Y' || play == 'y') {
			choice = 'Y';
		}
		else {
			printf("Do you want to play again? (Y/N) ");
			choice = checkValidInput();
		}
		printf("\n");
		/* do this until the user wants to quit. */
	} while (choice == 'Y' || choice == 'y');
	
	system("PAUSE");
	return 0;
}