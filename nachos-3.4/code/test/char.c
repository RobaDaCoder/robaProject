#include "syscall.h"

int main()
{	
	char character;
	PrintString("Enter a character: ");
	character = ReadChar();
	PrintString("You have enter the character: ");
	PrintChar(character);
  	Halt();
	return 0;
}
