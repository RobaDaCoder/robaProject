#include "syscall.h"

int main()
{
	char* buffer = "";
	const int maxlen = 100;
	ReadString(buffer, maxlen);
	PrintString(buffer);
  	Halt();
	return 0;
}
