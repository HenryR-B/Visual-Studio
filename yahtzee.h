#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "yahtzeeFunctions.h"

/* symbolic constants */
#define TRUE		1
#define FALSE		0
#define MAX_DICE	5
#define OPTIONS		13
#define NUM_PLAYERS		4
#define MAX_LENGTH		100
#define BONUS_UPPER_TOTAL	35
#define BONUS_CUTOFF		63
#define BONUS_SECTION		2
#define SINGLEPLAYER		1
#define BUF_SIZE		  1000
#define YAHTZEE_RULES "yahtzee_rules.txt"
#define YAHTZEE_INSTR "yahtzee_instructions.txt"

/* Function prototypes */
void playYahtzee(void);
int playRound(int round, int currentPlayer, int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[], int grandTotal[], int allPlayers[]);
int performOption(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[]);
void calculateTotal(int numPlayers, int upperTotal[][NUM_PLAYERS], int lowerTotal[], int grandTotal[], int allPlayers[]);
void printFile(FILE * file);

/*
 * Function playYahtzee - plays yahtzee with the user.
 * Asks if the user wants to see the rules or instructions, 
 * as well as how many players will be playing.
 */
void playYahtzee(void) {
	/* declaring variables */
	FILE * GameRules;
	FILE * tutorial;
	int usedOptions[NUM_PLAYERS][OPTIONS] = { FALSE };
	int upperTotal[BONUS_SECTION][NUM_PLAYERS] = { 0 };
	int lowerTotal[NUM_PLAYERS] = { 0 };
	int grandTotal[NUM_PLAYERS];
	int allPlayers[NUM_PLAYERS];
	char choice;

	int numPlayers;
	int currentPlayer = 0;
	int totalPoints[NUM_PLAYERS] = { 0 };
	char playerName[NUM_PLAYERS][MAX_LENGTH];
	char buffer[BUF_SIZE];

	srand((unsigned)time(NULL));
	
	/* Ask user if they want to see the rules and then print the file if Yes */
	printf("\nDo you want to read the rules for yahtzee? (Y/N) ");
	checkValidInput(buffer);
	choice = buffer[0];
	if (choice == 'Y' || choice == 'y') {
		GameRules = fopen(YAHTZEE_RULES, "r");
		printFile(GameRules);
	}
	printf("Press any key to continue, when you are ready\n ");
	system("PAUSE > nul");
	/* Ask user if they want to see the instructions and then print the file if Yes */
	printf("\nDo you want read the tutorial/instructions (ie. are you familiar with this particular system)? (Y/N) ");
	checkValidInput(buffer);
	choice = buffer[0];
	if (choice == 'Y' || choice == 'y') {
		tutorial = fopen(YAHTZEE_INSTR, "r");
		printFile(tutorial);
	}
	printf("Press any key to continue, when you are ready\n ");
	system("PAUSE > nul");
	printf("\n---------------------------------------------------------------------------------------\n");
	
	/* Check for the number of players */
	numPlayers = checkPlayers();

	/* If the number of players is greater than 1, the game is multiplayer */
	if (numPlayers > SINGLEPLAYER) {
		/* Prompts all players to enter names one by one and prints a welcome message */
		for (int index = 0; index < numPlayers; index++) {
			printf("Player %d, enter your name: ", index + 1);
			/* allows multiple word names */
			gets(playerName[index]);
			printf("Hello %s, welcome to yahtzee!\n\n", playerName[index]);
		}

		printf("\n-------------------------------------------------");
		printf("\n       Welcome to Yahtzee Multiplayer!\n");
		printf("-------------------------------------------------\n");
		/* For each round (up to 13 rounds) each player takes one turn, points added to total */
		for (int round = 1; round <= 13; round++) {
			printf("\n=================================================\n");
			printf("                    Round %d\n", round);
			printf("=================================================\n");
			for (currentPlayer = 0; currentPlayer < numPlayers; currentPlayer++) {
				printf("\n-------------------------------------------------");
				printf("\n         %s's turn (Round: %d)\n", playerName[currentPlayer], round);
				printf("-------------------------------------------------\n");
				totalPoints[currentPlayer] += playRound(round, currentPlayer, usedOptions, upperTotal, lowerTotal, grandTotal, allPlayers);
				printf("\n         %s's points: %d\n", playerName[currentPlayer], totalPoints[currentPlayer]);
				printf("=================================================\n\n");
			}
		}

		/* calculates final scores in descending order (including upper and lower scores, and if the bonus was applied) */
		printf("\n-----------------------------------------------\n");
		printf("----------------- FINAL SCORES ----------------");
		printf("\n-----------------------------------------------\n\n");
		calculateTotal(numPlayers, upperTotal, lowerTotal, grandTotal, allPlayers);
		for (int index = 0; index < numPlayers; index++) {
			printf("%d place: %s, with %d points total\n", index + 1, playerName[allPlayers[index]], grandTotal[index]);
			if (upperTotal[1][index] == TRUE) {
				printf("\nBonus Upper Section Applied");
			}
			else
				printf("\nBonus not applied");

			printf("\nUpper total: %d\n", upperTotal[0][index]);
			printf("Lower total: %d\n\n", lowerTotal[index]);
		}
		printf("\n\nGood Job everyone!\n\n");
		printf("==================================================\n\n");
	}

	/* Otherwise, the game is singleplayer */
	else {
		printf("\n-----------------------------------------------\n");
		printf("        Welcome to Yahtzee Singleplayer!\n");
		printf("-----------------------------------------------\n\n");

		/* for each round, player takes one turn, points added to total */
		for (int round = 1; round <= 13; round++) {
			printf("\n=================================================\n");
			printf("                    Round %d\n", round);
			printf("=================================================\n");
			totalPoints[currentPlayer] += playRound(round, currentPlayer, usedOptions, upperTotal, lowerTotal, grandTotal, allPlayers);
			printf("\nYour points: %d\n", totalPoints[currentPlayer]);
		}
		printf("\n-----------------------------------------------\n");
		printf("----------------- FINAL SCORES ----------------");
		printf("\n-----------------------------------------------\n");
		/* prints final scores - since it is only one player, it is only one score */
		printf("\nUpper total: %d", upperTotal[0][currentPlayer]);
		if (upperTotal[1][currentPlayer] == TRUE) {
			printf("\nBonus Upper Section Applied");
		}
		else
			printf("\nBonus not applied");

		printf("\nLower total: %d\n", lowerTotal[currentPlayer]);
		printf("\nTotal Points: %d		Good Job!\n\n", totalPoints[currentPlayer]);
		printf("=================================================\n\n");
	}
}

/*n
 * Funtion playRound - plays a single round of yahtzee with user
 * Param: round - the current round
 * Param: currentPlayer - the current player who is taking their turn
 * Param: usedOptions - the options that are used for the current player
 * Param: upperTotal - array of upper total scores for each player
 * Param: lowerTotal - array of lower total scores for each player
 * Param: grandTotal - array of lower total + upper total for each player
 * Param: allPlayers - array to store the index of the players
 * Returns: the points gained in this round
 */
int playRound(int round, int currentPlayer, int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[], int grandTotal[], int allPlayers[]) {
	int count = 1;
	int choice = FALSE;
	int option;
	int totalPoints;
	int currentDice[NUM_PLAYERS][MAX_DICE];
	char keep;
	char buffer[BUF_SIZE];

	/* rolls the die 5 times, and puts it into the slots for the current player */
	printf("\nFirst roll:\n");
	for (int index = 0; index < MAX_DICE; index++) {
		currentDice[currentPlayer][index] = rollDie();
	}
	/* sorts the dice and then prints it graphically */
	sortArray(currentDice, currentPlayer);
	printDice(currentDice, currentPlayer);

	/* asks the user if they want to choose an option (ie. full house) for their roll - if yes, perform that option */
	choice = chooseCombination();
	if (choice == TRUE) {
		option = chooseOption();
		totalPoints = performOption(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
	}

	/* otherwise, allow the user to roll two more times, or until the user wants to use their roll for an option */
	while (count <= 2 && choice == FALSE) {
		printf("\nDo you want to keep any dice? (Y/N) ");
		checkValidInput(buffer);
		keep = buffer[0];
		
		/* if the user wants to keep any dice, call the function to roll the dice they want */
		if (keep == 'Y' || keep == 'y') {
			rollNotKeepers(currentPlayer, currentDice);

			/* if the user has not rolled twice yet, ask if they want to choose an option, etc. */
			if (count != 2) {
				choice = chooseCombination();
				if (choice == TRUE) {
					option = chooseOption();
					totalPoints = performOption(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
				}
			}
		}
		else {
			/* otherwise, roll all dice again, and print the dice */
			printf("\n\nYou have rolled: ");
			for (int index = 0; index < MAX_DICE; index++) {
				currentDice[currentPlayer][index] = rollDie();
			}
			sortArray(currentDice, currentPlayer);
			printDice(currentDice, currentPlayer);

			/* ask user if they want to choose an option, etc. */
			choice = chooseCombination();
			if (choice == TRUE) {
				option = chooseOption();
				totalPoints = performOption(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
			}
		}
		count++;
	}
	/* User has rolled 3 times, so they must choose an option */
	if (count > 2) {
		printf("\nYou have rolled three times, you can't roll again.\n");
		option = chooseOption();
		totalPoints = performOption(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
	}
	
	return totalPoints;
}


/*
 * Function performOption - performs the given option and calls other functions to complete it
 * Param: option - the option to be carried out
 * Param: currentPlayer - the current player who is taking their turn
 * Param: currentDice - an array of current dice for all players
 * Param: usedOptions - an array of all the used options for all players
 * Param: upperTotal - an array of totals in the upper section for all players
 * Param: lowerTotal - an array of totals in the lower section for all players
 * Returns: all four arrays, as well as the points gained for this turn
 */
int performOption(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[]) {
	int points = 0;

	/* checking if the user enters yahtzee bonus, but they havent selected the yahtzee yet */
	if (usedOptions[currentPlayer][13 - 1] == FALSE && option == 14) {
		/* print a message to user if they entered an invalid option */ 
		printf("\nYou think you can outsmart me? You must use the 50 point yahtzee before your bonus yahtzee!\n");
		option = 13;
		points = check_3_4_5_Kind(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
		lowerTotal[currentPlayer] += points;
	}
	
	else if (usedOptions[currentPlayer][option - 1] == FALSE) {
		/* checking if option entered was in the upper section */
		if (option <= 6) {
			points = check_1_to_6(option, currentPlayer, currentDice, usedOptions);
			upperTotal[0][currentPlayer] += points;
		}

		/* checking if option entered is a full house */
		else if (option == 11) {
			points = checkFullHouse(option, currentPlayer, currentDice, usedOptions);
			lowerTotal[currentPlayer] += points;
		}

		/* checking if option entered is a 3/4 of a kind or a yahtzee */
		else if (option <= 8 || option >= 13) {
			points = check_3_4_5_Kind(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
			lowerTotal[currentPlayer] += points;
		}

		/* checking if option entered is a straight (large or small) */
		else if (option <= 10) {
			points = checkStraight(option, currentPlayer, currentDice, usedOptions);
			lowerTotal[currentPlayer] += points;
		}

		/* if all the above options were false, the option must be chance */
		else {
			points = checkChance(option, currentPlayer, currentDice, usedOptions);
			lowerTotal[currentPlayer] += points;
		}
	}
	
	else {
		/* print a message to user if they have entered an option that wasn't valid */
		printf("\nYou already used this option, try again.\nYour roll: ");

		/* print the users dice once again, and then ask the user to choose an option, etc. */
		printDice(currentDice, currentPlayer);
		option = chooseOption();
		points += performOption(option, currentPlayer, currentDice, usedOptions, upperTotal, lowerTotal);
		return; // return is to break out of the function as it is not needed 
	}

	printf("\nYou got %d points added to your score.\n", points);

	return points;
}

/*
 * Function calulateTotal - calculates the final score using arrays and sorts them from highest to lowest score
 * Param: numPlayers - the number of players in the game
 * Param: upperTotal - array of upper total scores for each player
 * Param: lowerTotal - array of lower total scores for each player
 * Param: grandTotal - array of lower total + upper total for each player
 * Param: allPlayers - array to store the index of the players and then to switch them according to the scores
 * Returns: all four arrays back to main
 */
void calculateTotal (int numPlayers, int upperTotal[][NUM_PLAYERS], int lowerTotal[], int grandTotal[], int allPlayers[]) {
	int player;
	int temp;
	int tempPlayer;
	int lower;
	int upper;

	/* for each player, calculate their total */
	for (player = 0; player < numPlayers; player++) {
		/* lower section total is set to a variable, and so is the upper section */
		/* the [0] is telling it that it is the first row of the array */
		lower = lowerTotal[player];
		upper = upperTotal[0][player];

		/* the second row of the upper total array determines if the bonus was applied or not */
		/* if the upper section is greater than or equal to 63, add 35 points */
		if (upper >= BONUS_CUTOFF) {
			upper += BONUS_UPPER_TOTAL;
			upperTotal[0][player] = upper;
			upperTotal[1][player] = TRUE;
		}
		/* add the two values and store them into the total for that player, where a different array stores the specific player index for the total */
		grandTotal[player] = lower + upper;
		allPlayers[player] = player;
	}

	/* sort the totals, and the player index will associate with the total */
	for (int index1 = 0; index1 < numPlayers; index1++) {
		for (int index2 = 0; index2 < numPlayers; index2++) {
			if (grandTotal[index1] > grandTotal[index2]) {
				temp = grandTotal[index1];
				tempPlayer = allPlayers[index1];
				grandTotal[index1] = grandTotal[index2];
				allPlayers[index1] = allPlayers[index2];
				grandTotal[index2] = temp;
				allPlayers[index2] = tempPlayer;
			}
		}
	}
}

/* 
 * Function printFile - prints a file line by line with formatting to the screen (including all characters)
 * Parameter: file - the file to be printed
 * Assumes that the file was already opened before being passed to the function
 */
void printFile(FILE * file) {
	char line[10000];
	/* checking if the file was opened correctly */
	if (file == NULL) {
		printf("Error opening file for reading.\n");
	}
	else {
		while (line != NULL) {
			/* print each line in the file one by one */
			if (fgets(line, 10000, file) == NULL)
				break;
			printf("%s", line);
		}
		/* close the file */
		fclose(file);
	}
}