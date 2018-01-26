#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define INPUTFILE "yvrTemperatures08.dat"

int main(void) {
	FILE* inFile;
	int day;
	double midnight;
	double sixAM;
	double noon;
	double sixPM;
	double minimum = 10;
	double maximum = 0;

	inFile = fopen(INPUTFILE, "r");

	if (inFile == NULL)
		printf("Error opening input file\n");
	else {
		while (fscanf(inFile, "%d", &day) == 1) {
			fscanf(inFile, "%lf %lf %lf %lf", &midnight, &sixAM, &noon, &sixPM);
			if (noon < minimum)
				minimum = noon;
			if (noon > maximum)
				maximum = noon;
		}

		printf("Temperatures from Noon, each day of the year, Max: %.1f, Min: %.1f\n", maximum, minimum);
		fclose(inFile);
	}

	system("PAUSE");
	return 0;
}
