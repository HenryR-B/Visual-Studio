#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

/* symbolic constants */
#define MAX_DICE		5
#define NUM_PLAYERS		4
#define TRUE		1
#define FALSE		0
#define MAX_DICE	5
#define OPTIONS		13
#define NUM_PLAYERS		4
#define MAX_LENGTH		100
#define BONUS_UPPER_TOTAL	35
#define BONUS_CUTOFF		63
#define BONUS_SECTION 2
#define BUF_SIZE	1000

/* function prototypes */
int indexOfSmallest(int data[][MAX_DICE], int size, int startIndex, int currentPlayer);
int dataOfLargest(int data[][MAX_DICE], int size, int startIndex, int currentPlayer);
void sortArray(int data[][MAX_DICE], int currentPlayer);
int checkChar(void);
void printDice(int currentDice[][MAX_DICE], int currentPlayer);
int checkPlayers(void);
void rollNotKeepers(int currentPlayer, int currentDice[][MAX_DICE]);
int chooseOption(void);
int chooseCombination(void);
int checkLowerUsed(int currentPlayer, int usedOptions[][OPTIONS]);
int check_1_to_6(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]);
int checkFullHouse(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]);
int check_3_4_5_Kind(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[]);
int checkStraight(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]);
int checkChance(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]);
int inputString(char string[BUF_SIZE], int length);
void checkValidInput(char input[BUF_SIZE]);

/*
 * Funtion dataOfLargest - finds the largest value in the array from startIndex to the end
 * Param: data - the data to be searched
 * Param: size - the size of the array
 * Param: startIndex - the first index to start looking at
 * Param: currentPlayer - the current player taking their turn
 * Returns: maximum value from startIndex to the end of the array
 */
int dataOfLargest(int data[][MAX_DICE], int size, int startIndex, int currentPlayer) {
	int index;
	int max = 0;

	for (index = 0; index < size; index++) {
		if (data[currentPlayer][index] > max) {
			max = data[currentPlayer][index];
		}
	}

	return max;
}

/*
* Funtion indexOfSmallest - finds the largest value in the array from startIndex to the end
* Param: data - the data to be searched
* Param: size - the size of the array
* Param: startIndex - the first index to start looking at
* Param: currentPlayer - the current player taking their turn
* Returns: index where the minimum value is found from startIndex to the end of the array
*/
int indexOfSmallest(int data[][MAX_DICE], int size, int startIndex, int currentPlayer) {
	int index;
	int foundIndex = 0;
	int min = dataOfLargest(data, size, startIndex, currentPlayer) + 1;

	for (index = startIndex; index < size; index++) {
		if (data[currentPlayer][index] < min) {
			min = data[currentPlayer][index];
			foundIndex = index;
		}
	}

	return foundIndex;
}

/*
* Funtion sortArray - sorts the array in ascending order
* Param: data - the data to be searched
* Param: currentPlayer - the current player taking their turn
* Returns: sorted array
*/
void sortArray(int data[][MAX_DICE], int currentPlayer) {
	int startIndex;
	int indexOfMin;
	int temp;
	int size = MAX_DICE;

	for (startIndex = 0; startIndex < size; startIndex++) {
		indexOfMin = indexOfSmallest(data, size, startIndex, currentPlayer);
		temp = data[currentPlayer][indexOfMin];
		data[currentPlayer][indexOfMin] = data[currentPlayer][startIndex];
		data[currentPlayer][startIndex] = temp;
	}
}

/*
 * Function checkChar - assumes prompt is outside the function, user enters a integer - program rejects it if it is a character
 * Returns: the number entered, 0 if it is an invalid number
 */
int checkChar(void) {
	unsigned int i;
	int num;
	char buffer[BUFSIZ];
	if (fgets(buffer, sizeof buffer, stdin) != NULL) {
		/* watch out for the ';' at the end of the next line (for) */
		for (i = 0; (i < strlen(buffer) - 1) && isdigit(buffer[i]); i++);

		if (i == strlen(buffer) - 1)
			num = atoi(buffer);
		else
			printf("\nInvalid number\n\n");
	}
	else
		printf("Error: Input validation\n");

	return num;
}

/*
 * Function printDice - prints the current dice to the screen in a graphical format
 * Param: currentDice - the current dice in play
 * Param: currentPlayer - the current player taking their turn
 * Uses similarities to print all 5 dice side by side
 */
void printDice(int currentDice[][MAX_DICE], int currentPlayer) {
	printf("\n -------      -------      -------      -------      ------- \n");
	for (int die = 0; die < MAX_DICE; die++) {
		if (currentDice[currentPlayer][die] == 1)
			printf("|       |    ");
		else if (currentDice[currentPlayer][die] == 2 || currentDice[currentPlayer][die] == 3)
			printf("| 0     |    ");
		else
			printf("| 0   0 |    ");
	}
	printf("\n");
	for (int die = 0; die < MAX_DICE; die++) {
		if (currentDice[currentPlayer][die] == 2 || currentDice[currentPlayer][die] == 4)
			printf("|       |    ");
		else if (currentDice[currentPlayer][die] == 6)
			printf("| 0   0 |    ");
		else
			printf("|   0   |    ");
	}
	printf("\n");
	for (int die = 0; die < MAX_DICE; die++) {
		if (currentDice[currentPlayer][die] == 1)
			printf("|       |    ");
		else if (currentDice[currentPlayer][die] == 2 || currentDice[currentPlayer][die] == 3)
			printf("|     0 |    ");
		else
			printf("| 0   0 |    ");
	}
	printf("\n -------      -------      -------      -------      ------- \n");
}

/* 
 * Function checkPlayers - checks for how many players will play yahtzee
 * Returns: the number of players who will play
 */
int checkPlayers(void) {
	int numPlayers = 0;
	int count = 0;

	/* prompt user for the number of players, and check if it is a valid input */
	printf("\n\nPlease enter number of players who will play (1-4 players only): ");
	numPlayers = checkChar();
	while (numPlayers < 1 || numPlayers > 4) {
		if (count > 0)
			printf("\n\nPlease enter number of players who will play (1-4 players only): ");
		numPlayers = checkChar();
		count++;
	}
	printf("\n\n");

	return numPlayers;
}

/* 
 * Function rollNotKeepers - rolls the dice that the user does not want to keep
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play
 * Returns: array of dice via pointer
 */
void rollNotKeepers(int currentPlayer, int currentDice[][MAX_DICE]) {
	int index;
	int count;
	char keep;
	int keepers[MAX_DICE] = { FALSE };
	char buffer[BUF_SIZE];

	/* asks user if they want to keep each dice individually, and it is set as a 'keeper' */
	for (index = 0; index < MAX_DICE; index++) {
		printf("\nDo you want to keep dice #%d? (Y/N) ", index + 1);
		checkValidInput(buffer);
		keep = buffer[0];
		if (keep == 'Y' || keep == 'y') {
			keepers[index] = TRUE;
		}
	}
	/* for each dice, determine if it is a 'keeper', if it is, re-roll the dice for that index */
	for (count = 0; count < MAX_DICE; count++) {
		if (keepers[count] == FALSE) {
			currentDice[currentPlayer][count] = rollDie();
		}
	}

	/* sort and print the dice */
	printf("\nYou have rolled: ");
	sortArray(currentDice, currentPlayer);
	printDice(currentDice, currentPlayer);
}

/*
 * Function chooseOption - prompts the user which option they would like to choose
 * Returns: option - the option that they want to perform
 */
int chooseOption(void) {
	int option = 0;
	int count = 0;

	printf("\nChoose which option you would like to use (enter number associated with that option):\n");

	printf("\nUpper Section\n\n");
	printf("1. Ones			- sum of all ones\n");
	printf("2. Twos			- sum of all twos\n");
	printf("3. Threes		- sum of all threes\n");
	printf("4. Fours		- sum of all fours\n");
	printf("5. Fives		- sum of all fives\n");
	printf("6. Sixes		- sum of all sixes\n");
	printf("\nLower Section\n\n");
	printf("7. Three of a kind	- sum of all values\n");
	printf("8. Four of a kind	- sum of all values\n");
	printf("9. Small Straight	- 30 Points (4 consecutive)\n");
	printf("10. Large Straight	- 40 Points (5 consecutive)\n");
	printf("11. Full House		- 25 Points (3 of a kind and 2 of a kind)\n");
	printf("12. Chance	    	- sum of all values\n");
	printf("13. First Yahtzee	- 50 Points (5 of a kind)\n");
	printf("14. Yahtzee Bonus	- 100 Points (5 of a kind)\n");
	printf("\nChose your option: ");
	option = checkChar();

	/* makes sure the option is a valid number and within the range */
	while (option < 1 || option > 14) {
		printf("\nError, you need to choose a valid option.\n");
		printf("\n\nChoose your option: ");
		option = checkChar();
		count++;
	}

	return option;
}

/*
 * Function chooseCombination - prompts user if they want to choose an option
 * Returns: TRUE if they want to choose an option, FALSE if not
 */
int chooseCombination(void) {
	char input;
	int choice;
	char buffer[BUF_SIZE];

	printf("\n\nDo you want to choose an option (eg. Three of a Kind) for this roll? (Y/N): ");
	checkValidInput(buffer);
	input = buffer[0];

	if (input == 'Y' || input == 'y') {
		choice = TRUE;
	}
	else
		choice = FALSE;

	return choice;
}

/* 
 * Function checkLowerUsed - checks if the options 9, 10 or 11 are all used for the specific player
 * Param: currentPlayer - the current player who is taking their turn
 * Param: usedOptions - the options already used by the player
 * Returns: TRUE if they are all used, FALSE if not
 */
int checkLowerUsed(int currentPlayer, int usedOptions[][OPTIONS]) {
	int allUsed;

	for (int index = 8; index < 11; index++) {
		if (usedOptions[currentPlayer][index] == TRUE) {
			allUsed = TRUE;
		}
		else
			allUsed = FALSE;
	}
	return allUsed;
}

/*
 * Function check_1_to_6 - calculates the points for the upper section
 * Param: option - the option chosen by the current player
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play by the player
 * Param: usedOptions - the options that are already used by the player
 * Returns: the points the user accumulated for this roll
 */
int check_1_to_6(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]) {
	int points = 0;

	for (int index = 0; index < MAX_DICE; index++) {
		if (currentDice[currentPlayer][index] == option)
			points += option;
	}
	/* mark this option as used */
	usedOptions[currentPlayer][option - 1] = TRUE;

	return points;
}

/*
 * Function checkFullHouse - checks if there is actually a full house, and adds up the points if true
 * Param: option - the option chosen by the current player
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play by the player
 * Param: usedOptions - the options that are already used by the player
 * Returns: the points the user accumulated for this roll
 */
int checkFullHouse(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]) {
	int count = 0;
	int lastCount = 0;
	int countREV;
	int valueFWD;
	int valueREV;
	int points;

	char arr[] = {26};
	int lastIndex = MAX_DICE - 1;
	int firstIndex = 0;

	/* checks if there are at least two dice in a row checking forward */
	for (int index = 0; index < MAX_DICE; index++) {
		if (currentDice[currentPlayer][firstIndex] == currentDice[currentPlayer][index]) {
			count++;
		}
		/* did not read another same value, so break out of the loop */
		if (lastCount == count) {
			valueFWD = currentDice[currentPlayer][firstIndex];
			break;
		}
		lastCount = count;
	}

	/* check the array in reverse order to the index where the forward count stopped - checks if there are the opposite of what the forward checked */
	lastIndex = MAX_DICE - 1;
	countREV = 0;
	for (int indexRev = MAX_DICE - 1; indexRev >= lastCount; indexRev--) {
		if (currentDice[currentPlayer][lastIndex] == currentDice[currentPlayer][indexRev])
			countREV++;
	}
	valueREV = currentDice[currentPlayer][lastIndex];

	/* Only two possiblities for the full house, either you have 3 of the same and then 2, or 2 and then 3*/
	if (lastCount == 2 && countREV == 3 || lastCount == 3 && countREV == 2) {
		printf("\nFull House! (%d's and %d's)\n", valueFWD, valueREV);
		points = 25;
		/* mark this option as used */
		usedOptions[currentPlayer][option - 1] = TRUE;
	}

	else
		points = 0;

	return points;
}

/*
 * Function check_3_4_5_Kind - checks if there is a 3 of a kind, 4 of a kind or a yahtzee
 * Param: option - the option chosen by the current player
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play by the player
 * Param: usedOptions - the options that are already used by the player
 * Param: upperTotal - the current upper section total for the player
 * Param: lowerTotal - the current lower section total for the player
 * Returns: the points the user accumulated for this roll
 */
int check_3_4_5_Kind(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS], int upperTotal[][NUM_PLAYERS], int lowerTotal[]) {
	int count;
	int lastCount;
	int valueFWD;
	int points = 0;
	int input = 0;
	int allUsed;

	/* checking for how many same dice are in a row */
	for (int index = 0; index < MAX_DICE - 2; index++) {
		count = 0;
		lastCount = 0;
		for (int index2 = index; index2 < MAX_DICE; index2++) {
			if (currentDice[currentPlayer][index] == currentDice[currentPlayer][index2]) {
				count++;
			}
			/* didnt read the same value -> break out of the loop */
			if (lastCount == count) {
				break;
			}
			lastCount = count;
		}
		/* if the count is greater than 3 already, break out of the loop */
		if (count >= 3) {
			valueFWD = currentDice[currentPlayer][index];
			break;
		}
	}

	/* checking for a valid 3 of a kind */
	if (count >= 3 && option == 7) {
		/* add the points and mark option as used, as well as add the points to the lower total */
		printf("\nThree of a Kind (%d's)!! \n", valueFWD);
		for (int index = 0; index < MAX_DICE; index++)
			points += currentDice[currentPlayer][index];
		usedOptions[currentPlayer][option - 1] = TRUE;
		lowerTotal[currentPlayer] += points;
	}
	/* checking for a valid 4 of a kind */
	else if (count >= 4 && option == 8) {
		/* add the points and mark option as used, as well as add the points to the lower total */
		printf("\nFour of a Kind (%d's)!!\n", valueFWD);
		for (int index = 0; index < MAX_DICE; index++)
			points += currentDice[currentPlayer][index];
		usedOptions[currentPlayer][option - 1] = TRUE;
		lowerTotal[currentPlayer] += points;
	}
	/* checking for a valid First Yahtzee */
	else if (count == 5 && option == 13) {
		/* add the points and mark option as used, as well as add the points to the lower total */
		printf("\nYAHTZEE (%d's)!!!\n", valueFWD);
		points = 50;
		usedOptions[currentPlayer][option - 1] = TRUE;
		lowerTotal[currentPlayer] += points;
	}

	/* checking for a valid Bonus Yahtzee */
	else if (count == 5 && option == 14) {
		printf("\nYAHTZEE BONUS (%d's)!!!\n", valueFWD);
		points = 100;
		/* adding bonus respective points to upper section */
		if (usedOptions[currentPlayer][valueFWD - 1] == FALSE) {
			printf("\nEntering bonus points to %d's\n", valueFWD);
			points += check_1_to_6(valueFWD, currentPlayer, currentDice, usedOptions);
			upperTotal[0][currentPlayer] += points;
			usedOptions[currentPlayer][valueFWD - 1] = TRUE;
		}
		/* if upper respective section is full, check 3 of a kind, then 4 of a kind, then chance */
		else if (usedOptions[currentPlayer][6] == FALSE) {
			printf("\nEntering bonus points into 3 of a kind\n");
			for (int index = 0; index < MAX_DICE; index++)
				points += currentDice[currentPlayer][index];
			usedOptions[currentPlayer][6] = TRUE;
			lowerTotal[currentPlayer] += points;
		}
		else if (usedOptions[currentPlayer][7] == FALSE) {
			printf("\nEntering bonus points into 4 of a kind\n");
			for (int index = 0; index < MAX_DICE; index++)
				points += currentDice[currentPlayer][index];
			usedOptions[currentPlayer][7] = TRUE;
			lowerTotal[currentPlayer] += points;
		}
		else if (usedOptions[currentPlayer][11] == FALSE) {
			printf("\nEntering bonus points into chance\n");
			for (int index = 0; index < MAX_DICE; index++)
				points += currentDice[currentPlayer][index];
			usedOptions[currentPlayer][11] = TRUE;
			lowerTotal[currentPlayer] += points;
		}
		else {
			/* if all the above categories are full, you can 'steal' a different option */
			printf("\nWhich category do you want to steal? Enter number associated with the option\n");
			printf("9. Small Straight	- 30 Points (4 consecutive)\n");
			printf("10. Large Straight	- 40 Points (5 consecutive)\n");
			printf("11. Full House		- 25 Points (3 of a kind and 2 of a kind)\n");
			printf("\nChoose your option: ");
			input = checkChar();

			/* make sure that the options 9-11 aren't all used */
			allUsed = checkLowerUsed(currentPlayer, usedOptions);
			while ((input < 9 || input > 11) || (usedOptions[currentPlayer][input - 1] == TRUE && allUsed == FALSE)) {
				printf("\nError, enter a valid option. \n");
				printf("\nChoose your option: ");
				input = checkChar();
			}
			/* as long as the option hasn't been used, enter the points into the respective category */
			if (usedOptions[currentPlayer][input - 1] == FALSE) {
				switch (input) {
				case 9:
					printf("\nEntering 30 bonus points into small straight\n");
					points += 30;
					lowerTotal[currentPlayer] += points;
					break;
				case 10:
					printf("\nEntering 40 bonus points into large straight\n");
					points += 40;
					lowerTotal[currentPlayer] += points;
					break;
				case 11:
					printf("\nEntering 25 bonus points into full house\n");
					points += 25;
					lowerTotal[currentPlayer] += points;
					break;
				}
				usedOptions[currentPlayer][input - 1] = TRUE;
			}

			else {
				printf("\nNo bonus points available\n");
				points += 0;
			}

		}
	}

	return points;
}

/* 
 * Function checkStraight - checks if there is a valid straight and gets the points
 * Param: option - the option chosen by the current player
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play by the player
 * Param: usedOptions - the options that are already used by the player
 * Returns: the points the user accumulated for this roll
 */
int checkStraight(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]) {
	int count = 0;
	int lastCount = 0;
	int points;
	int lastIndex = MAX_DICE - 1;
	int firstIndex = 0;

	/* checks if there are consecutive dice - increases the counter if found consecutive */
	for (int index = 1; index < MAX_DICE; index++) {
		if (currentDice[currentPlayer][index] > currentDice[currentPlayer][firstIndex]) {
			firstIndex = index;
			count++;
		}
	}
	/*
	if (firstIndex < 3) {
		for (int index = 0; index < MAX_DICE; index++) {
			if (currentDice[currentPlayer][index] == currentDice[currentPlayer][index + 1]) {
				temp = currentDice[currentPlayer][index];

				for (int index2 = 1; index2 < MAX_DICE; index2++)
					currentDice[currentPlayer][index2] = currentDice[currentPlayer][index2 + 1];
				currentDice[currentPlayer][MAX_DICE - 1] = temp;
			}
		}

		firstIndex = 0;
		count = 0;
		for (int index = 1; index < MAX_DICE; index++) {
			if (currentDice[currentPlayer][index] > currentDice[currentPlayer][firstIndex]) {
				firstIndex = index;
				count++;
			}
		}
	}
	*/

	/* read 4 or more consecutive values, and the option = 9 -> small straight */
	if (count >= 3 && option == 9) {
		printf("\nSmall Straight!\n");
		points = 30;
		/* mark the option as used for the player */
		usedOptions[currentPlayer][option - 1] = TRUE;
	}

	/* read 5 consecutive values, and the option is 10 -> large straight */
	else if (count == 4 && option == 10) {
		printf("\nLarge Straight!\n");
		points = 40;
		/* mark the option as used for the player */
		usedOptions[currentPlayer][option - 1] = TRUE;
	}

	else {
		points = 0;
	}

	return points;
}

/*
 * Function checkChance - adds up all the values no matter what stores it as the points
 * Param: option - the option chosen by the current player
 * Param: currentPlayer - the current player taking their turn
 * Param: currentDice - the current dice in play by the player
 * Param: usedOptions - the options that are already used by the player
 * Returns: the points the user accumulated for this roll
 */
int checkChance(int option, int currentPlayer, int currentDice[][MAX_DICE], int usedOptions[][OPTIONS]) {
	int points = 0;
	int index;

	printf("\nChance\n");
	for (index = 0; index < MAX_DICE; index++)
		points += currentDice[currentPlayer][index];
	/* mark the option as used for the player */
	usedOptions[currentPlayer][option - 1] = TRUE;

	return points;
}

/*
 * Function inputString - gets (a) input character(s) from user and when user presses enter, stores the newline character as a null byte '\0'
 * Param: string - array of input character(s)
 * Param: length - max length of string
 * Returns: amount of letters entered
 */
int inputString(char string[BUF_SIZE], int length) {
	int x;

	fgets(string, length, stdin);
	for (x = 0; x <= length; x++) {
		if (string[x] == '\n') {
			string[x] = '\0';
			break;
		}
	}

	return x;
}

/*
 * Function checkValidInput - gets the input from user (a character) and repeats if user enters more than one or none at all
 * Param: input - the array for the input character(s)
 * Returns: the array of a single character (at the end of the cycle, it will be one character)
 */
void checkValidInput(char input[BUF_SIZE]) {
	int count;
	int letters;
	count = 0;

	do {
		if (count > 0)
			printf("\nPlease enter exactly ONE character: ");
		letters = inputString(input, BUF_SIZE);

		count++;
	} while (letters != 1);
}