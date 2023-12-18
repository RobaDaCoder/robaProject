#include "syscall.h"

int main()
{
	char* buffer = "";
	const int maxlen = 100;
	PrintString("Enter a string: ");
	ReadString(buffer, maxlen);
	PrintString("You have enter the string: ");
	PrintString(buffer);
  	Halt();
	return 0;
}
