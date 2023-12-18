#include "syscall.h"

int main()
{
	int a = ReadInt();
	PrintInt(a);
	Halt();
	return 0;
}
