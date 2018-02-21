#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdio.h>
#include <time.h>

/* symbolic constants */
#define MIN_BET 5
#define TRUE 1
#define FALSE 0
#define MAX_ROLL 6

/* Function prototypes */
int getWallet(void);
int makeBet(int walletCash);
void playCraps(int walletCash);
int doAgain(void);
void goodbye(int walletCash);
int rollForPoint(int point);
int rollDice(void);
int rollDie(void);

/*
* Function: getWallet
* Prompts user for the amount of money they have to play with
* - assumed to be measured in whole dollars.
* Rejects values that are not at least MIN_BET in size and prompts
* for another value until a value of size MIN_BET or greater is entered.
* Returns: amount of money entered by user (a positive, integer dollar value)
*/
int getWallet(void) {
	int startCash;
	int count = 0;

	/* gets the amount in the users wallet, and it must be greater than MIN_BET */
	do {
		if (count++ > 0)
			printf("Please make sure the amount is greater than the minimum bet.\n\n");

		printf("How much money do you have to play with?\n");
		scanf("%d", &startCash);
	} while (startCash < MIN_BET);

	return startCash;
}

/*
* Function: makeBet
* Prompts user to make a bet (minimum value: MIN_BET; maximum value: amount in wallet)
* Keeps prompting user until a value of at least MIN_BET but no more than amount
* in wallet is entered
* Parameter: wallet - the amount in the user's wallet
* Returns: user's bet (minimum MIN_BET)
*/
int makeBet(int walletCash) {
	int bet;

	printf("\nPlace your bet (minimum $%d): ", MIN_BET);
	scanf("%d", &bet);
	while (bet < MIN_BET) {
		printf("Your bet is below the minimum of $%d, try again.\n", MIN_BET);

		goodbye(walletCash);

		printf("Place your bet (minimum $%d): ", MIN_BET);
		scanf("%d", &bet);
	}

	while (bet > walletCash) {
		printf("You have bet more than you have in your wallet, try again.\n");

		goodbye(walletCash);

		printf("Place your bet (minimum $%d): ", MIN_BET);
		scanf("%d", &bet);
	}

	return bet;
}

/*
* Function: playCraps
* Plays a single round of craps with the suer.
* Returns: true if user won round, false otherwise.
*/
void playCraps(int walletCash) {
	int bet;
	int win;
	int play;
	int point;
	int roll;

	srand((unsigned)time(NULL));

	bet = makeBet(walletCash);

	roll = rollDice();
	if (roll == 2 || roll == 3 || roll == 12)
		win = FALSE;
	else if (roll == 7 || roll == 11)
		win = TRUE;
	else {
		point = roll;
		win = rollForPoint(point);
	}

	if (win == TRUE) {
		printf("You win :-)\n");
		walletCash += bet;
	}

	else {
		printf("You lose :-(\n");
		walletCash -= bet;
	}

	if (walletCash < MIN_BET)
		play = FALSE;
	else
		play = doAgain();

	if (play == TRUE) {
		goodbye(walletCash);
		playCraps(walletCash);
	}
	else {
		goodbye(walletCash);
		if (walletCash > MIN_BET)
			printf("Goodbye!\n");
	}
}

/*
* Function: doAgain
* Ask user if they want to play again
* Returns: true if user wants to play again, false otherwise.
*/
int doAgain(void) {
	int play;

	printf("Enter 1 to play again, 0 to quit: ");
	scanf("%d", &play);

	return play;
}

/*
* Function: goodbye
* Prints a goodbye message to the user based on whether or not they
* went broke while playing the game. Tells the user they're broke
* if they have less than MIN_BET in their wallet, otherwise tells
* them how much they have in their wallet.
* Parameter: wallet - amount of money in wallet
*/
void goodbye(int walletCash) {
	if (walletCash < MIN_BET)
		printf("You are broke! Goodbye!\n");
	else
		printf("You have $%d in your wallet. ", walletCash);
}

/*
* Funtion: rollForPoint
* Repeatedly rolls dice until either the point value or the value 7 is rolled.
* Parameter: point - the current point value
* Returns: true if user rolled point value before rolling
* a 7 (user won round), false otherwise (user lost round)
*/
int rollForPoint(int point) {
	int roll;
	int win;
	printf("Rolling for point: %d...\n", point);

	roll = rollDice();
	while (roll != point && roll != 7) {
		roll = rollDice();
	}

	if (roll == 7)
		win = FALSE;
	else
		win = TRUE;

	return win;
}

/*
* Function: rollDice
* Rolls a pair of dice.
* Returns: sum of face values rolled
*/
int rollDice(void) {
	int roll_1 = rollDie();
	int roll_2 = rollDie();
	int roll = roll_1 + roll_2;

	printf("You rolled a %d.\n", roll);

	return roll;
}

/*
* Function: rollDie
* Rolls a single die.
* Returns: face value rolled
*/
int rollDie(void) {
	return rand() % MAX_ROLL + 1;
}
