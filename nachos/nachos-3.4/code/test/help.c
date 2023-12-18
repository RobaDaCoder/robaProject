/* help.c
 *	A simple program to output name of each members
 */

#include "syscall.h"

int main()
{
    	PrintString("==================================================\n");
	PrintString("     OS - NachOS Project     \n");
	PrintString("\n");
	PrintString("1/ Members\n");
	PrintString("	21120157 - Le Pham Hoang Trung.\n");
	PrintString("	21120415 - Tran Ngoc Bao.\n");
	PrintString("	21120554 - Le Van Tan.\n");
	PrintString("	20120568 - Nguyen Duc Tai.\n");
	PrintString("	20120076 - Mai Vinh Hien.\n");
    PrintString("2/ Available programs\n");
	PrintString("	int: Read and Print out integer.\n");
	PrintString("	char: Read and Print out character.\n");
	PrintString("	string: Read and Print out a string.\n");
	PrintString("	help: Print out this message and leave.\n");
	PrintString("	ascii: Print out the ascii table.\n");
	PrintString("	sort: The user input an array of integer\n");
	PrintString(" 	      Use bubble sort to sort the array.\n");
	PrintString("==================================================\n");
}
