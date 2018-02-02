#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MIN_VAL 1
#define MAX_VAL 16

int main(void) {
	int num;
	int count;
	int row = 1;
	int valCount = 1;

	printf("Enter integer between 1 and 16, inclusive: ");
	scanf("%d", &num);
	while (num < MIN_VAL || num > MAX_VAL) {
		printf("Error...");

		printf("Enter integer between 1 and 16, inclusive: ");
		scanf("%d", &num);
	}
	
	printf("%d\n\n", num);

	while (row <= num) {
		count = row;
		valCount = num + row - 1;
		while (count <= valCount) {
			printf("%3d ", count * 2);
			count++;
		}
		row++;
		printf("\n");
	}

	printf("\n");

	system("PAUSE");
	return 0;
}