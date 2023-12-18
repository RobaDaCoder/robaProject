#include "syscall.h"

int main()
{
	int a = 0;
	PrintString("Enter an integer: ");
	a = ReadInt();
	PrintString("You have enter integer: ");
	PrintInt(a);
	Halt();
	return 0;
}

