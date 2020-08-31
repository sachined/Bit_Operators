/**********************************************************************************

   This program...

   Save the output as a comment at the end of the program!
**********************************************************
**
**  Written By: Sachin Nediyanchath           // <-- write your name here
**	IDE: Microsoft Visual Studios 2017
**  Date: 03/09/2019                  // <-- write the date here

Strange error: 1>c:\users\administrator.win7-11\desktop\cis 26b 2018\assignment 4\26b_hw_4b.c(81): warning C4013: 'gets' undefined; assuming extern returning int
Still runs but this bothers me
***************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
A function macro that will turn on a range of bits
*/
#define TURNONLIGHTS(x,y) ((~(((unsigned int)~0) << x)) << y)
/*
A function macro that will turn off a range of bits
*/
#define TURNOFFLIGHTS(x,y) (~((~(((unsigned int)~0) << x)) << y))
/*
A function macro that will overlay a range of bits
*/
#define OVERLAY(x,y) (~(~(((unsigned short)~0) << x) << y)) 

void printmenu(void);
void userchoice(void);

int main( void )
{
	printf("\n");
	printmenu();
	userchoice();
	return 0;
}

/*
Print the menu of commands for user to alter lights
*/
void printmenu(void)
{
	printf("       1) turn on all lights\n"
		"       2) turn on center stage lights(lights 5 - 10)\n"
		"       3) turn on left stage lights(lights 11 - 15)\n"
		"       4) turn on right stage lights(lights 0 - 4)\n"
		"       5) turn off all lights\n"
		"       6) turn off center stage lights\n"
		"       7) turn off left stage lights\n"
		"       8) turn off right stage lights\n"
		"       9) enter on / off pattern\n"
		"       10) quit the menu\n");
}

/*
Function that deals with user input for primary menu
*/
void userchoice(void)
{
	char choice[30], *endptr, *lineptr, *tokenptr;
	int num, token, flag, count, letter = 0;
	static unsigned int lightsonfire = 0;
	unsigned int currentlights;
	unsigned int overlap(unsigned int runninglights);
	unsigned int userfunction(unsigned int runninglights, int user);
	void printbits(unsigned int wrd);
	void printmenu(void);

	while (printf("Enter menu choice: "), gets(choice), strcmp(choice, "10"))
	{
		flag = 0;														// Error indicator (0 for no errors, while 1 indicates an error)		
		if (*(choice) == '\0') flag = 1;								// If first character is null, change the flag to 1	
		num = (int)strtol(choice, &endptr, 10);							// Gets an integer from the user input  
		if (!flag && !isalpha(*(choice)) && (num == 0 || num > 10))					// If num is 0 (invalid option)
		{
			printf("Illegal menu selection! Try again!\n");				// Show error message and change flag to 1
			flag = 1;
		}
		if (*endptr != '\0')											// If the pointer after num is not null
		{
			lineptr = choice;											// A pointer will point to beginning of user input
			token = 0;													// Initalize token count to 0 
			while (token < 3 && !flag)									// While there are less than 3 tokens and no error flag indicated
			{
				tokenptr = strtok(lineptr, "\040\t");					// Get a token from input until reaching space or tab
				token++;												// Increment token count	
				tokenptr = strtok(NULL, "\040\t\0");					// Get another token...
				token++;
				if (tokenptr != NULL && token > 1)						// If the pointer is not null and there are more than one token...
				{
					printf("Only one menu selection allowed! Try again!\n");	// Error message and change flag to 1
					flag = 1;
				}
				if (tokenptr == NULL && token < 3)						// If pointer is null and there are less than 3 tokens
				{
					printf("Illegal menu selection! Try again!\n");		// Error message and change flag to 1
					flag = 1;
				}
			}
			count = strcspn(choice, "\0");								// Counts the number of characters in the user input
			for (int i = 0; i < count; i++)								// This for loop will check for letters
			{
				if (isdigit(*(choice + i)) || *(choice + i) == '\040' || *(choice + i) == '\t') letter = 0;
				else if (letter) break;
				else letter = 1;
			}
			if (!flag && letter)										// If there are letters...
			{
				printf("Illegal menu selection! Try again!\n");			// Error message and change flag to 1
				flag = 1;
			}
		}
		if (!flag)														// If flag is still 0
		{
			if (num >= 1 && num < 9) currentlights = userfunction(lightsonfire, num);	// If num is in the range of 1 and 8, the userfunction function will be used
			else if (num == 9) currentlights = overlap(lightsonfire);	//If num is 9, the overlap function will be used
			printf("Light settings: ");
			printbits(currentlights);									// This will print out the light configuration
			lightsonfire = currentlights;								// Store the current light configuration for future processing
		}
		printmenu();
	}
}

/*
Function that determines what functions to use based on input
1-4 will turn on the lights, while 5-8 will turn off the lights
Each number will alter light configuration differently
*/
unsigned int userfunction(unsigned int runninglights, int user)
{
	unsigned int temp = runninglights, mask;

	if (user == 1)
	{
		mask = TURNONLIGHTS(16, 0);
		temp |= mask;
	}
	if (user == 2)
	{
		mask = TURNONLIGHTS(6, 5);
		temp |= mask;
	}
	if (user == 3)
	{
		mask = TURNONLIGHTS(5, 11);
		temp |= mask;
	}
	if (user == 4)
	{
		mask = TURNONLIGHTS(5, 0);
		temp |= mask;
	}
	if (user == 5)
	{
		mask = TURNOFFLIGHTS(16, 0);
		temp &= mask;
	}
	if (user == 6)
	{
		mask = TURNOFFLIGHTS(6, 5);
		temp &= mask;
	}
	if (user == 7)
	{
		mask = TURNOFFLIGHTS(5, 11);
		temp &= mask;
	}
	if (user == 8)
	{
		mask = TURNOFFLIGHTS(5, 0);
		temp &= mask;
	}
	return temp;
}

/*
This function is integral for printbits function which is used in other functions
(borrowed from Part A with slight modifications
*/
int  testbit(unsigned int wrd, int bit_to_test)
{
	wrd >>= bit_to_test;		// Right bit-shift 
	wrd &= 1;
	return wrd;
}

/**************************************************************
This printbits function uses a trick to print an unsigned short as a string of
16 bits (borrowed from part A)
*/
void printbits(unsigned int wrd)
{
	int  testbit(unsigned int wrd, int bit_to_test);
	int i;

	for (i = 15; i >= 0; i--)
	{
		printf("%1d", testbit(wrd, i));
		if (i == 11 || i == 5) printf(" ");
	}
	printf("\n");
	return;
}

/*
Overlap function is accessed for option 9, which allows customization of light pattern
*/
unsigned int overlap(unsigned int runninglights)
{
	char setting[30], copy[30], *endptr, *lineptr, *weirdptr, *tokenptr, *leftptr = NULL, *rightptr = NULL;
	int flag, token, letter, count, startbit, numbit, bflag;
	unsigned int bnum, currentlights, mask;
	void printbits(unsigned int wrd);

	currentlights = runninglights;	// Stores current light settings before going into the function

	while (printf("Enter starting light number and on/off pattern: "), gets(setting), strcmp(setting, ""))
	{
		flag = 0;										// Used in the same way as the one from userfunction function
		strcpy(copy, setting);							// Copy the user input
		lineptr = copy;									//The pointer will point to beginning of user input
		token = 0;

		while (token < 3 && !flag)						// Count tokens in the user input along with error checking
		{
			tokenptr = strtok(lineptr, "\040\t");
			token++;
			if (token < 2) leftptr = tokenptr;		// If there are less than 2 tokens so far, left side of user input will be pointed to by leftptr
			if (tokenptr == NULL) break;			// If the pointer points to NULL, break out of this while loop
			tokenptr = strtok(NULL, "\t\040\0");	// Keep going through the user input...
			token++;
			if (token < 3) rightptr = tokenptr;		// If there are less than 3 tokens, right side of user input will be pointed to by rightptr
			lineptr = NULL;							// In case there are more than 2 tokens...should continue for sake of validation
			if (tokenptr == NULL && token < 3)		// If the pointer is null and there are less than 3 tokens (rightptr is usually null at this point)
			{
				printf("You did not enter a bit pattern! Try again!\n");	// Error message and change flag to 1
				flag = 1;
			}
			if (token > 3)							// If there are more than 3 tokens
			{
				printf("Too many entries on line! Try again!\n");			// Error message and change flag to 1 
				flag = 1;
			}
		}
		if (!flag)										// If the flag is still 0...
		{
			count = strlen(setting);	// This is why a copy string was used for previous while loop
			letter = 0;					// This flag is for letters (set to 0 to prepare for the for loop

			for (int i = 0; i < count; i++)		// This for loop will check for non-numbers in the user input
			{
				if (isdigit(*(setting + i)) || *(setting + i) == '\040' || *(setting + i) == '\t') letter = 0;
				else if (letter) break;
				else letter = 1;			// If a non-number is found (except tab and space), change letter flag to 1
			}
			if (letter && !flag)	// If there is a non-letter in the user input....
			{
				printf("Illegal on/off pattern! Try again!\n");	// Error message and change flag to 1
				flag = 1;
			}
		}
		bflag = 0;	// This flag indicates whether left or right pointers is binary (set to 0 now to enable checking)
		if (!flag && !letter)	// If letter flag and flag is 0
		{
			lineptr = strpbrk(leftptr, "0123456789");	// Point to any number that is 0 through 9

			if (*(lineptr) == '2' || *(lineptr) == '3' || *(lineptr) == '3' || *(lineptr) == '4' ||	// If 2-9
				*(lineptr) == '5' || *(lineptr) == '6' || *(lineptr) == '7' || *(lineptr) == '8' || *(lineptr) == '9')
			{
				if (*(lineptr + 1) == '\0')	// If the character after the initial one is null
				{
					startbit = (int)(*(lineptr)) - (int)'0';	// convert the character into startbit number
					bflag = 0;
				}
				else						// If any other character 
				{
					printf("Illegal on/off pattern! Try again!\n");	// Error message and change flag to 1
					flag = 1;
				}
				if (!flag)	// If flag is still 0, move on to the right pointer
				{
					if (*(rightptr) == '1' || *(rightptr) == '0')	// If first character is 1 or 0
					{
						weirdptr = strpbrk(rightptr, "23456789");	// Check for 2-9 to see if non-binary
						if (weirdptr == NULL)	// If none of the above numbers were found
						{
							bnum = (unsigned int)strtol(rightptr, &endptr, 2);	// Convert right pointer into binary 
							numbit = strcspn(rightptr, "\0");					// Count the number of characters that right pointer has before reaching null
							bflag = 1;											// Change binary flag to 1 for success!
						}
						//else bflag = 0;			// If any of the characters were found....
					}
					if ((15 - startbit) < (numbit - 1)) flag = 1; // If (numbit - 1) is greater than the difference between 15 and startbit, change flag to 1 
					if (startbit == 15 && numbit == 1) flag = 0;  // If numbit is 1 and startbit is 15, no change in flag
				}
				if (flag) printf("Illegal on / off pattern! Try again!\n");	// If flag is 1, error message
				if (!bflag)	// If binary flag is 0 after parsing, display error message and change flag to 1 						
				{
					printf("Illegal on / off pattern! Try again!\n");
					flag = 1;
				}
			}
			else if (*(lineptr) == '0' || *(lineptr) == '1')	// If first character of left pointer is 0 or 1
			{
				if (*(lineptr) == '1' && (*(lineptr + 1) == '2' || *(lineptr + 1) == '3' ||	// If 12,13,14, or 15
					*(lineptr + 1) == '4' || *(lineptr + 1) == '5') && *(lineptr + 2) == '\0')
				{
					startbit = (int)(*(lineptr)) - (int)'0';						//convert the characters into int for startbit
					startbit = startbit * 10 + ((int)*(lineptr + 1) - (int)'0');
					bflag = 0;														// Keep binary flag to 0, since yet to encounter binary
				}
				else if ((*(lineptr) == '1' && ((*(lineptr + 1) == '0' || *(lineptr + 1) == '1') && *(lineptr + 2) == '\0')
					|| *(lineptr + 1) == '\0') || (*(lineptr) == '0' && *(lineptr + 1) == '\0'))	// SPECIAL CASES: 0, 1, 10, or 11 (since they might be binary depending on right pointer)
				{
					weirdptr = strpbrk(rightptr, "23456789");	// Searches for 2-9 and makes a pointer at such occurrence
					if (*(rightptr) == '0' || *(rightptr) == '1')
					{
						if (weirdptr == NULL)	// If none of the above characters are found
						{
							bnum = (unsigned int)strtol(rightptr, &endptr, 2);	// right pointer becomes bnum 
							numbit = strcspn(rightptr, "\0");
							startbit = (int)(*(leftptr)) - (int)'0';			// left pointer becomes startbit
							if (*(leftptr + 1) != '\0') startbit = startbit * 10 + ((int)*(leftptr + 1) - (int)'0');
							bflag = 1;
						}
						else if ((*(rightptr) == '1' && (*(rightptr + 1) == '0' || *(rightptr + 1) == '1' || *(rightptr + 1) == '2'
							|| *(rightptr + 1) == '3' || *(rightptr + 1) == '4' || *(rightptr + 1) == '5') && *(rightptr + 2) == '\0')
							|| ((*(rightptr) == '0' || *(rightptr) == '1') && *(rightptr + 1) == '\0'))
						{
							startbit = (int)(*(rightptr)) - (int)'0'; // Right pointer becomes startbit
							if (*(rightptr + 1) != '\0') startbit = startbit * 10 + ((int)*(rightptr + 1) - (int)'0');
							bnum = (unsigned int)strtol(leftptr, &endptr, 2);	// Left pointer becomes bnum
							numbit = strcspn(leftptr, "\0");
							bflag = 1;
						}
					}
					else if (weirdptr != NULL && *(weirdptr + 1) == '\0')	// If there are non 1/0 numbers found in right pointer i.e. 2-9
					{
						startbit = (int)(*(rightptr)) - (int)'0'; // Right pointer becomes startbit
						if (*(rightptr + 1) != '\0') startbit = startbit * 10 + ((int)*(rightptr + 1) - (int)'0');
						bnum = (unsigned int)strtol(leftptr, &endptr, 2);	// Left pointer becomes bnum
						numbit = strcspn(leftptr, "\0");
						bflag = 1;
					}
					else bflag = 0;	// Otherwise no binary number found...
				}
				else
				{
					bnum = (unsigned int)strtol(leftptr, &endptr, 2);
					numbit = strcspn(leftptr, "\0");
					startbit = (int)(*(rightptr)) - (int)'0';
					if (*(rightptr + 1) != '\0') startbit = startbit * 10 + ((int)*(rightptr + 1) - (int)'0');
					bflag = 1;
				}
			}
			if (!flag && !bflag)	// If flag and binary flag are 0
			{
				if (*(rightptr) == '1' || *(rightptr) == '0')
				{
					bnum = (unsigned int)strtol(rightptr, &endptr, 2);	// Right pointer is bnum
					numbit = strcspn(rightptr, "\0");
					bflag = 1;
				}
				if (!bflag)
				{
					printf("Illegal on / off pattern! Try again!\n");
					flag = 1;
				}
			}
			if ((15 - startbit) < (numbit - 1)) flag = 1;
			if (startbit == 15 && numbit == 1) flag = 0;
			if (bflag && flag) printf("Illegal on / off pattern! Try again!\n");
		}
		if (!flag && bflag)
		{
			currentlights = runninglights;
			mask = OVERLAY(numbit, startbit);
			currentlights &= mask;
			currentlights |= bnum << startbit;
		}
		if (bflag && !flag) break;	// If binary flag is 1 and flag is 0, please leave the while loop to return a value
	}
	return currentlights;
}

/*	================= Sample Output ================= */
/*	Results:
 
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: pooh
Illegal menu selection! Try again!
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice:
	   1) turn on all lights
	   2) turn on center stage lights(lights 5 - 10)
	   3) turn on left stage lights(lights 11 - 15)
	   4) turn on right stage lights(lights 0 - 4)
	   5) turn off all lights
	   6) turn off center stage lights
	   7) turn off left stage lights
	   8) turn off right stage lights
	   9) enter on / off pattern
	   10) quit the menu
	   Enter menu choice:
Enter menu choice: 3 poo
Only one menu selection allowed! Try again!
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 3 5
Only one menu selection allowed! Try again!
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 1fe
Illegal menu selection! Try again!
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 1
Light settings: 11111 111111 11111
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 5
Light settings: 00000 000000 00000
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 2
Light settings: 00000 111111 00000
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 4
Light settings: 00000 111111 11111
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 3
Light settings: 11111 111111 11111
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 6
Light settings: 11111 000000 11111
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 9
Enter starting light number and on/off pattern:
Light settings: 11111 000000 11111
	   1) turn on all lights
	   2) turn on center stage lights(lights 5 - 10)
	   3) turn on left stage lights(lights 11 - 15)
	   4) turn on right stage lights(lights 0 - 4)
	   5) turn off all lights
	   6) turn off center stage lights
	   7) turn off left stage lights
	   8) turn off right stage lights
	   9) enter on / off pattern
	   10) quit the menu
	   Enter menu choice:
Enter menu choice: 8
Light settings: 11111 000000 00000
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 7
Light settings: 00000 000000 00000
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 9
Enter starting light number and on/off pattern: g ,fg
Illegal on/off pattern! Try again!
Enter starting light number and on/off pattern: .df
You did not enter a bit pattern! Try again!
Enter starting light number and on/off pattern: 5 10 93
Too many entries on line! Try again!
Enter starting light number and on/off pattern: 101011 5
Light settings: 00000 101011 00000
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 9
Enter starting light number and on/off pattern: 2 11110111
Light settings: 00000 111110 11100
       1) turn on all lights
       2) turn on center stage lights(lights 5 - 10)
       3) turn on left stage lights(lights 11 - 15)
       4) turn on right stage lights(lights 0 - 4)
       5) turn off all lights
       6) turn off center stage lights
       7) turn off left stage lights
       8) turn off right stage lights
       9) enter on / off pattern
       10) quit the menu
Enter menu choice: 10
Press any key to continue . . .
 
*/
